#pragma once

#include "vector.h"

typedef struct {
    char *name;
    char *cflags;
    char *libs;
} Package;

typedef vec(Package *) Packages;

void package_init(Package *package, const char *name);
void package_free(Package *package);
