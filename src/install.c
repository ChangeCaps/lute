// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include <stdio.h>

#include "argp.h"
#include "build.h"
#include "install.h"
#include "util.h"

void print_install_usage() {
    printf("Usage: lute install [target] [options]\n"
           "\n"
           "Options:\n"
           "  -h, --help                Show this help message\n"
           "  -n, --dry                 "
           "Only print actions, without performing them\n"
           "      --no-build            Do not build the target\n"
           "      --bin-path            Set the binary installation path\n"
           "      --lib-path            Set the library installation path\n"
           "      --include-path        Set the include installation path\n"
           "      --pkg-config-path     Set the pkg-config installation path\n"
           "\n"
           "Platform-specific options:\n"
           "      --nix                 Install on a NixOS system"
           ", use this in the installPhase of a derivation\n");
}

void print_install_help() {
    printf("Install a target\n");
    printf("Version: %s\n\n", VERSION);
    print_install_usage();
}

InstallOptions install_options_default() {
    InstallOptions options = {0};

    options.help = false;
    options.dry = false;
    options.build = true;
    options.nix = false;
    options.bin_path = "/usr/local/bin";
    options.lib_path = "/usr/local/lib";
    options.include_path = "/usr/local/include";
    options.pkg_config_path = "/usr/lib/pkginfo";

    return options;
}

bool install_options_parse(InstallOptions *options, int argc, char **argv,
                           int *argi) {

    while (*argi < argc) {
        char *arg = argv[(*argi)++];

        if (arg_is(arg, "-h", "--help")) {
            options->help = true;
        } else if (arg_is(arg, "-n", "--dry")) {
            options->dry = true;
        } else if (arg_is(arg, NULL, "--no-build")) {
            options->build = false;
        } else if (arg_is(arg, NULL, "--bin-path")) {
            options->bin_path = argv[(*argi)++];
        } else if (arg_is(arg, NULL, "--lib-path")) {
            options->lib_path = argv[(*argi)++];
        } else if (arg_is(arg, NULL, "--include-path")) {
            options->include_path = argv[(*argi)++];
        } else if (arg_is(arg, NULL, "--pkg-config-path")) {
            options->pkg_config_path = argv[(*argi)++];
        } else if (arg_is(arg, NULL, "--nix")) {
            options->nix = true;
        } else {
            printf("Unknown option: %s\n", arg);
            return false;
        }
    }

    return true;
}

static bool set_nix_paths(InstallOptions *options, const BuildTarget *target) {
    char *out = getenv("out");
    char *lib = getenv("lib");
    char *dev = getenv("dev");

    if (out)
        make_dirs(out);
    if (lib)
        make_dirs(lib);
    if (dev)
        make_dirs(dev);

    if (target->output & BINARY) {
        if (!out) {
            printf("Nix environment variable 'out' not set\n");
            return false;
        }

        char *binpath = malloc(256);
        snprintf(binpath, 256, "%s/bin", out);

        options->bin_path = binpath;
    }

    if (target->output & (STATIC | SHARED)) {
        if (!lib) {
            printf("Nix environment variable 'lib' not set\n");
            return false;
        }

        options->lib_path = lib;
    }

    if (target->output & (STATIC | SHARED | HEADER)) {
        if (!dev) {
            printf("Nix environment variable 'dev' not set\n");
            return false;
        }

        char *incpath = malloc(256);
        snprintf(incpath, 256, "%s/include", dev);

        char *pkgpath = malloc(256);
        snprintf(pkgpath, 256, "%s/lib/pkgconfig", dev);

        options->include_path = incpath;
        options->pkg_config_path = pkgpath;
    }

    return true;
}

int install_command(int argc, char **argv, int *argi) {
    BuildGraph graph;

    if (!build_graph_load(&graph)) {
        return 1;
    }

    BuildTarget *target = select_target(&graph, argc, argv, argi);

    if (target == NULL) {
        return 1;
    }

    InstallOptions options = install_options_default();

    if (!install_options_parse(&options, argc, argv, argi)) {
        printf("\n");
        print_install_usage();
        return 1;
    }

    if (options.help) {
        print_install_help();
        return 0;
    }

    BuildOptions build_options = build_options_default();
    build_options.profile = PROFILE_RELEASE;

    char outdir[256];
    snprintf(outdir, sizeof(outdir), "lute-out/%s/%s",
             profile_name(build_options.profile), target->name);

    if (options.build && !options.dry) {
        if (!build_target(&build_options, target, target->output, outdir)) {
            return 1;
        }
    }

    if (options.nix && !set_nix_paths(&options, target)) {
        return 1;
    }

    printf("Installing target %s\n", target->name);

    if (target->output & BINARY) {
        install_binary(&options, target, outdir);
    }

    if (target->output & STATIC) {
        install_static(&options, target, outdir);
    }

    if (target->output & SHARED) {
        install_shared(&options, target, outdir);
    }

    if (target->output & (STATIC | SHARED | HEADER)) {
        install_headers(&options, target, outdir);
    }

    if (target->output & (STATIC | SHARED | HEADER)) {
        install_pkg_config(&options, target, outdir);
    }

    return 0;
}

