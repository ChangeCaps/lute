// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include "init.h"
#include "argp.h"
#include "fs.h"
#include "log.h"

InitOptions init_options_default() {
    InitOptions options = {0};

    options.lang = C;

    return options;
}

bool init_options_parse(InitOptions *options, int argc, char **argv,
                        int *argi) {
    while (*argi < argc) {
        char *arg = argv[(*argi)++];

        if (arg_is(arg, "-h", "--help")) {
            options->help = true;
        } else if (arg_is(arg, "-l", "--lang")) {
            char *lang_arg = argv[(*argi)++];

            if (strcmp(lang_arg, "c") == 0 || strcmp(lang_arg, "C") == 0) {
                options->lang = C;
            } else if (strcmp(lang_arg, "cpp") == 0 ||
                       strcmp(lang_arg, "Cpp") == 0 ||
                       strcmp(lang_arg, "CPP") == 0 ||
                       strcmp(lang_arg, "c++") == 0 ||
                       strcmp(lang_arg, "C++") == 0) {
                options->lang = CXX;
            } else {
                printf("Unknown language: '%s'\n", lang_arg);
                return false;
            }
        } else {
            printf("Unknown option: %s\n", arg);
            return false;
        }
    }

    return true;
}

void print_init_usage() {
    INFO("Usage: lute init [options]\n"
         "\n"
         "Options:\n"
         "  -h, --help                  Show this help message\n"
         "  -l, --lang  (default: c)    The language of the default target\n");
}

void print_init_help() {
    INFO("Initialize a new project\n");
    INFO("Version: %s\n\n", VERSION);
    print_init_usage();
}

int init_command(int argc, char **argv, int *argi) {
    InitOptions options = init_options_default();

    if (!init_options_parse(&options, argc, argv, argi)) {
        INFO("\n");
        print_init_usage();
        return 1;
    }

    if (options.help) {
        print_init_help();
        return 0;
    }

    char *cwd = get_working_dir();
    if (!cwd) {
        ERROR("Failed to get current working directory\n");
        return 1;
    }

    char *name = strrchr(cwd, '/');

    if (file_exists("build.c")) {
        INFO("Project already initialized\n");
        free(cwd);
        return 1;
    }

    INFO("Initializing project `%s`\n", name);

    if (!write_build_file(options, name)) {
        free(cwd);
        return 1;
    }

    if (!write_gitignore_file()) {
        free(cwd);
        return 1;
    }

    INFO("Project `%s` initialized\n", name);

    free(cwd);

    return 0;
}

bool write_build_file(InitOptions options, const char *name) {
    FILE *file = fopen("build.c", "w");

    if (!file) {
        ERROR("Failed to create `build.c` file\n");
        return false;
    }

    char *lang = options.lang == C ? "C" : "CXX";

    fprintf(file, "#include <lute/lute.h>\n");
    fprintf(file, "\n");
    fprintf(file, "void build(Build *b) {\n");
    fprintf(file, "    Target *trg = target(b, \"%s\", BINARY);\n", name);
    fprintf(file, "\n");
    fprintf(file, "    trg->warn = Wall | Wextra;\n");
    fprintf(file, "    trg->lang = %s;\n", lang);
    fprintf(file, "}\n");

    fclose(file);

    INFO("Created `build.c` file\n");

    return true;
}

bool write_gitignore_file() {
    if (file_exists(".gitignore")) {
        INFO("`.gitignore` file already exists\n");
        return true;
    }

    FILE *file = fopen(".gitignore", "w");

    if (!file) {
        ERROR("Failed to create `.gitignore` file\n");
        return false;
    }

    fprintf(file, "# Lute build artifacts\n");
    fprintf(file, "lute-cache/\n");
    fprintf(file, "lute-out/\n");
    fprintf(file, "\n");
    fprintf(file, "# Compile commands\n");
    fprintf(file, "compile_commands.json\n");

    fclose(file);

    INFO("Created `.gitignore` file\n");

    return true;
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
