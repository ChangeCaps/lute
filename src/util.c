
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "util.h"

bool file_exists(const char *path) {
    struct stat st = {0};

    return stat(path, &st) == 0;
}

bool is_dir(const char *path) {
    struct stat st = {0};

    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }

    return false;
}

bool make_dir(const char *path) {
    if (is_dir(path)) {
        return true;
    }

    return mkdir(path, 0755) == 0;
}

bool make_dirs(const char *path) {
    char *copy = strdup(path);
    char *ptr = copy;

    while ((ptr = strchr(ptr + 1, '/'))) {
        *ptr = '\0';

        if (!make_dir(copy)) {
            free(copy);
            return false;
        }

        *ptr = '/';
    }

    free(copy);

    return make_dir(path);
}
