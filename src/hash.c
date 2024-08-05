// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "hash.h"

void hash_string(HashId id, const char *prefix, const char *str) {
    assert(strlen(prefix) <= 6);

    uint64_t hash = 5381;

    for (size_t i = 0; i < strlen(str); i++) {
        hash = ((hash << 5) + hash) + str[i];
    }

    id[0] = '\0';
    strcat(id, prefix);
    strcat(id, "-");

    for (size_t i = 0; i < 16; i++) {
        char c = (hash >> (i * 4)) & 0xf;
        c = c < 10 ? '0' + c : 'a' + c - 10;
        strncat(id, &c, 1);
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
