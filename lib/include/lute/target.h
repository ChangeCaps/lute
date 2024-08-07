// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "vector.h"

// Outputs of a target.
typedef enum Output {
    // A binary target.
    BINARY = 1 << 0,

    // A static library target.
    STATIC = 1 << 1,

    // A shared library target.
    SHARED = 1 << 2,

    // A header-only library target.
    HEADER = 1 << 3,

    LIBRARY = STATIC | SHARED,
} Output;

// Warning flags.
typedef enum WarnFlag {
    // Enable all warnings.
    Wall = 1 << 0,

    // Enable extra warnings.
    Wextra = 1 << 1,

    // Enable warnings as errors.
    Werror = 1 << 2,
} WarnFlag;

// Languages of a target.
typedef enum Language {
    // The C language.
    C = 0,
    // The C++ language.
    CPP = 1,
} Language;

// A dependency.
typedef struct Dep {
    // The git repository URL of the dependency.
    char *url;

    // The name of the target to depend on.
    char *target;
} Dep;

// A list of dependencies.
typedef Vec(Dep) Deps;

// Initialize a dependency.
void dep_init(Dep *dep, const char *name, const char *target);

// Free a dependency.
void dep_free(Dep *dep);

// Serialize a dependency to a file.
void serialize_dep(const Dep *dep, FILE *file);

// Deserialize a dependency from a file.
bool deserialize_dep(Dep *dep, FILE *file);

// Free a list of dependencies.
void deps_free(Deps *deps);

// Deserialize a list of dependencies from a file.
bool deserialize_deps(Deps *deps, FILE *file);

// A list of strings.
typedef Vec(char *) Strings;

// A build target.
typedef struct Target {
    // The name of the target.
    char *name;

    // The output kind(s) of the target.
    //
    // Can be combined eg. `STATIC | SHARED`.
    Output output;

    // The warning flags to use.
    //
    // Example `Wall | Wextra`.
    WarnFlag warn;

    // The language of the target.
    //
    // Can be either `C` or `CPP`.
    Language lang;

    // The standard to use.
    //
    // Example `c11` or `c++17`.
    char *std;

    // The sources of the target.
    //
    // Do not interact with this directly.
    Strings sources;

    // The includes of the target.
    //
    // Do not interact with this directly.
    Strings includes;

    // The packages of the target.
    //
    // Do not interact with this directly.
    Strings packages;

    // The dependencies of the target.
    //
    // Do not interact with this directly.
    Deps deps;
} Target;

typedef Vec(Target *) Targets;

bool is_valid_target_name(const char *name);

bool target_init(Target *target, const char *name, Output kind);
void target_free(Target *target);

void serialize_target(const Target *target, FILE *file);
bool deserialize_target(Target *target, FILE *file);

void serialize_targets(const Targets *targets, FILE *file);
bool deserialize_targets(Targets *targets, FILE *file);

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
