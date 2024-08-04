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
