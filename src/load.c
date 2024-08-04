#include <dlfcn.h>
#include <stdio.h>

#include "command.h"
#include "load.h"

bool build_load(Build *build, const char *path) {
    Package lute_package;
    package_init(&lute_package, "lute");

    Args args;
    vec_init(&args);

    vec_push(&args, "clang");
    vec_push(&args, "-shared");
    vec_push(&args, path);
    vec_push(&args, lute_package.cflags);
    vec_push(&args, lute_package.libs);
    vec_push(&args, "-o");
    vec_push(&args, "out/build.so");
    vec_push(&args, "-I");
    vec_push(&args, "lib/include");

    bool success = execute(&args);
    vec_free(&args);

    package_free(&lute_package);

    if (!success)
        return false;

    void *handle = dlopen("out/build.so", RTLD_NOW);

    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return false;
    }

    void (*build_fn)(Build *) = dlsym(handle, "build");

    if (!build_fn) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return false;
    }

    build_fn(build);
    dlclose(handle);

    return true;
}
