// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define __vec_concat(a, b) __CONCAT(a, b)
#define __vec_i __vec_concat(__i, __LINE__)

#define Vec(type)                                                              \
    struct {                                                                   \
        type *data;                                                            \
        size_t len;                                                            \
        size_t cap;                                                            \
    }

#define vec_init(vec)                                                          \
    do {                                                                       \
        (vec)->data = NULL;                                                    \
        (vec)->len = 0;                                                        \
        (vec)->cap = 0;                                                        \
    } while (0)

#define vec_free(vec)                                                          \
    do {                                                                       \
        free((vec)->data);                                                     \
        (vec)->data = NULL;                                                    \
        (vec)->len = 0;                                                        \
        (vec)->cap = 0;                                                        \
    } while (0)

#define __vec_elem_size(vec) sizeof(struct { typeof(*(vec)->data) data; })
#define vec_grow(vec)                                                          \
    do {                                                                       \
        (vec)->cap = (vec)->cap * 2 + 1;                                       \
        (vec)->data = realloc((vec)->data, __vec_elem_size(vec) * (vec)->cap); \
    } while (0)

#define vec_push(vec, elem)                                                    \
    do {                                                                       \
        if ((vec)->len == (vec)->cap) {                                        \
            vec_grow(vec);                                                     \
        }                                                                      \
        (vec)->data[(vec)->len++] = elem;                                      \
    } while (0)

#define vec_foreach(vec, elem)                                                 \
    size_t __vec_i = 0;                                                        \
    for (typeof(*(vec)->data) elem;                                            \
         __vec_i < (vec)->len && ((elem) = (vec)->data[__vec_i], 1);           \
         __vec_i++)

#define vec_foreachat(vec, elem)                                               \
    size_t __vec_i = 0;                                                        \
    for (typeof(*(vec)->data) *elem;                                           \
         __vec_i < (vec)->len && ((elem) = &(vec)->data[__vec_i], 1);          \
         __vec_i++)

#define vec_join(vec, sep) __vec_join((vec)->data, (vec)->len, sep)

inline char *__vec_join(const char **data, size_t len, const char *sep) {
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
