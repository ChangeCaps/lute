// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include "list.h"
#include "graph.h"

int list_command(int argc, char **argv, int *argi) {
    (void)argc;
    (void)argv;
    (void)argi;

    BuildGraph graph;

    if (!build_graph_load(&graph)) {
        return 1;
    }

    printf("Targets:\n");

    vec_foreach(&graph.root->targets, target) {
        printf("  %s:\n", target.name);

        printf("    Output:");

        if (target.output & BINARY)
            printf(" binary");
        if (target.output & STATIC)
            printf(" static");
        if (target.output & SHARED)
            printf(" shared");
        if (target.output & HEADER)
            printf(" header");

        printf("\n");

        printf("    Language:");

        switch (target.lang) {
        case C:
            printf(" C");
            break;
        case CPP:
            printf(" C++");
            break;
        }

        printf("\n");

        printf("    Sources:\n");
        vec_foreach(&target.sources, source) {
            //
            printf("      %s\n", source);
        }

        printf("    Includes:\n");
        vec_foreach(&target.includes, include) {
            //
            printf("      %s\n", include);
        }

        printf("    Packages:\n");
        vec_foreach(&target.packages, package) {
            //
            printf("      %s\n", package->name);
        }

        printf("    Depends:\n");
        vec_foreach(&target.deps, dep) {
            //
            printf("      %s\n", dep->url);
        }
    }

    build_graph_free(&graph);

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
