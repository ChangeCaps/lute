#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "lute/build.h"

void build_init(Build *build) {
    build->cc = getenv("CC");
    build->cxx = getenv("CXX");

    if (!build->cc)
        build->cc = "clang";

    if (!build->cxx)
        build->cxx = "clang++";

    vec_init(&build->targets);
    vec_init(&build->paths);
    vec_init(&build->packages);
}

void build_free(Build *build) {
    vec_foreach(&build->targets, target) {
        target_free(target);
        free(target);
    }

    vec_foreach(&build->paths, path) {
        path_free(path);
        free(path);
    }

    vec_foreach(&build->packages, package) {
        package_free(package);
        free(package);
    }

    vec_free(&build->targets);
    vec_free(&build->paths);
    vec_free(&build->packages);
}

Target *build_push_target(Build *build, Target target) {
    Target *new_target = malloc(sizeof(Target));
    *new_target = target;
    vec_push(&build->targets, new_target);
    return new_target;
}

Path *build_push_path(Build *build, Path source) {
    Path *new_source = malloc(sizeof(Path));
    *new_source = source;
    vec_push(&build->paths, new_source);
    return new_source;
}

Package *build_push_package(Build *build, Package package) {
    Package *new_package = malloc(sizeof(Package));
    *new_package = package;
    vec_push(&build->packages, new_package);
    return new_package;
}

Package *build_add_package(Build *build, const char *name) {
    vec_foreach(&build->packages, pkg) {
        if (strcmp(pkg->name, name) == 0) {
            return pkg;
        }
    }

    Package package;
    package_init(&package, name);
    return build_push_package(build, package);
}

Path *build_add_path(Build *build, const char *path) {
    FileKind kind = SOURCE;

    struct stat st;
    stat(path, &st);

    if (S_ISDIR(st.st_mode)) {
        kind = DIRECTORY;
    } else {
        char *ext = strrchr(path, '.');

        if (strcmp(ext, ".h") == 0 || strcmp(ext, ".hpp") == 0) {
            kind = HEADER;
        }
    }

    Path new_path;
    new_path.path = strdup(path);
    new_path.kind = kind;
    return build_push_path(build, new_path);
}

void build_add_paths(Build *build, const char *path, Paths *paths) {
    DIR *dir = opendir(path);

    if (dir == NULL) {
        vec_push(paths, build_add_path(build, path));
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char *full_path = malloc(strlen(path) + strlen(entry->d_name) + 2);

        if (path[strlen(path) - 1] == '/')
            sprintf(full_path, "%s%s", path, entry->d_name);
        else
            sprintf(full_path, "%s/%s", path, entry->d_name);

        build_add_paths(build, full_path, paths);

        free(full_path);
    }

    closedir(dir);
}
