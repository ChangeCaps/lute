// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include "lute/lute.h"

Target *target(Build *build, const char *name, Output kind) {
    Target target;
    target_init(&target, name, kind, build);
    return build_push_target(build, target);
}

void source(Target *target, const char *path) {
    build_add_paths(target->build, path, &target->sources);
    target_infer_lang(target);
}

void include(Target *target, const char *path) {
    Path *include_path = build_add_path(target->build, path);
    vec_push(&target->includes, include_path);
}

void package(Target *target, const char *name) {
    Package *package = build_add_package(target->build, name);
    vec_push(&target->packages, package);
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
