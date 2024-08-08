// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

bool read_file(const char *path, char **data) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return false;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *data = malloc(size + 1);
    if (!*data) {
        fclose(file);
        return false;
    }

    if (fread(*data, sizeof(char), size, file) != size) {
        fclose(file);
        free(*data);
        return false;
    }

    (*data)[size] = '\0';

    fclose(file);

    return true;
}

bool copy_file(const char *src, const char *dst) {
    char cmd[512];
    sprintf(cmd, "cp %s %s", src, dst);
    return system(cmd) == 0;
}

bool copy_files(const char *src, const char *dst) {
    char cmd[512];
    sprintf(cmd, "cp -r %s/* %s", src, dst);
    return system(cmd) == 0;
}

bool remove_dir(const char *path) {
    if (!is_dir(path)) {
        remove(path);
        return true;
    }

    DIR *dir = opendir(path);

    if (!dir) {
        return false;
    }

    struct dirent *entry;

    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char *full = malloc(strlen(path) + strlen(entry->d_name) + 2);
        sprintf(full, "%s/%s", path, entry->d_name);

        if (is_dir(full)) {
            remove_dir(full);
        } else {
            remove(full);
        }

        free(full);
    }

    closedir(dir);

    return rmdir(path) == 0;
}

bool last_modified(const char *path, time_t *time) {
    struct stat st = {0};

    if (stat(path, &st) == 0) {
        *time = st.st_mtime;
        return true;
    }

    return false;
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
