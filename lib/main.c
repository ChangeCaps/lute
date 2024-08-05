#include <lute/lute.h>

void build(Build *build);

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    Build b;
    build_init(&b);

    build(&b);

    serialize_build(&b, stdout);

    build_free(&b);

    return 0;
}
