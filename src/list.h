// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stdbool.h>

typedef enum {
    LIST_OUTPUT_NONE = 0,
    LIST_OUTPUT_SOURCE = 1 << 0,
    LIST_OUTPUT_LANGUAGE = 1 << 1,

    LIST_OUTPUT_DEFAULT = LIST_OUTPUT_LANGUAGE,
    LIST_OUTPUT_ALL = LIST_OUTPUT_SOURCE | LIST_OUTPUT_LANGUAGE,
} ListOutput;

typedef struct {
    bool help;
    ListOutput output;
} ListOptions;

ListOptions list_options_default();
bool list_options_parse(ListOptions *options, int argc, char **argv, int *argi);

void print_list_usage();
void print_list_help();

int list_command(int argc, char **argv, int *argi);

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
