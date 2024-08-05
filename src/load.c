// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load.h"
#include "util.h"

static bool get_lute_build_flags(char **cflags, char **libs) {
    const char *env_cflags = getenv("LUTE_CFLAGS");
    const char *env_libs = getenv("LUTE_LIBS");

    if (env_cflags && env_libs) {
        *cflags = strdup(env_cflags);
        *libs = strdup(env_libs);
        return true;
    } else {
        *cflags = strdup("-Ilib/include");
        *libs = strdup("out/lib/lutebuild.o");
        return true;
    }

    return false;
}

static bool compile_build(const char *build_path, const char *out_path) {
    Vec(const char *) args;
    vec_init(&args);

    char *cflags;
    char *libs;

    if (!get_lute_build_flags(&cflags, &libs)) {
        printf("Error: LUTE_CFLAGS and LUTE_LIBS not set\n");
        return false;
    }

    vec_push(&args, "clang");
    vec_push(&args, "-o");
    vec_push(&args, out_path);
    vec_push(&args, cflags);
    vec_push(&args, libs);
    vec_push(&args, build_path);

    char *cmd = vec_join(&args, " ");
    vec_free(&args);

    if (system(cmd) != 0) {
        printf("Error: Could not run clang\n");
        printf("Command: %s\n", cmd);

        free(cflags);
        free(libs);
        free(cmd);

        return false;
    }

    free(cflags);
    free(libs);
    free(cmd);

    return true;
}

bool load_build(Build *build, const char *bpath, const char *opath) {
    if (!file_exists(bpath)) {
        printf("Error: Build file does not exist %s\n", bpath);
        return false;
    }

    if (!compile_build(bpath, opath)) {
        return false;
    }

    char *bdir = strdup(bpath);

    if (strrchr(bdir, '/')) {
        *strrchr(bdir, '/') = '\0';
    } else if (strrchr(bdir, '\\')) {
        *strrchr(bdir, '\\') = '\0';
    } else {
        bdir[0] = '.';
        bdir[1] = '\0';
    }

    char *rpath = realpath(opath, NULL);

    Vec(const char *) args;
    vec_init(&args);

    vec_push(&args, "cd");
    vec_push(&args, bdir);
    vec_push(&args, "&&");
    vec_push(&args, rpath);

    char *cmd = vec_join(&args, " ");
    vec_free(&args);
    free(bdir);
    free(rpath);

    FILE *pipe = popen(cmd, "r");
    free(cmd);

    if (!pipe) {
        printf("Error: Could not run build\n");

        return false;
    }

    if (!deserialize_build(build, pipe)) {
        printf("Error: Could not deserialize build\n");

        return false;
    }

    pclose(pipe);

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
