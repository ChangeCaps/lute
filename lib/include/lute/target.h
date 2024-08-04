// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stdbool.h>

#include "package.h"
#include "path.h"

typedef enum {
    BINARY = 1 << 0,
    STATIC = 1 << 1,
    SHARED = 1 << 2,
} Output;

typedef enum {
    Wall = 1 << 0,
    Wextra = 1 << 1,
    Werror = 1 << 2,
} WarnFlag;

typedef enum {
    C,
    CXX,
} Language;

typedef struct {
    char *name;
    Output output;
    WarnFlag warn;
    Language lang;

    char *outdir;

    struct Build *build;
    Paths sources;
    Paths includes;
    Packages packages;
} Target;

typedef Vec(Target *) Targets;

bool is_valid_target_name(const char *name);

void target_init(Target *target, const char *name, Output kind, void *build);
void target_free(Target *target);

bool target_is(const Target *target, Output kind);
void target_infer_lang(Target *target);
char *target_compiler(const Target *target);
void target_binary(Target *target, char *path, size_t maxlen);
void target_static(Target *target, char *path, size_t maxlen);
void target_shared(Target *target, char *path, size_t maxlen);

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
