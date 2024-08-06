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

int build_command(int argc, char **argv) {
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

    char outdir[256];
    snprintf(outdir, sizeof(outdir), "out/%s", target->name);

    if (!build_target(target, target->output, outdir)) {
        printf("Build of target %s failed, exiting\n", target->name);
        return 1;
    }

    return 0;
}

int build_help_command(BuildGraph *build, int argc, char **argv) {
    (void)build;
    (void)argc;
    (void)argv;

    printf("Usage: lute build [target]\n");
    return 0;
}

bool build_target(const BuildTarget *target, Output output,
                  const char *outdir) {

    vec_foreach(&target->deps, dep) {
        char depoutdir[256];
        snprintf(depoutdir, sizeof(depoutdir), "out/deps/%s", dep->id);

        if (!build_target(dep->node->targets.data, STATIC | BINARY, depoutdir))
            return false;
    }

    if (!build_objects(target, outdir))
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
        args_push(&args, "-O0");

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
            snprintf(depoutdir, sizeof(depoutdir), "out/deps/%s", dep->id);

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
        args_push(&args, "-g");
        args_push(&args, "-O0");

        vec_foreach(&target->packages, package) {
            args_push(&args, package->libs);
        }

        vec_foreach(&target->deps, dep) {
            if (!(dep->target->output & STATIC))
                continue;

            char deplib[256];
            snprintf(deplib, sizeof(deplib), "out/deps/%s/lib%s.a", dep->id,
                     dep->name);

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
        args_push(&args, "-g");
        args_push(&args, "-O0");

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
            snprintf(depoutdir, sizeof(depoutdir), "out/deps/%s", dep->id);

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

bool build_objects(const BuildTarget *target, const char *outdir) {
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
        args_push(&args, "-g");
        args_push(&args, "-O0");

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
