// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include "package.h"
#include "path.h"
#include "target.h"

typedef struct Build {
    char *cc;
    char *cxx;

    Targets targets;
    Paths paths;
    Packages packages;
} Build;

void build_init(Build *build);
void build_free(Build *build);

Target *build_push_target(Build *build, Target target);
Path *build_push_path(Build *build, Path source);
Package *build_push_package(Build *build, Package package);
Package *build_add_package(Build *build, const char *name);

/// Load a source file into the build.
Path *build_add_path(Build *build, const char *path);

/// Load source files into the build.
void build_add_paths(Build *build, const char *path, Paths *paths);

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
