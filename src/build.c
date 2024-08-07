// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <lute/target.h>
#include <stdio.h>

#include "args.h"
#include "build.h"
#include "util.h"

static const char *get_compiler(const BuildTarget *target) {
    char *cc = getenv("CC");
    char *cxx = getenv("CXX");

    cc = cc ? cc : "clang";
    cxx = cxx ? cxx : "clang++";

    switch (target->lang) {
    case C:
        return cc;
    case CPP:
        return cxx;
    default:
        return NULL;
    }
}

void help_build_options() {
    printf("  -h --help                 Show this help message\n");
    printf("  -r --release              Build with release profile\n");
    printf("  -d --debug (default)      Build with debug profile\n");
}

void build_help_command() {
    printf("Usage: lute build [target] [options]\n");
    printf("\n");
    printf("Options:\n");
    help_build_options();
}

BuildOptions build_options_default() {
    BuildOptions options = {0};
    options.help = false;
    options.profile = DEBUG;
    return options;
}

static bool is_arg(const char *arg, const char *short_name,
                   const char *long_name) {
    bool is_short = short_name && strcmp(arg, short_name) == 0;
    bool is_long = long_name && strcmp(arg, long_name) == 0;

    return is_short || is_long;
}

bool build_options_parse(BuildOptions *options, int argc, char **argv, int *i) {
    for (; *i < argc; (*i)++) {
        char *arg = argv[*i];

        if (is_arg(arg, "-h", "--help")) {
            options->help = true;
        } else if (is_arg(arg, "-r", "--release")) {
            options->profile = RELEASE;
        } else if (is_arg(arg, "-d", "--debug")) {
            options->profile = DEBUG;
        } else if (strcmp(argv[*i], "--")) {
            return true;
        } else {
            printf("Error: Unknown option %s\n", argv[*i]);
            return false;
        }
    }

    return true;
}

