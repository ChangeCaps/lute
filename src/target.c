#include "target.h"

Target *select_target(Build *build, const char *name) {
    if (!name && build->targets.len > 0) {
        return build->targets.data[0];
    }

    for (size_t i = 0; i < build->targets.len; i++) {
        Target *target = build->targets.data[i];

        if (strcmp(target->name, name) == 0) {
            return target;
        }
    }

    return NULL;
}
