#include "lute/lute.h"

Target *target(Build *build, const char *name, Output kind) {
    Target target;
    target_init(&target, name, kind, build);
    return build_push_target(build, target);
}

void source(Target *target, const char *path) {
    build_add_paths(target->build, path, &target->sources);
    target_infer_lang(target);
}

void include(Target *target, const char *path) {
    Path *include_path = build_add_path(target->build, path);
    vec_push(&target->includes, include_path);
}

void package(Target *target, const char *name) {
    Package *package = build_add_package(target->build, name);
    vec_push(&target->packages, package);
}
