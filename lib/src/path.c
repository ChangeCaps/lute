#include <stdlib.h>

#include "lute/path.h"

void path_free(Path *source) { free(source->path); }
