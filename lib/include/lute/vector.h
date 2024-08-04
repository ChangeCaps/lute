#pragma once

#include <stdlib.h>
#include <string.h>

#define vec(type)                                                              \
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

#define vec_free_with(vec, free_elem)                                          \
    do {                                                                       \
        for (size_t i = 0; i < (vec)->len; i++) {                              \
            free_elem((vec)->data[i]);                                         \
            free((vec)->data[i]);                                              \
        }                                                                      \
        vec_free(vec);                                                         \
    } while (0)

#define vec_grow(vec)                                                          \
    do {                                                                       \
        (vec)->cap = (vec)->cap * 2 + 1;                                       \
        (vec)->data = realloc((vec)->data, sizeof(*(vec)->data) * (vec)->cap); \
    } while (0)

#define vec_push(vec, elem)                                                    \
    do {                                                                       \
        if ((vec)->len == (vec)->cap) {                                        \
            vec_grow(vec);                                                     \
        }                                                                      \
        (vec)->data[(vec)->len++] = elem;                                      \
    } while (0)

#define vec_foreach(vec, elem)                                                 \
    typeof(*(vec)->data) elem;                                                 \
    for (size_t __i = 0; __i < (vec)->len && ((elem) = (vec)->data[__i], 1);   \
         __i++)

#define vec_join(vec, sep) __vec_join((vec)->data, (vec)->len, sep)
char *__vec_join(const char **data, size_t len, const char *sep);