bool install_binary(const InstallOptions *options, const BuildTarget *target,
                    const char *outdir) {

    if (!make_dirs(options->bin_path)) {
        printf("Failed to create directory %s\n", options->bin_path);
        return false;
    }

    char outpath[256];
    snprintf(outpath, sizeof(outpath), "%s/%s", options->bin_path,
             target->name);

    char binpath[256];
    snprintf(binpath, sizeof(binpath), "%s/%s", outdir, target->name);

    printf("Installing binary to %s\n", outpath);

    if (!options->dry) {
        if (!copy_file(binpath, outpath)) {
            return false;
        }
    }

    return true;
}

bool install_static(const InstallOptions *options, const BuildTarget *target,
                    const char *outdir) {

    if (!make_dirs(options->lib_path)) {
        printf("Failed to create directory %s\n", options->lib_path);
        return false;
    }

    char outpath[256];
    snprintf(outpath, sizeof(outpath), "%s/lib%s.a", options->lib_path,
             target->name);

    char libpath[256];
    snprintf(libpath, sizeof(libpath), "%s/lib%s.a", outdir, target->name);

    printf("Installing static library to %s\n", outpath);

    if (!options->dry) {
        if (!copy_file(libpath, outpath)) {
            return false;
        }
    }

    return true;
}

bool install_shared(const InstallOptions *options, const BuildTarget *target,
                    const char *outdir) {

    if (!make_dirs(options->lib_path)) {
        printf("Failed to create directory %s\n", options->lib_path);
        return false;
    }

    char outpath[256];
    snprintf(outpath, sizeof(outpath), "%s/lib%s.so", options->lib_path,
             target->name);

    char libpath[256];
    snprintf(libpath, sizeof(libpath), "%s/lib%s.so", outdir, target->name);

    printf("Installing shared library to %s\n", outpath);

    if (!options->dry) {
        if (!copy_file(libpath, outpath)) {
            return false;
        }
    }

    return true;
}

bool install_headers(const InstallOptions *options, const BuildTarget *target,
                     const char *outdir) {
    (void)outdir;

    if (!make_dirs(options->include_path)) {
        printf("Failed to create directory %s\n", options->include_path);
        return false;
    }

    char incpath[256];
    snprintf(incpath, sizeof(incpath), "%s/%s", options->include_path,
             target->name);

    if (!make_dirs(incpath)) {
        printf("Failed to create directory %s\n", incpath);
        return false;
    }

    printf("Installing headers to %s\n", incpath);

    if (!options->dry) {
        vec_foreach(&target->includes, include) {
            if (!copy_files(include, incpath)) {
                return false;
            }
        }
    }

    return true;
}

bool install_pkg_config(const InstallOptions *options,
                        const BuildTarget *target, const char *outdir) {
    (void)outdir;

    if (!make_dirs(options->pkg_config_path)) {
        printf("Failed to create directory %s\n", options->pkg_config_path);
        return false;
    }

    char outpath[256];
    snprintf(outpath, sizeof(outpath), "%s/%s.pc", options->pkg_config_path,
             target->name);

    char incpath[256];
    snprintf(incpath, sizeof(incpath), "%s/%s", options->include_path,
             target->name);

    printf("Installing pkg-config file to %s\n", outpath);

    FILE *file = fopen(outpath, "w");

    if (!file) {
        printf("Failed to open file %s\n", outpath);
        return false;
    }

    fprintf(file, "Name: %s\n", target->name);
    fprintf(file, "Description: A library for %s\n", target->name);
    fprintf(file, "Version: 0.1.0\n");

    if (target->output & (STATIC | SHARED)) {
        fprintf(file, "Libs: -L%s -l%s\n", options->lib_path, target->name);
    } else {
        fprintf(file, "Libs:\n");
    }

    fprintf(file, "Cflags: -I%s\n", incpath);

    fclose(file);

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
