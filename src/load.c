// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <dlfcn.h>
#include <stdio.h>

#include "command.h"
#include "load.h"

bool build_load(Build *build, const char *path) {
    Package lute_package;
    package_init(&lute_package, "lute");

    Args args;
    vec_init(&args);

    vec_push(&args, "clang");
    vec_push(&args, "-shared");
    vec_push(&args, path);
    vec_push(&args, lute_package.cflags);
    vec_push(&args, lute_package.libs);
    vec_push(&args, "-o");
    vec_push(&args, "out/build.so");
    vec_push(&args, "-I");
    vec_push(&args, "lib/include");

    bool success = execute(&args);
    vec_free(&args);

    package_free(&lute_package);

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
