// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdio.h>

#include "argp.h"
#include "clean.h"
#include "util.h"

void print_clean_usage() {
    printf("Usage: lute clean [options]\n"
           "\n"
           "Options:\n"
           "  -h, --help        Show this help message\n");
}

void print_clean_help() {
    printf("Clean build artifacts\n\n");
    print_clean_usage();
}

CleanOptions clean_options_default() {
    CleanOptions options = {0};

    options.help = false;

    return options;
}

bool clean_options_parse(CleanOptions *options, int argc, char **argv,
                         int *argi) {

    while (*argi < argc) {
        char *arg = argv[(*argi)++];

        if (arg_is(arg, "-h", "--help")) {
            options->help = true;
        } else {
            printf("Unknown option: %s\n", arg);
            return false;
        }
    }

    return true;
}

int clean_command(int argc, char **argv, int *argi) {
    CleanOptions options = clean_options_default();

    if (!clean_options_parse(&options, argc, argv, argi)) {
        printf("\n");
        print_clean_usage();
        return 1;
    }

    if (options.help) {
        print_clean_help();
        return 0;
    }

    printf("Cleaning build artifacts\n");

    if (file_exists("lute-out")) {
        printf("Removing lute-out\n");
        remove_dir("lute-out");
    }

    if (file_exists("lute-cache")) {
        printf("Removing lute-cache\n");
        remove_dir("lute-cache");
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
