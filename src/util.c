// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

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
