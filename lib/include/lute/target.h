// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "vector.h"

// Outputs of a target.
typedef enum Outputs {
    // A binary target.
    BINARY = 1 << 0,

    // A static library target.
    STATIC = 1 << 1,

    // A shared library target.
    SHARED = 1 << 2,

    // A header-only library target.
    HEADER = 1 << 3,

    // A library target, shorthand for `STATIC | SHARED`.
    LIBRARY = STATIC | SHARED,
} Output;

// Warning flags of a target.
typedef enum Warnings {
    // Enable all warnings.
    Wall = 1 << 0,

    // Enable extra warnings.
    Wextra = 1 << 1,

    // Enable warnings as errors.
    Werror = 1 << 2,
} WarnFlags;

// Language of a target.
typedef enum Language {
    // The C language.
    C = 0,
    // The C++ language.
    CXX = 1,
} Language;

// Standard versions.
typedef enum Standard {
    // The C89 standard.
    C89 = 1,

    // The C90 standard.
    C90 = 2,

    // The C95 standard.
    C95 = 3,

    // The C99 standard.
    C99 = 4,

    // The C11 standard.
    C11 = 5,

    // The C17 standard.
    C17 = 6,

    // The C23 standard.
    C23 = 7,

    // The C++98 standard.
    CXX98 = 1024,

    // The C++03 standard.
    CXX03 = 1025,

    // The C++11 standard.
    CXX11 = 1026,

    // The C++14 standard.
    CXX14 = 1027,

    // The C++17 standard.
    CXX17 = 1028,

    // The C++20 standard.
    CXX20 = 1029,

    // The C++23 standard.
    CXX23 = 1030,

    // The C++26 standard.
    CXX26 = 1031,
} Standard;

const char *standard_name(Standard std);

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
    WarnFlags warn;

    // The language of the target.
    //
    // Can be either `C` or `CXX`.
    Language lang;

    // The standard to use.
    //
    // Example `C11` or `CXX17`.
    Standard std;

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

void targets_free(Targets *targets);

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
