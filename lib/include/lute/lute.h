#pragma once

#include "build.h"

Target *target(Build *build, const char *name, Output kind);
void source(Target *target, const char *path);
void include(Target *target, const char *path);
void package(Target *target, const char *name);
