#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lute/lute.h>

#include "build.h"
#include "command.h"
#include "load.h"
#include "run.h"
#include "target.h"
#include "util.h"

void help_build() {
    //
    printf("Usage: lute build [target]\n");
}

int run_command(Build *build, int argc, char **argv) {
    char *target_name = NULL;

    if (argc >= 3) {
        target_name = argv[2];
    }

    Target *target = select_target(build, target_name);

    if (!target) {
        printf("Error: Could not find target\n\n");
        help_build();
        return 1;
    }

    if (build_binary(target)) {
        return 1;
    }

    char output[256];
    target_binary(target, output, sizeof(output));

    printf("Running target: %s\n", output);

    Args args;
    vec_init(&args);
    vec_push(&args, output);

    if (!execute(&args)) {
        vec_free(&args);
        return 1;
    }

    vec_free(&args);

    return 0;
}

int clean_command(Build *build) {
    printf("Cleaning targets\n");

    vec_foreach(&build->targets, target) {
        printf("  %s\n", target->name);
        remove(target->outdir);
    }

    return 0;
}

int list_command(Build *build) {
    printf("Targets:\n");

    vec_foreach(&build->targets, target) {
        printf("  %s\n", target->name);
        printf("    Output:");

        if (target_is(target, BINARY))
            printf(" Executable");
        if (target_is(target, STATIC))
            printf(" Static Library");
        if (target_is(target, SHARED))
            printf(" Shared Library");

        printf("\n");

        printf("    Language: %s\n", target->lang == C ? "C" : "C++");
        printf("    Output Directory: %s\n", target->outdir);
    }

    return 0;
}

void help_lute() {
    printf("Usage: lute [command]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  build\n");
    printf("  run\n");
    printf("  clean\n");
    printf("  list\n");
    printf("  help\n");
}

int lute(Build *build, int argc, char **argv) {
    if (argc < 2) {
        help_lute();
        return 1;
    }

    if (strcmp(argv[1], "build") == 0) {
        return build_command(build, argc, argv);
    } else if (strcmp(argv[1], "run") == 0) {
        return run_command(build, argc, argv);
    } else if (strcmp(argv[1], "clean") == 0) {
        return clean_command(build);
    } else if (strcmp(argv[1], "list") == 0) {
        return list_command(build);
    } else if (strcmp(argv[1], "help") == 0) {
        help_lute();
    } else {
        printf("Unknown command: %s\n", argv[1]);
        help_lute();
    }

    return 0;
}

int main(int argc, char **argv) {
    if (!make_dir("out")) {
        printf("Error: Could not create out directory\n");
        return 1;
    }

    Build build;
    build_init(&build);

    if (!build_load(&build, "build.c")) {
        build_free(&build);

        return 1;
    }

    int code = lute(&build, argc, argv);

    build_free(&build);

    return code;
}
