// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <string.h>

#include "argp.h"
#include "log.h"

bool arg_is(const char *arg, const char *short_name, const char *long_name) {
    bool is_short = short_name && strcmp(arg, short_name) == 0;
    bool is_long = long_name && strcmp(arg, long_name) == 0;

    return is_short || is_long;
}

BuildTarget *select_target(const BuildGraph *graph, int argc, char **argv,
                           int *argi) {

    BuildTarget *target = NULL;

    if (argc > 2 && is_valid_target_name(argv[2])) {
        vec_foreachat(&graph->root->targets, t) {
            if (strcmp(t->name, argv[2]) == 0)
                target = t;
        }

        (*argi)++;

        if (!target) {
            ERROR("Target %s not found\n", argv[2]);
            return NULL;
        }
    }

    if (!target && graph->root->targets.len != 1) {
        ERROR("No target specified and no default target found\n");
        return NULL;
    } else if (!target) {
        target = graph->root->targets.data;
    }

    return target;
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
