// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <lute/target.h>
#include <stdbool.h>

typedef struct {
    bool help;
    Language lang;
} InitOptions;

InitOptions init_options_default();
bool init_options_parse(InitOptions *options, int argc, char **argv, int *argi);

void print_init_usage();
void print_init_help();

int init_command(int argc, char **argv, int *argi);

bool write_build_file(InitOptions options, const char *name);
bool write_gitignore_file();

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
