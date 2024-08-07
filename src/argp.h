// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stdbool.h>

#include "graph.h"

// Check if an argument is a flag with the given short and long name.
//
// Short and long names can be NULL, in which case they are ignored.
bool arg_is(const char *arg, const char *short_name, const char *long_name);

// Select a target from the command line arguments.
//
// If a target is not found, NULL is returned.
BuildTarget *select_target(const BuildGraph *graph, int argc, char **argv,
                           int *argi);

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
