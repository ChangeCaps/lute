// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <lute/lute.h>

#include "argp.h"
#include "build.h"
#include "clean.h"
#include "list.h"
#include "run.h"

void print_lute_usage() {
    printf("Usage: lute [options] [command]\n"
           "\n"
           "Options:\n"
           "  -h, --help    Show this help message\n"
           "\n"
           "Commands:\n"
           "  run, r        Build and run a target\n"
           "  build, b      Build a target\n"
           "  clean         Clean build artifacts\n"
           "  list          List available targets\n"
           "  help          Show this help message\n");
}

void print_lute_help() {
    printf("Lute - A simple build system for C/C++ projects\n\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("No command specified\n\n");
        print_lute_usage();
        return 1;
    }

    int argi = 1;

    while (argi < argc) {
        char *arg = argv[argi++];

        if (arg_is(arg, "r", "run")) {
            return run_command(argc, argv, &argi);
        } else if (arg_is(arg, "b", "build")) {
            return build_command(argc, argv, &argi);
        } else if (arg_is(arg, NULL, "clean")) {
            return clean_command(argc, argv, &argi);
        } else if (arg_is(arg, NULL, "list")) {
            return list_command(argc, argv, &argi);
        } else if (arg_is(arg, NULL, "help") || arg_is(arg, "-h", "--help")) {
            print_lute_help();
        } else {
            printf("Unknown argument: %s\n\n", arg);
            print_lute_usage();
        }
    }

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
