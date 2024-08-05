// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <lute/build.h>
#include <lute/serialize.h>

void build_init(Build *build) {
    // initialize the build

    vec_init(&build->targets);
}

void build_free(Build *build) {
    vec_foreach(&build->targets, target) {
        target_free(target);
        free(target);
    }

    vec_free(&build->targets);
}

Target *build_push_target(Build *build, Target target) {
    Target *new_target = malloc(sizeof(Target));
    *new_target = target;
    vec_push(&build->targets, new_target);
    return new_target;
}

void serialize_build(const Build *build, FILE *file) {
    serialize_data(&build->targets.len, file);
    vec_foreach(&build->targets, target) serialize_target(target, file);
}

bool deserialize_build(Build *build, FILE *file) {
    *build = (Build){0};

    bool success = deserialize_targets(&build->targets, file);

    if (!success) {
        build_free(build);
    }

    return success;
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
