// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include "graph.h"

typedef enum {
    DEBUG,
    RELEASE,
} Profile;

typedef struct {
    bool help;
    Profile profile;
} BuildOptions;

BuildOptions build_options_default();
bool build_options_parse(BuildOptions *options, int argc, char **argv,
                         int *index);

int build_command(int argc, char **argv);

void build_help_command();
void help_build_options();

bool build_target(const BuildOptions *options, const BuildTarget *target,
                  Output output, const char *outdir);
bool build_objects(const BuildOptions *options, const BuildTarget *target,
                   const char *output);

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
