#pragma once

#include <stdbool.h>

bool file_exists(const char *path);
bool is_dir(const char *path);
bool make_dir(const char *path);
bool make_dirs(const char *path);
