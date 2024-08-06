CC = clang
CCFLAGS = -Ilib/include -Wall -Wextra -g

LIB_SOURCES = $(wildcard lib/src/*.c)
LIB_OBJECTS = $(LIB_SOURCES:lib/src/%.c=out/lib/%.o)
LIB_DEPENDS = $(LIB_OBJECTS:.o=.d)

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:src/%.c=out/%.o)
DEPENDS = $(OBJECTS:.o=.d)

all: out out/lib out/lute out/lib/lutebuild.o

clean:
	rm -rf out

out:
	mkdir -p out

out/lib: out
	mkdir -p out/lib

out/lib/%.o: lib/src/%.c
	$(CC) $(CCFLAGS) -MMD -MP -c $< -o $@

out/lib/lutebuild.o: $(LIB_OBJECTS)
	$(CC) $(CCFLAGS) -c lib/main.c -o out/lib/main.o
	ld -r $(LIB_OBJECTS) out/lib/main.o -o out/lib/lutebuild.o

out/%.o: src/%.c
	$(CC) $(CCFLAGS) -MMD -MP -c $< -o $@

-include $(LIB_DEPENDS)
-include $(DEPENDS)

out/lute: $(LIB_OBJECTS) $(OBJECTS)
	$(CC) $(CCFLAGS) $(LIB_OBJECTS) $(OBJECTS) -o out/lute
