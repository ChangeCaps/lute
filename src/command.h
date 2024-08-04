#pragma once

#include <stdbool.h>

#include <lute/vector.h>

typedef vec(const char *) Args;

bool execute(const Args *args);
