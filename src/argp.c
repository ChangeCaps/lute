// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <string.h>

#include "argp.h"

bool arg_is(const char *arg, const char *short_name, const char *long_name) {
    bool is_short = short_name && strcmp(arg, short_name) == 0;
    bool is_long = long_name && strcmp(arg, long_name) == 0;

    return is_short || is_long;
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
