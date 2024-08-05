// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include "run.h"
#include "args.h"
#include "build.h"
#include "graph.h"

int run_command(int argc, char **argv) {
    BuildGraph graph;

    if (!build_graph_load(&graph)) {
        return 1;
    }

    BuildTarget *target = NULL;

    if (argc > 2 && is_valid_target_name(argv[2])) {
        vec_foreachat(&graph.root->targets, t) {
            if (strcmp(t->name, argv[2]) == 0)
                target = t;
        }

        if (!target) {
            printf("Error: Target %s not found\n", argv[2]);
            return 1;
        }
    }

    if (!target && graph.root->targets.len != 1) {
        printf("Error: No target specified and no default target found\n");
        return 1;
    } else if (!target) {
        target = graph.root->targets.data;
    }

    if (!(target->output & BINARY)) {
        printf("Error: Target %s is not a binary target\n", target->name);
        return 1;
    }

    char outdir[256];
    snprintf(outdir, sizeof(outdir), "out/%s", target->name);

    if (!build_target(target, BINARY, outdir)) {
        printf("Build of target %s failed, exiting\n", target->name);
        return 1;
    }

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "./%s/%s", outdir, target->name);

    Args args = args_new();
    args_push(&args, cmd);

    bool found = false;
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--") == 0) {
            found = true;
            continue;
        } else if (found) {
            args_push(&args, argv[i]);
        }
    }

    bool success = args_exec(&args) == 0;
    args_free(&args);

    if (!success) {
        printf("Error: Could not run target %s\n", target->name);
        return 1;
    }

    return 0;
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
