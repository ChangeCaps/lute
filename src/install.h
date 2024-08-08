// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#pragma once

#include <stdbool.h>

#include "graph.h"

typedef struct {
    bool help;
    bool dry;
    bool build;
    bool nix;
    const char *bin_path;
    const char *lib_path;
    const char *include_path;
    const char *pkg_config_path;
} InstallOptions;

InstallOptions install_options_default();
bool install_options_parse(InstallOptions *options, int argc, char **argv,
                           int *argi);

void print_install_usage();
void print_install_help();

int install_command(int argc, char **argv, int *argi);

bool install_binary(const InstallOptions *options, const BuildTarget *target,
                    const char *outdir);
bool install_static(const InstallOptions *options, const BuildTarget *target,
                    const char *outdir);
bool install_shared(const InstallOptions *options, const BuildTarget *target,
                    const char *outdir);
bool install_headers(const InstallOptions *options, const BuildTarget *target,
                     const char *outdir);
bool install_pkg_config(const InstallOptions *options,
                        const BuildTarget *target, const char *outdir);

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
