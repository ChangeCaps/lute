// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <dlfcn.h>
#include <stdio.h>

#include "command.h"
#include "load.h"

static bool get_lute_build_flags(char **cflags, char **libs) {
    const char *env_cflags = getenv("LUTE_CFLAGS");
    const char *env_libs = getenv("LUTE_LIBS");

    if (env_cflags && env_libs) {
        *cflags = strdup(env_cflags);
        *libs = strdup(env_libs);
        return true;
    }

    Package lute_package;
    package_init(&lute_package, "lute");

    *cflags = strdup(lute_package.cflags);
    *libs = strdup(lute_package.libs);

    free(lute_package.cflags);

    return true;
}

bool build_load(Build *build, const char *path) {
    Args args;
    vec_init(&args);

    char *cflags;
    char *libs;

    if (!get_lute_build_flags(&cflags, &libs))
        return false;

    vec_push(&args, "clang");
    vec_push(&args, "-shared");
    vec_push(&args, path);
    vec_push(&args, cflags);
    vec_push(&args, libs);
    vec_push(&args, "-o");
    vec_push(&args, "out/build.so");

    bool success = execute(&args);
    vec_free(&args);

    free(cflags);
    free(libs);

    if (!success)
        return false;

    void *handle = dlopen("out/build.so", RTLD_NOW);

    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return false;
    }

    void (*build_fn)(Build *) = dlsym(handle, "build");

    if (!build_fn) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return false;
    }

    build_fn(build);
    dlclose(handle);

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
