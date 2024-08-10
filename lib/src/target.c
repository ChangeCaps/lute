// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lute/serialize.h"
#include "lute/target.h"

void dep_init(Dep *dep, const char *name, const char *target) {
    dep->url = strdup(name);
    dep->target = strdup(target);
}

void dep_free(Dep *dep) {
    free(dep->url);
    free(dep->target);
}

void serialize_dep(const Dep *dep, FILE *file) {
    serialize_str(dep->url, file);
    serialize_str(dep->target, file);
}

bool deserialize_dep(Dep *dep, FILE *file) {
    *dep = (Dep){0};

    bool success =
        deserialize_str(&dep->url, file) && deserialize_str(&dep->target, file);

    if (!success) {
        dep_free(dep);
    }

    return success;
}

void deps_free(Deps *deps) {
    vec_foreachat(deps, dep) dep_free(dep);
    vec_free(deps);
}

bool deserialize_deps(Deps *deps, FILE *file) {
    *deps = (Deps){0};

    size_t len;
    if (!deserialize_data(&len, file)) {
        return false;
    }

    for (size_t i = 0; i < len; i++) {
        Dep dep;

        if (!deserialize_dep(&dep, file)) {
            deps_free(deps);
            return false;
        }

        vec_push(deps, dep);
    }

    return true;
}

const char *standard_name(Standard std) {
    switch (std) {
    case C89:
        return "c89";
    case C99:
        return "c99";
    case C11:
        return "c11";
    case C17:
        return "c17";
    case C23:
        return "c23";
    case CXX98:
        return "c++98";
    case CXX03:
        return "c++03";
    case CXX11:
        return "c++11";
    case CXX14:
        return "c++14";
    case CXX17:
        return "c++17";
    case CXX20:
        return "c++20";
    case CXX23:
        return "c++23";
    case CXX26:
        return "c++26";
    default:
        return NULL;
    }
}

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

        if (!(is_alphanumeric(c) || c == '_' || c == '-'))
            return false;
    }

    return true;
}

bool target_init(Target *target, const char *name, Output kind) {
    if (!is_valid_target_name(name)) {
        printf("Error: Invalid target name: %s\n", name);
        return false;
    }

    target->name = strdup(name);
    target->output = kind;
    target->warn = Wall | Wextra;
    target->lang = C;
    target->std = 0;

    vec_init(&target->sources);
    vec_init(&target->includes);
    vec_init(&target->packages);
    vec_init(&target->deps);

    return true;
}

void target_free(Target *target) {
    free(target->name);

    vec_foreach(&target->sources, source) free(source);
    vec_foreach(&target->includes, include) free(include);
    vec_foreach(&target->packages, package) free(package);
    vec_foreachat(&target->deps, dep) dep_free(dep);

    vec_free(&target->sources);
    vec_free(&target->includes);
    vec_free(&target->packages);
    vec_free(&target->deps);
}

void serialize_target(const Target *target, FILE *file) {
    serialize_str(target->name, file);
    serialize_data(&target->output, file);
    serialize_data(&target->warn, file);
    serialize_data(&target->lang, file);
    serialize_data(&target->std, file);

    serialize_data(&target->sources.len, file);
    vec_foreach(&target->sources, source) serialize_str(source, file);

    serialize_data(&target->includes.len, file);
    vec_foreach(&target->includes, include) serialize_str(include, file);

    serialize_data(&target->packages.len, file);
    vec_foreach(&target->packages, package) serialize_str(package, file);

    serialize_data(&target->deps.len, file);
    vec_foreachat(&target->deps, dep) serialize_dep(dep, file);
}

static bool deserialize_strings(Strings *strings, FILE *file) {
    size_t len;
    if (!deserialize_data(&len, file)) {
        return false;
    }

    for (size_t i = 0; i < len; i++) {
        char *string;
        if (!deserialize_str(&string, file)) {
            return false;
        }

        vec_push(strings, string);
    }

    return true;
}

bool deserialize_target(Target *target, FILE *file) {
    *target = (Target){0};

    bool success = deserialize_str(&target->name, file) &&
                   deserialize_data(&target->output, file) &&
                   deserialize_data(&target->warn, file) &&
                   deserialize_data(&target->lang, file) &&
                   deserialize_data(&target->std, file) &&
                   deserialize_strings(&target->sources, file) &&
                   deserialize_strings(&target->includes, file) &&
                   deserialize_strings(&target->packages, file) &&
                   deserialize_deps(&target->deps, file);

    if (!success) {
        target_free(target);
    }

    return success;
}

void targets_free(Targets *targets) {
    vec_foreach(targets, target) target_free(target);
    vec_free(targets);
}

void serialize_targets(const Targets *targets, FILE *file) {
    serialize_data(&targets->len, file);
    vec_foreach(targets, target) serialize_target(target, file);
}

bool deserialize_targets(Targets *targets, FILE *file) {
    *targets = (Targets){0};

    size_t len;
    if (!deserialize_data(&len, file)) {
        return false;
    }

    for (size_t i = 0; i < len; i++) {
        Target *target = malloc(sizeof(Target));
        if (!deserialize_target(target, file)) {
            targets_free(targets);
            return false;
        }

        vec_push(targets, target);
    }

    return true;
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
