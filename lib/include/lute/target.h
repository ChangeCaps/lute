// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "vector.h"

typedef enum Output {
    BINARY = 1 << 0,
    STATIC = 1 << 1,
    SHARED = 1 << 2,
    HEADER = 1 << 3,

    LIBRARY = STATIC | SHARED,
} Output;

typedef enum WarnFlag {
    Wall = 1 << 0,
    Wextra = 1 << 1,
    Werror = 1 << 2,
} WarnFlag;

typedef enum Language {
    C = 0,
    CPP = 1,
} Language;

typedef struct Dep {
    char *url;
    char *target;
} Dep;

typedef Vec(Dep) Deps;

void dep_init(Dep *dep, const char *name, const char *target);
void dep_free(Dep *dep);

void serialize_dep(const Dep *dep, FILE *file);
bool deserialize_dep(Dep *dep, FILE *file);

void deps_free(Deps *deps);

bool deserialize_deps(Deps *deps, FILE *file);

typedef Vec(char *) Strings;

typedef struct Target {
    char *name;
    Output output;
    WarnFlag warn;
    Language lang;

    Strings sources;
    Strings includes;
    Strings packages;
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
