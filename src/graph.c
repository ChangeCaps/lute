// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <assert.h>
#include <dirent.h>
#include <lute/build.h>

#include "graph.h"
#include "load.h"
#include "util.h"

static char *pkg_config_flags(const char *flags, const char *name) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "pkg-config --%s %s", flags, name);

    FILE *fp = popen(cmd, "r");

    if (!fp) {
        printf("Error: Could not run pkg-config\n");
        return NULL;
    }

    char buffer[4096];
    if (!fgets(buffer, sizeof(buffer), fp)) {
        printf("Error: Could not read pkg-config output\n");
        pclose(fp);
        return NULL;
    }

    pclose(fp);

    // remove trailing newline
    if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
    }

    return strdup(buffer);
}

bool build_package_init(BuildPackage *package, const char *name) {
    package->cflags = pkg_config_flags("cflags", name);
    package->libs = pkg_config_flags("libs", name);

    if (!package->cflags || !package->libs) {
        free(package->cflags);
        free(package->libs);
        return false;
    }

    package->name = strdup(name);

    return true;
}

void build_package_free(BuildPackage *package) {
    free(package->name);
    free(package->cflags);
    free(package->libs);
}

void build_target_free(BuildTarget *target) {
    free(target->name);

    vec_foreach(&target->packages, package) free(package);
}

void build_node_init(BuildNode *node) { vec_init(&node->targets); }

void build_node_free(BuildNode *node) {
    vec_foreachat(&node->targets, target) build_target_free(target);
    vec_free(&node->targets);
}

BuildTarget *build_node_target(BuildNode *node, const char *name) {
    vec_foreachat(&node->targets, target) {
        if (strcmp(target->name, name) == 0)
            return target;
    }

    return NULL;
}

static bool is_url_shorthand(const char *url, const char *shorthand) {
    return memcmp(url, shorthand, strlen(shorthand)) == 0;
}

static char *expand_url_shorthand(const char *url, const char *shorthand,
                                  const char *expanded) {
    char *out = malloc(strlen(url) + strlen(expanded) - strlen(shorthand) + 1);

    strcpy(out, expanded);
    strcat(out, url + strlen(shorthand));

    return out;
}

static char *expand_dep_url(const char *url) {
    char *github_short = "github:";
    char *github_long = "git@github.com:";
    char *gitlab_short = "gitlab:";
    char *gitlab_long = "git@gitlab.com:";

    if (is_url_shorthand(url, github_short))
        return expand_url_shorthand(url, github_short, github_long);
    if (is_url_shorthand(url, gitlab_short))
        return expand_url_shorthand(url, gitlab_short, gitlab_long);

    return strdup(url);
}

void build_dep_init(BuildDep *dep, const char *url, const char *name) {
    dep->url = expand_dep_url(url);
    dep->name = strdup(name);

    hash_string(dep->id, "dep", dep->url);
    dep->node = NULL;
    dep->target = NULL;
}

void build_dep_free(BuildDep *dep) { free(dep->url); }

void build_graph_init(BuildGraph *graph) {
    vec_init(&graph->packages);
    vec_init(&graph->deps);
    vec_init(&graph->nodes);
    vec_init(&graph->paths);
    graph->root = NULL;
}

void build_graph_free(BuildGraph *graph) {
    vec_foreach(&graph->packages, package) {
        build_package_free(package);
        free(package);
    }

    vec_foreach(&graph->deps, dep) {
        build_dep_free(dep);
        free(dep);
    }

    vec_foreach(&graph->nodes, node) {
        build_node_free(node);
        free(node);
    }

    vec_foreach(&graph->paths, path) free(path);

    vec_free(&graph->packages);
    vec_free(&graph->deps);
    vec_free(&graph->nodes);
}

static char *build_add_path(BuildGraph *graph, const char *path) {
    char *real = realpath(path, NULL);

    vec_foreach(&graph->paths, p) if (strcmp(p, real) == 0) return p;

    vec_push(&graph->paths, real);
    return real;
}

static bool build_add_source(BuildGraph *graph, const char *path,
                             Paths *paths) {
    if (!is_dir(path)) {
        char *ext = strrchr(path, '.');

        if (!ext || (strcmp(ext, ".c") != 0 && strcmp(ext, ".cpp") != 0)) {
            return true;
        }

        vec_push(paths, build_add_path(graph, path));
        return true;
    }

    DIR *dir = opendir(path);

    if (!dir) {
        printf("Error: Could not open directory %s\n", path);
        return false;
    }

    struct dirent *entry;

    while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char *new_path = malloc(strlen(path) + strlen(entry->d_name) + 2);

        if (path[strlen(path) - 1] == '/') {
            sprintf(new_path, "%s%s", path, entry->d_name);
        } else {
            sprintf(new_path, "%s/%s", path, entry->d_name);
        }

        if (!build_add_source(graph, new_path, paths)) {
            free(new_path);
            closedir(dir);
            return false;
        }

        free(new_path);
    }

    closedir(dir);

    return true;
}

