// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include "build.h"

// Add a target to a build.
Target *target(Build *build, const char *name, Output kind);

// Add a source file to a target.
//
// If the path is a directory, all files in the directory are added recursively.
void source(Target *target, const char *path);

// Add an include path to a target.
void include(Target *target, const char *path);

// Add a pkg-config package to a target.
void package(Target *target, const char *name);

// Add a dependency to a target.
//
// The url is a git repository URL. The deptrg is the name of the target to
// depend on.
void depend(Target *target, const char *url, const char *deptrg);

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
