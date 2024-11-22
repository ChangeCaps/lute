// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <lute/target.h>
#include <lute/vector.h>

#include "hash.h"

typedef struct BuildPackage {
    char *name;
    char *cflags;
    char *libs;
    char *links;
} BuildPackage;

bool build_package_init(BuildPackage *package, const char *name);
void build_package_free(BuildPackage *package);

typedef Vec(char *) Paths;

typedef struct BuildDep BuildDep;
typedef struct BuildTarget {
    char *name;
    Output output;
    WarnFlags warn;
    Language lang;
    Standard std;

    Paths sources;
    Paths includes;
    Vec(BuildPackage *) packages;
    Vec(BuildDep *) deps;
} BuildTarget;

void build_target_free(BuildTarget *target);

typedef struct BuildNode {
    Vec(BuildTarget) targets;
} BuildNode;

void build_node_init(BuildNode *node);
void build_node_free(BuildNode *node);

BuildTarget *build_node_target(BuildNode *node, const char *name);

typedef struct BuildDep {
    char *url;
    char *name;
    HashId id;

    BuildNode *node;
    BuildTarget *target;
} BuildDep;

void build_dep_init(BuildDep *dep, const char *url, const char *name);
void build_dep_free(BuildDep *dep);

typedef struct BuildGraph {
    Vec(BuildPackage *) packages;
    Vec(BuildDep *) deps;
    Vec(BuildNode *) nodes;

    Paths paths;

    BuildNode *root;
} BuildGraph;

void build_graph_init(BuildGraph *graph);
void build_graph_free(BuildGraph *graph);

bool build_graph_load(BuildGraph *graph);

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
