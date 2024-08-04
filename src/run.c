// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdio.h>

#include "build.h"
#include "run.h"
#include "target.h"

int run_command(Build *build, int argc, char **argv) {
    char *target_name = NULL;

    if (argc >= 3) {
        target_name = argv[2];
    }

    Target *target = select_target(build, target_name);

    if (!target) {
        printf("Error: Could not find target\n\n");
        return 1;
    }

    if (!build_binary(target)) {
        return 1;
    }

    char output[256];
    target_binary(target, output, sizeof(output));

    printf("Running target: %s\n", output);

    Args args;
    vec_init(&args);
    vec_push(&args, output);

    if (!execute(&args)) {
        vec_free(&args);
        return 1;
    }

    vec_free(&args);

    return 0;
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
