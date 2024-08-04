#include <errno.h>

#include "command.h"

bool execute(const Args *args) {
    char *cmd = vec_join(args, " ");
    errno = system(cmd);
    free(cmd);

    return errno == 0;
}
