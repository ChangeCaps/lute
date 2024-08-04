// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

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
    // if the path already exists, return it
    vec_foreach(&build->paths, build_path) {
        if (strcmp(build_path->path, path) == 0)
            return build_path;
    }

    FileKind kind = SOURCE;

    struct stat st;
    stat(path, &st);

    if (S_ISDIR(st.st_mode)) {
        kind = DIRECTORY;
    } else {
        char *ext = strrchr(path, '.');

        if (strcmp(ext, ".c") == 0 || strcmp(ext, ".cpp") == 0)
            kind = SOURCE;
        else if (strcmp(ext, ".h") == 0 || strcmp(ext, ".hpp") == 0)
            kind = HEADER;
        else
            return NULL;
    }

    Path new_path;
    new_path.path = strdup(path);
    new_path.kind = kind;
    return build_push_path(build, new_path);
}

void build_add_paths(Build *build, const char *path, Paths *paths) {
    DIR *dir = opendir(path);

    if (dir == NULL) {
        Path *new_path = build_add_path(build, path);

        if (new_path)
            vec_push(paths, new_path);

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
