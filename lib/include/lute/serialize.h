// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stdbool.h>
#include <stdio.h>

#define serialize_data(data, file) fwrite(data, sizeof(*(data)), 1, file)
#define deserialize_data(data, file) fread(data, sizeof(*(data)), 1, file)

void serialize_str(const char *string, FILE *file);
bool deserialize_str(char **string, FILE *file);

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
