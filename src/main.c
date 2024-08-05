// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <lute/lute.h>

#include "build.h"
#include "clean.h"
#include "install.h"
#include "list.h"
#include "run.h"

void help_lute() {
    printf("Usage: lute [command]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  run\n");
    printf("  build\n");
    printf("  install\n");
    printf("  clean\n");
    printf("  list\n");
    printf("  help\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        help_lute();
        return 1;
    }
    if (strcmp(argv[1], "run") == 0) {
        return run_command(argc, argv);
    } else if (strcmp(argv[1], "build") == 0) {
        return build_command(argc, argv);
    } else if (strcmp(argv[1], "install") == 0) {
        return install_command(argc, argv);
    } else if (strcmp(argv[1], "clean") == 0) {
        return clean_command(argc, argv);
    } else if (strcmp(argv[1], "list") == 0) {
        return list_command(argc, argv);
    } else if (strcmp(argv[1], "help") == 0) {
        help_lute();
    } else {
        printf("Unknown command: %s\n", argv[1]);
        help_lute();
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