static BuildPackage *build_add_package(BuildGraph *graph, const char *name) {
    vec_foreach(&graph->packages, package) {
        if (strcmp(package->name, name) == 0) {
            return package;
        }
    }

    BuildPackage *build_package = malloc(sizeof(BuildPackage));

    if (!build_package_init(build_package, name)) {
        free(build_package);
        return NULL;
    }

    vec_push(&graph->packages, build_package);

    return build_package;
}

static bool build_graph_load_target(BuildGraph *graph,
                                    BuildTarget *build_target,
                                    const Target *target) {

    vec_init(&build_target->sources);
    vec_foreach(&target->sources, source) {
        if (!build_add_source(graph, source, &build_target->sources)) {
            vec_free(&build_target->sources);

            return false;
        }
    }

    vec_init(&build_target->packages);
    vec_foreach(&target->packages, package) {
        BuildPackage *build_package = build_add_package(graph, package);

        if (!build_package) {
            vec_free(&build_target->sources);
            vec_free(&build_target->packages);

            return false;
        }

        vec_push(&build_target->packages, build_package);
    }

    vec_init(&build_target->includes);
    vec_foreach(&target->includes, include) {
        vec_push(&build_target->includes, build_add_path(graph, include));
    }

    vec_init(&build_target->deps);
    vec_foreach(&target->deps, dep) {
        BuildDep *build_dep = malloc(sizeof(BuildDep));
        build_dep_init(build_dep, dep.url, dep.target);
        vec_push(&build_target->deps, build_dep);
    }

    build_target->name = strdup(target->name);
    build_target->output = target->output;
    build_target->warn = target->warn;
    build_target->lang = target->lang;
    build_target->std = target->std ? strdup(target->std) : NULL;

    return true;
}

static BuildNode *build_graph_load_node(BuildGraph *graph,
                                        const char *build_path,
                                        const char *out_path) {
    Build build;
    if (!load_build(&build, build_path, out_path)) {
        return NULL;
    }

    BuildNode *node = malloc(sizeof(BuildNode));
    build_node_init(node);

    vec_foreach(&build.targets, target) {
        BuildTarget build_target;

        if (!build_graph_load_target(graph, &build_target, target)) {
            build_free(&build);
            build_node_free(node);
            free(node);

            return NULL;
        }

        vec_push(&node->targets, build_target);
    }

    build_free(&build);

    vec_push(&graph->nodes, node);

    return node;
}

static bool fetch_dep(const char *url, const char *path) {
    Vec(const char *) args;
    vec_init(&args);

    vec_push(&args, "git");
    vec_push(&args, "clone");
    vec_push(&args, url);
    vec_push(&args, path);

    char *cmd = vec_join(&args, " ");
    vec_free(&args);

    bool success = system(cmd) == 0;
    free(cmd);

    if (!success) {
        printf("Error: Could not fetch dep %s\n", url);
    }

    return success;
}

bool build_graph_load_deps(BuildGraph *graph, BuildNode *node) {
    vec_foreach(&node->targets, target) {
        vec_foreach(&target.deps, dep) {
            // if dep is already loaded, skip
            if (dep->node) {
                continue;
            }

            char path[256];
            snprintf(path, sizeof(path), ".lute/deps/%s", dep->id);

            // if dep is not fetched, fetch it
            if (!is_dir(path)) {
                if (!fetch_dep(dep->url, path)) {
                    return false;
                }
            }

            char bpath[256];
            snprintf(bpath, sizeof(bpath), "%s/build.c", path);

            char opath[256];
            snprintf(opath, sizeof(opath), ".lute/build/%s", dep->id);

            // load dep node
            BuildNode *dep_node = build_graph_load_node(graph, bpath, opath);

            // if dep node could not be loaded, return false
            if (!dep_node) {
                return false;
            }

            // set dep node
            dep->node = dep_node;

            BuildTarget *dep_target = build_node_target(dep_node, dep->name);

            // if dep target could not be found, return false
            if (!dep_target) {
                printf("Error: Dependency %s does not have target %s\n",
                       dep->url, dep->name);
                return false;
            }

            // set dep target
            dep->target = dep_target;

            // load dep node's deps
            if (!build_graph_load_deps(graph, dep_node)) {
                return false;
            }
        }
    }

    return true;
}

bool build_graph_load(BuildGraph *graph) {
    if (!make_dirs("out")) {
        return false;
    }

    if (!make_dirs(".lute/deps")) {
        return false;
    }

    if (!make_dirs(".lute/build")) {
        return false;
    }

    build_graph_init(graph);

    graph->root = build_graph_load_node(graph, "build.c", ".lute/build/build");

    if (!graph->root) {
        build_graph_free(graph);
        return false;
    }

    if (!build_graph_load_deps(graph, graph->root)) {
        build_graph_free(graph);
        return false;
    }

    return true;
}

// This file is part of Lute.
// Copyright (C) 2024  Hjalte C. Nannestad
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
