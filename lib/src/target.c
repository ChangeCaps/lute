// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lute/build.h"
#include "lute/target.h"

static bool is_alphabetic(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool is_numeric(char c) { return c >= '0' && c <= '9'; }

static bool is_alphanumeric(char c) {
    return is_alphabetic(c) || is_numeric(c);
}

bool is_valid_target_name(const char *name) {
    if (!is_alphabetic(name[0]))
        return false;

    for (size_t i = 0; i < strlen(name); i++) {
        char c = name[i];

        if (!is_alphanumeric(c) || c == '_' || c == '-')
            return false;
    }

    return true;
}

void target_init(Target *target, const char *name, Output kind, void *build) {
    if (!is_valid_target_name(name)) {
        printf("Error: Invalid target name: %s\n", name);
        exit(1);
    }

    target->name = strdup(name);
    target->output = kind;
    target->warn = 0;
    target->lang = C;

    target->outdir = malloc(strlen("out/") + strlen(name) + 1);
    sprintf(target->outdir, "out/%s", name);

    target->build = build;
    vec_init(&target->sources);
    vec_init(&target->includes);
    vec_init(&target->packages);
}

void target_free(Target *target) {
    free(target->name);

    vec_free(&target->sources);
    vec_free(&target->includes);
    vec_free(&target->packages);
}

bool target_is(const Target *target, Output kind) {
    return (target->output & kind) > 0;
}

void target_infer_lang(Target *target) {
    for (size_t i = 0; i < target->sources.len; i++) {
        Path *source = target->sources.data[i];

        const char *ext = strrchr(source->path, '.');

        if (strcmp(ext, ".cpp") == 0) {
            target->lang = CXX;
            return;
        }
    }
}

char *target_compiler(const Target *target) {
    switch (target->lang) {
    case C:
        return target->build->cc;
    case CXX:
        return target->build->cxx;
    default:
        return NULL;
    }
}

void target_binary(Target *target, char *path, size_t size) {
    snprintf(path, size, "%s/%s", target->outdir, target->name);
}

void target_static(Target *target, char *path, size_t size) {
    snprintf(path, size, "%s/%s.o", target->outdir, target->name);
}

void target_shared(Target *target, char *path, size_t size) {
    snprintf(path, size, "%s/lib%s.so", target->outdir, target->name);
}

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
