// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <lute/lute.h>

Target *target(Build *b, const char *name, Output kind) {
    Target target;
    target_init(&target, name, kind);
    return build_push_target(b, target);
}

void source(Target *t, const char *path) {
    char *source = strdup(path);
    vec_push(&t->sources, source);
}

void include(Target *t, const char *path) {
    char *include = strdup(path);
    vec_push(&t->includes, include);
}

void package(Target *t, const char *name) {
    char *package = strdup(name);
    vec_push(&t->packages, package);
}

void depend(Target *t, const char *uri, const char *target) {
    Dep dep;
    dep_init(&dep, uri, target);
    vec_push(&t->deps, dep);
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
