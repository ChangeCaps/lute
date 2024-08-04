// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdio.h>

#include "build.h"
#include "target.h"
#include "util.h"

int build_command(Build *build, int argc, char **argv) {
    char *target_name = NULL;

    if (argc >= 3) {
        target_name = argv[2];
    }

    Target *target = select_target(build, target_name);

    if (!target) {
        printf("Error: Could not find target\n\n");
        return build_help_command(build, argc, argv);
    }

    return build_target(target);
}

int build_help_command(Build *build, int argc, char **argv) {
    (void)build;
    (void)argc;
    (void)argv;

    printf("Usage: lute build [target]\n");
    return 1;
}

bool build_target(Target *target) {
    if (!make_dir(target->outdir)) {
        printf("Error: Could not create target directory\n");
        return false;
    }

    if (target_is(target, BINARY))
        if (build_binary(target))
            return false;

    if (target_is(target, STATIC))
        if (build_static(target))
            return false;

    if (target_is(target, SHARED))
        if (build_shared(target))
            return false;

    return true;
}

bool build_binary(Target *target) {
    if (!target_is(target, BINARY)) {
        printf("Error: Target '%s' is not a binary\n", target->name);

        return 1;
    }

    printf("Building target[binary]: %s\n", target->name);

    char output[256];
    target_binary(target, output, sizeof(output));

    Args args;
    vec_init(&args);
    push_binary_args(&args, target, output);

    if (!execute(&args)) {
        vec_free(&args);
        return false;
    }

    vec_free(&args);

    return true;
}

bool build_static(Target *target) {
    printf("Building target[static]: %s\n", target->name);

    char output[256];
    target_static(target, output, sizeof(output));

    Args args;
    vec_init(&args);
    push_static_args(&args, target, output);

    if (!execute(&args)) {
        vec_free(&args);
        return false;
    }

    vec_free(&args);

    return true;
}

bool build_shared(Target *target) {
    printf("Building target[shared]: %s\n", target->name);

    char output[256];
    target_shared(target, output, sizeof(output));

    Args args;
    vec_init(&args);
    push_shared_args(&args, target, output);

    if (!execute(&args)) {
        vec_free(&args);
        return false;
    }

    vec_free(&args);

    return true;
}

void push_binary_args(Args *args, Target *target, const char *path) {
    vec_push(args, target_compiler(target));

    push_source_args(args, target);
    push_include_args(args, target);

    vec_foreach(&target->packages, package) {
        vec_push(args, package->cflags);
        vec_push(args, package->libs);
    }

    vec_push(args, "-o");
    vec_push(args, path);
    push_warning_args(args, target);
}

void push_static_args(Args *args, Target *target, const char *path) {
    vec_push(args, target_compiler(target));

    push_source_args(args, target);
    push_include_args(args, target);

    vec_foreach(&target->packages, package) {
        vec_push(args, package->cflags);
        vec_push(args, package->libs);
    }

    vec_push(args, "-c");
    vec_push(args, "-o");
    vec_push(args, path);
    push_warning_args(args, target);
}

void push_shared_args(Args *args, Target *target, const char *path) {
    vec_push(args, target_compiler(target));

    push_source_args(args, target);
    push_include_args(args, target);

    vec_foreach(&target->packages, package) {
        vec_push(args, package->cflags);
        vec_push(args, package->libs);
    }

    vec_push(args, "-shared");
    vec_push(args, "-o");
    vec_push(args, path);
    push_warning_args(args, target);
}

void push_source_args(Args *args, Target *target) {
    vec_foreach(&target->sources, source) {
        if (source->kind != SOURCE)
            continue;

        vec_push(args, source->path);
    }
}

void push_include_args(Args *args, Target *target) {
    vec_foreach(&target->includes, include) {
        vec_push(args, "-I");
        vec_push(args, include->path);
    }
}

void push_warning_args(Args *args, Target *target) {
    if (target->warn & Wall) {
        vec_push(args, "-Wall");
    }

    if (target->warn & Wextra) {
        vec_push(args, "-Wextra");
    }

    if (target->warn & Werror) {
        vec_push(args, "-Werror");
    }
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
