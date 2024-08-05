// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include "build.h"

Target *target(Build *build, const char *name, Output kind);
void source(Target *target, const char *path);
void include(Target *target, const char *path);
void package(Target *target, const char *name);
void depend(Target *target, const char *uri, const char *deptrg);

int __lute_main(void (*build)(Build *), int argc, char **argv);

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
