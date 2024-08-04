// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <string.h>

#include "lute/vector.h"

char *__vec_join(const char **data, size_t len, const char *sep) {
    char *str = NULL;
    size_t new_len = 0;

    for (size_t i = 0; i < len; i++) {
        new_len += strlen(data[i]) + strlen(sep);
    }

    str = malloc(new_len + 1);
    str[0] = '\0';

    for (size_t i = 0; i < len; i++) {
        strcat(str, data[i]);

        if (i < len - 1) {
            strcat(str, sep);
        }
    }

    return str;
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