int build_command(int argc, char **argv) {
    BuildGraph graph;

    if (!build_graph_load(&graph)) {
        return 1;
    }

    int argi = 2;
    BuildTarget *target = NULL;

    if (argc > 2 && is_valid_target_name(argv[2])) {
        vec_foreachat(&graph.root->targets, t) {
            if (strcmp(t->name, argv[2]) == 0)
                target = t;
        }

        argi++;

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

    BuildOptions options = build_options_default();

    if (!build_options_parse(&options, argc, argv, &argi)) {
        return 1;
    }

    if (options.help) {
        build_help_command();
        return 0;
    }

    char outdir[256];
    snprintf(outdir, sizeof(outdir), "out/%s", target->name);

    if (!build_target(&options, target, target->output, outdir)) {
        printf("Build of target %s failed, exiting\n", target->name);
        return 1;
    }

    return 0;
}

bool build_target(const BuildOptions *options, const BuildTarget *target,
                  Output output, const char *outdir) {
    printf("Building target %s", target->name);

    switch (options->profile) {
    case DEBUG:
        printf("[debug]\n");
        break;
    case RELEASE:
        printf("[release]\n");
        break;
    }

    vec_foreach(&target->deps, dep) {
        char depoutdir[256];
        snprintf(depoutdir, sizeof(depoutdir), ".lute/deps/out/%s", dep->id);

        if (!build_target(options, dep->node->targets.data, STATIC | BINARY,
                          depoutdir))
            return false;
    }

    if (!build_objects(options, target, outdir))
        return false;

    const char *compiler = get_compiler(target);

    if (!compiler) {
        printf("Error: No compiler found\n");
        return false;
    }

    Vec(const char *) objects;
    vec_init(&objects);

    vec_foreach(&target->sources, source) {
        HashId id;
        hash_string(id, "obj", source);

        char *object = malloc(256);
        snprintf(object, 256, "%s/%s.o", outdir, id);

        vec_push(&objects, object);
    }

    char *objs = vec_join(&objects, " ");
    vec_free(&objects);

    if (target->output & output & BINARY) {
        printf("Building binary %s\n", target->name);

        char binpath[256];
        snprintf(binpath, sizeof(binpath), "%s/%s", outdir, target->name);

        Args args = args_new();
        args_push(&args, compiler);
        args_push(&args, objs);
        args_push(&args, "-o");
        args_push(&args, binpath);
        args_push(&args, "-g");

        switch (options->profile) {
        case DEBUG:
            args_push(&args, "-g");
            args_push(&args, "-O1");
            break;
        case RELEASE:
            args_push(&args, "-O3");
            break;
        }

        if (target->std) {
            char std[256];
            snprintf(std, sizeof(std), "-std=%s", target->std);
            args_push(&args, std);
        }

        vec_foreach(&target->packages, package) {
            args_push(&args, package->libs);
        }

        vec_foreach(&target->deps, dep) {
            if (!(dep->target->output & LIBRARY))
                continue;

            char depoutdir[256];
            snprintf(depoutdir, sizeof(depoutdir), ".lute/deps/out/%s",
                     dep->id);

            args_push(&args, "-L");
            args_push(&args, depoutdir);
            args_push(&args, "-l");
            args_push(&args, dep->name);
        }

        bool success = args_exec(&args) == 0;
        args_free(&args);

        if (!success) {
            printf("Error: Could not build binary %s\n", target->name);
            return false;
        }
    }

    if (target->output & output & STATIC) {
        printf("Building static library %s\n", target->name);

        char libpath[256];
        snprintf(libpath, sizeof(libpath), "%s/lib%s.a", outdir, target->name);

        Args args = args_new();

        args_push(&args, "ar");
        args_push(&args, "rcs");
        args_push(&args, libpath);
        args_push(&args, objs);

        vec_foreach(&target->packages, package) {
            args_push(&args, package->libs);
        }

        vec_foreach(&target->deps, dep) {
            if (!(dep->target->output & STATIC))
                continue;

            char deplib[256];
            snprintf(deplib, sizeof(deplib), ".lute/deps/out/%s/lib%s.a",
                     dep->id, dep->name);

            args_push(&args, deplib);
        }

        bool success = args_exec(&args) == 0;
        args_free(&args);

        if (!success) {
            printf("Error: Could not build static library %s\n", target->name);
            return false;
        }
    }

    if (target->output & output & SHARED) {
        printf("Building shared library %s\n", target->name);

        char libpath[256];
        snprintf(libpath, sizeof(libpath), "%s/lib%s.so", outdir, target->name);

        Args args = args_new();

        args_push(&args, compiler);
        args_push(&args, "-shared");
        args_push(&args, objs);
        args_push(&args, "-o");
        args_push(&args, libpath);

        switch (options->profile) {
        case DEBUG:
            args_push(&args, "-g");
            args_push(&args, "-O1");
            break;
        case RELEASE:
            args_push(&args, "-O3");
            break;
        }

        if (target->std) {
            char std[256];
            snprintf(std, sizeof(std), "-std=%s", target->std);
            args_push(&args, std);
        }

        vec_foreach(&target->packages, package) {
            args_push(&args, package->libs);
        }

        vec_foreach(&target->deps, dep) {
            if (!(dep->target->output & SHARED))
                continue;

            char depoutdir[256];
            snprintf(depoutdir, sizeof(depoutdir), ".lute/deps/out/%s",
                     dep->id);

            args_push(&args, "-L");
            args_push(&args, depoutdir);
            args_push(&args, "-l");
            args_push(&args, dep->name);
        }

        bool success = args_exec(&args) == 0;
        args_free(&args);

        if (!success) {
            printf("Error: Could not build shared library %s\n", target->name);
            return false;
        }
    }

    return true;
}

static void push_includes(Args *args, const BuildTarget *target) {
    vec_foreach(&target->includes, include) {
        args_push(args, "-I");
        args_push(args, include);
    }

    vec_foreach(&target->deps, dep) push_includes(args, dep->target);
}

bool build_objects(const BuildOptions *options, const BuildTarget *target,
                   const char *outdir) {
    if (!make_dirs(outdir)) {
        printf("Error: Could not create output directory\n");

        return false;
    }

    // get the compiler for the target, default to clang
    const char *compiler = get_compiler(target);

    if (!compiler) {
        printf("Error: No compiler found\n");
        return false;
    }

    vec_foreach(&target->sources, source) {
        printf("Compiling %s\n", source);

        HashId id;
        hash_string(id, "obj", source);

        char object[256];
        snprintf(object, sizeof(object), "%s/%s.o", outdir, id);

        Args args = args_new();
        args_push(&args, compiler);
        args_push(&args, "-c");
        args_push(&args, source);
        args_push(&args, "-o");
        args_push(&args, object);

        switch (options->profile) {
        case DEBUG:
            args_push(&args, "-g");
            args_push(&args, "-O1");
            break;
        case RELEASE:
            args_push(&args, "-O3");
            break;
        }

        if (target->std) {
            char std[256];
            snprintf(std, sizeof(std), "-std=%s", target->std);
            args_push(&args, std);
        }

        push_includes(&args, target);

        vec_foreach(&target->packages, package) {
            args_push(&args, package->cflags);
        }

        if (target->warn & Wall)
            args_push(&args, "-Wall");
        if (target->warn & Wextra)
            args_push(&args, "-Wextra");
        if (target->warn & Werror)
            args_push(&args, "-Werror");

        bool success = args_exec(&args) == 0;
        args_free(&args);

        if (!success) {
            printf("Error: Could not compile %s\n", source);
            return false;
        }
    }

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
