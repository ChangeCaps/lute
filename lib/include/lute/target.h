#pragma once

#include <stdbool.h>

#include "package.h"
#include "path.h"

typedef enum {
    BINARY = 1 << 0,
    STATIC = 1 << 1,
    SHARED = 1 << 2,
} Output;

typedef enum {
    Wall = 1 << 0,
    Wextra = 1 << 1,
    Werror = 1 << 2,
} WarnFlag;

typedef enum {
    C,
    CXX,
} Language;

typedef struct {
    char *name;
    Output output;
    WarnFlag warn;
    Language lang;

    char *outdir;

    struct Build *build;
    Paths sources;
    Paths includes;
    Packages packages;
} Target;

typedef vec(Target *) Targets;

void target_init(Target *target, const char *name, Output kind, void *build);
void target_free(Target *target);

bool target_is(const Target *target, Output kind);
void target_infer_lang(Target *target);
char *target_compiler(const Target *target);
void target_binary(Target *target, char *path, size_t maxlen);
void target_static(Target *target, char *path, size_t maxlen);
void target_shared(Target *target, char *path, size_t maxlen);
