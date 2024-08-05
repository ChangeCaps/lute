// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <lute/serialize.h>

void serialize_str(const char *string, FILE *file) {
    if (!string) {
        int32_t null = -1;
        serialize_data(&null, file);
        return;
    }

    int32_t len = strlen(string);
    serialize_data(&len, file);
    fwrite(string, sizeof(char), len, file);
}

bool deserialize_str(char **string, FILE *file) {
    int32_t len;
    if (!deserialize_data(&len, file)) {
        return false;
    }

    if (len == -1) {
        *string = NULL;
        return true;
    }

    *string = malloc(len + 1);
    if (!*string) {
        return false;
    }

    if (fread(*string, sizeof(char), len, file) != (size_t)len) {
        free(*string);
        return false;
    }

    (*string)[len] = '\0';

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
