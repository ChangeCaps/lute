// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include "list.h"
#include "argp.h"
#include "graph.h"

void print_list_usage() {
    printf("Usage: lute list [options]\n"
           "\n"
           "Options:\n"
           "  -h --help     Show this help message\n"
           "     --none     Show only target names\n"
           "     --lang     Show target language\n"
           "     --src      Show target sources\n"
           "     --full     Show full target information\n");
}

void print_list_help() {
    printf("List available targets\n\n");
    print_list_usage();
}

ListOptions list_options_default() {
    ListOptions options = {0};

    options.help = false;
    options.output = LIST_OUTPUT_DEFAULT;

    return options;
}

bool list_options_parse(ListOptions *options, int argc, char **argv,
                        int *argi) {

    while (*argi < argc) {
        char *arg = argv[(*argi)++];

        if (arg_is(arg, "-h", "--help")) {
            options->help = true;
        } else if (arg_is(arg, NULL, "--none")) {
            options->output = LIST_OUTPUT_NONE;
        } else if (arg_is(arg, NULL, "--lang")) {
            options->output |= LIST_OUTPUT_LANGUAGE;
        } else if (arg_is(arg, NULL, "--src")) {
            options->output |= LIST_OUTPUT_SOURCE;
        } else if (arg_is(arg, NULL, "--full")) {
            options->output = LIST_OUTPUT_ALL;
        } else {
            printf("Unknown option: %s\n\n", arg);
            return false;
        }
    }

    return true;
}

static void print_target_title(const BuildTarget *target) {
    printf("  %s:", target->name);

    bool first = true;

    if (target->output & BINARY) {
        printf(" binary");
        first = false;
    }

    if (target->output & STATIC) {
        first ?: printf(",");
        printf(" static");
        first = false;
    }

    if (target->output & SHARED) {
        first ?: printf(",");
        printf(" shared");
        first = false;
    }

    if (target->output & HEADER) {
        first ?: printf(",");
        printf(" header");
        first = false;
    }

    printf("\n");
}

static void list_target_language(const BuildTarget *target) {
    printf("    Language: ");

    switch (target->lang) {
    case C:
        printf("C");
        break;
    case CPP:
        printf("C++");
        break;
    }

    printf("\n");
}

static void list_target_sources(const BuildTarget *target) {
    printf("    Sources:\n");
    vec_foreach(&target->sources, source) printf("      %s\n", source);
}

static void list_target(const ListOptions *options, const BuildTarget *target) {
    print_target_title(target);

    if (options->output & LIST_OUTPUT_LANGUAGE) {
        list_target_language(target);
    }

    if (options->output & LIST_OUTPUT_SOURCE) {
        list_target_sources(target);
    }
}

int list_command(int argc, char **argv, int *argi) {
    (void)argc;
    (void)argv;
    (void)argi;

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

        (*argi)++;

        if (!target) {
            printf("Target %s not found\n", argv[2]);
            return 1;
        }
    }

    ListOptions options = list_options_default();
    if (!list_options_parse(&options, argc, argv, argi)) {
        printf("\n");
        print_list_usage();
        return 1;
    }

    if (options.help) {
        print_list_help();
        return 0;
    }

    if (target) {
        printf("Available Target:\n");
        list_target(&options, target);
    } else {
        printf("Available targets:\n");
        vec_foreachat(&graph.root->targets, target) {
            list_target(&options, target);
        }
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
