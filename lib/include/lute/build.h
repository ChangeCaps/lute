// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include "target.h"

// The primary data structure of Lute.
typedef struct Build {
    // The language of the build.
    //
    // This will be used as the default language for added targets.
    Language lang;

    // The warning flags of the build.
    //
    // This will be used as the default warning flags for added targets.
    WarnFlags warn;

    // The standard of the build.
    //
    // This will be used as the default standard for added targets.
    Standard std;

    // The targets of the build.
    Targets targets;
} Build;

// Initialize a build.
void build_init(Build *build);

// Free a build.
void build_free(Build *build);

// Add a target to a build.
Target *build_push_target(Build *build, Target target);

// Serialize a build to a file.
void serialize_build(const Build *build, FILE *file);

// Deserialize a build from a file.
bool deserialize_build(Build *build, FILE *file);

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
