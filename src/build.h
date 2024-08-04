#pragma once

#include <lute/lute.h>

#include "command.h"

int build_command(Build *build, int argc, char **argv);
int build_help_command(Build *build, int argc, char **argv);

bool build_target(Target *target);

bool build_binary(Target *target);
bool build_static(Target *target);
bool build_shared(Target *target);

void push_binary_args(Args *args, Target *target, const char *path);
void push_static_args(Args *args, Target *target, const char *path);
void push_shared_args(Args *args, Target *target, const char *path);

void push_source_args(Args *args, Target *target);
void push_include_args(Args *args, Target *target);
void push_warning_args(Args *args, Target *target);
