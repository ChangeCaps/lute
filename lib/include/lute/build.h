#pragma once

#include "package.h"
#include "path.h"
#include "target.h"

typedef struct Build {
    char *cc;
    char *cxx;

    Targets targets;
    Paths paths;
    Packages packages;
} Build;

void build_init(Build *build);
void build_free(Build *build);

Target *build_push_target(Build *build, Target target);
Path *build_push_path(Build *build, Path source);
Package *build_push_package(Build *build, Package package);
Package *build_add_package(Build *build, const char *name);

/// Load a source file into the build.
Path *build_add_path(Build *build, const char *path);

/// Load source files into the build.
void build_add_paths(Build *build, const char *path, Paths *paths);
