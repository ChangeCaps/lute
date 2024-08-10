// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include "run.h"
#include "argp.h"
#include "args.h"
#include "build.h"
#include "graph.h"

void print_run_usage() {
    printf("Usage: lute run [target] [options] [-- [args]]\n"
           "\n"
           "Options:\n");
    print_build_options();
}

void print_run_help() {
    printf("Build and run a target\n");
    printf("Version: %s\n\n", VERSION);
    print_run_usage();
}

int run_command(int argc, char **argv, int *argi) {
    BuildGraph graph;

    if (!build_graph_load(&graph)) {
        return 1;
    }

    BuildTarget *target = select_target(&graph, argc, argv, argi);

    if (!target) {
        return 1;
    }

    BuildOptions options = build_options_default();

    if (!build_options_parse(&options, argc, argv, argi)) {
        printf("\n");
        print_run_usage();
        return 1;
    }

    if (options.help) {
        print_run_help();
        return 0;
    }

    char outdir[256];
    snprintf(outdir, sizeof(outdir), "lute-out/%s/%s",
             profile_name(options.profile), target->name);

    if (!build_target(&options, target, BINARY, outdir)) {
        printf("Build of target %s failed, exiting\n", target->name);
        return 1;
    }

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "./%s/%s", outdir, target->name);

    Args args = args_new();
    args_push(&args, cmd);

    for (; *argi < argc; (*argi)++)
        args_push(&args, argv[*argi]);

    int status = args_exec(&args);
    args_free(&args);

    return status;
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
