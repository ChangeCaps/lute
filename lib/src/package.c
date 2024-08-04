#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lute/package.h"

#define BUFFER_SIZE 4096

char *get_command_output(const char *command) {
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        printf("Failed to run command\n");
        exit(1);
    }

    char *result = malloc(BUFFER_SIZE);

    if (fgets(result, BUFFER_SIZE, pipe) == NULL) {
        free(result);
        pclose(pipe);
        exit(1);
    }

    // check the status of the command
    if (pclose(pipe) != 0) {
        free(result);
        exit(1);
    }

    // for whatever reason pkgconf adds a newline to the end of the output
    // this is a hacky way to remove it
    char *newline = strchr(result, '\n');

    if (newline)
        *newline = '\0';

    return result;
}

char *get_cflags(const char *name) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "pkg-config --cflags %s", name);
    return get_command_output(buffer);
}

char *get_libs(const char *name) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "pkg-config --libs %s", name);
    return get_command_output(buffer);
}

void package_init(Package *package, const char *name) {
    package->name = strdup(name);
    package->cflags = get_cflags(name);
    package->libs = get_libs(name);
}

void package_free(Package *package) {
    free(package->name);
    free(package->cflags);
    free(package->libs);
}
