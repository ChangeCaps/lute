// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stdbool.h>
#include <time.h>

bool file_exists(const char *path);
bool is_dir(const char *path);
bool make_dir(const char *path);
bool make_dirs(const char *path);
bool read_file(const char *path, char **data);
bool copy_file(const char *src, const char *dst);
bool copy_files(const char *src, const char *dst);
bool remove_dir(const char *path);
bool last_modified(const char *path, time_t *time);

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
