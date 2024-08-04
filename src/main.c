// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <lute/lute.h>

#include "build.h"
#include "load.h"
#include "run.h"
#include "util.h"

void help_build() {
    //
    printf("Usage: lute build [target]\n");
}

int clean_command(Build *build) {
    printf("Cleaning targets\n");

    vec_foreach(&build->targets, target) {
        printf("  %s\n", target->name);
        remove(target->outdir);
    }

    return 0;
}

int list_command(Build *build) {
    printf("Targets:\n");

    vec_foreach(&build->targets, target) {
        printf("  %s\n", target->name);
        printf("    Output:");

        if (target_is(target, BINARY))
            printf(" Executable");
        if (target_is(target, STATIC))
            printf(" Static Library");
        if (target_is(target, SHARED))
            printf(" Shared Library");

        printf("\n");

        printf("    Language: %s\n", target->lang == C ? "C" : "C++");
        printf("    Output Directory: %s\n", target->outdir);
    }

    return 0;
}

void help_lute() {
    printf("Usage: lute [command]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  build\n");
    printf("  run\n");
    printf("  clean\n");
    printf("  list\n");
    printf("  help\n");
}

int lute(Build *build, int argc, char **argv) {
    if (argc < 2) {
        help_lute();
        return 1;
    }

    if (strcmp(argv[1], "build") == 0) {
        return build_command(build, argc, argv);
    } else if (strcmp(argv[1], "run") == 0) {
        return run_command(build, argc, argv);
    } else if (strcmp(argv[1], "clean") == 0) {
        return clean_command(build);
    } else if (strcmp(argv[1], "list") == 0) {
        return list_command(build);
    } else if (strcmp(argv[1], "help") == 0) {
        help_lute();
    } else {
        printf("Unknown command: %s\n", argv[1]);
        help_lute();
    }

    return 0;
}

int main(int argc, char **argv) {
    if (!make_dir("out")) {
        printf("Error: Could not create out directory\n");
        return 1;
    }

    Build build;
    build_init(&build);

    if (!build_load(&build, "build.c")) {
        build_free(&build);

        return 1;
    }

    int code = lute(&build, argc, argv);

    build_free(&build);

    return code;
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
