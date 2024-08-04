// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lute/package.h"

#define BUFFER_SIZE 4096

char *get_command_output(const char *command) {
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        printf("Failed to run command\n");
        exit(1);
    }

    char *result = malloc(BUFFER_SIZE);

    if (fgets(result, BUFFER_SIZE, pipe) == NULL) {
        free(result);
        pclose(pipe);
        exit(1);
    }

    // check the status of the command
    if (pclose(pipe) != 0) {
        free(result);
        exit(1);
    }

    // for whatever reason pkgconf adds a newline to the end of the output
    // this is a hacky way to remove it
    char *newline = strchr(result, '\n');

    if (newline)
        *newline = '\0';

    return result;
}

char *get_cflags(const char *name) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "pkg-config --cflags %s", name);
    return get_command_output(buffer);
}

char *get_libs(const char *name) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "pkg-config --libs %s", name);
    return get_command_output(buffer);
}

void package_init(Package *package, const char *name) {
    package->name = strdup(name);
    package->cflags = get_cflags(name);
    package->libs = get_libs(name);
}

void package_free(Package *package) {
    free(package->name);
    free(package->cflags);
    free(package->libs);
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
