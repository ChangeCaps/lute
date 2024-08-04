#pragma once

#include "vector.h"

typedef enum {
    SOURCE,
    HEADER,
    DIRECTORY,
} FileKind;

/// Represents a path to a source file.
typedef struct {
    /// The path to the source file.
    char *path;

    /// The kind of the source file.
    FileKind kind;
} Path;

void path_free(Path *source);

typedef vec(Path *) Paths;
