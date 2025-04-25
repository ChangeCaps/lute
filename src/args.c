// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include "args.h"

Args args_new() {
    Args args;
    vec_init(&args);
    return args;
}

void args_free(Args *args) {
    vec_foreach(args, arg) free(arg);
    vec_free(args);
}

void args_push(Args *args, const char *arg) {
    char *copy = strdup(arg);
    vec_push(args, copy);
}

char *args_join(Args *args) { return vec_join((Vec(const char *) *)args, " "); }

void args_print(FILE *file, Args *args) {
    vec_foreach(args, arg) fprintf(file, "%s ", arg);
    fprintf(file, "\n");
}

int args_exec(Args *args) {
    char *cmd = args_join(args);
    int ret = system(cmd);
    free(cmd);
    return ret;
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
