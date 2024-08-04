# Lute
Lute is yet another build system for C and C++, that aims for simplicity and ease of use.

## Configuration
A lute project is configured with a `build.c` file in the project root.
```c
// build.c

#include <lute/lute.h>

void build(Build *build) {
    // add an executable target called lute-example
    Target *exe = target(build, "lute-example", BINARY);

    // add Wall and Wextra to the warning flags
    exe->warn = Wall | Wextra;

    // add the 'src' directory as the source files
    source(exe, "src");
}
```

## License
Lute is licensed under AGPL-3.0, see [LICENSE] for more information.
