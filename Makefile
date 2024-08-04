CXX = clang
CXXFLAGS = -Ilib/include -Wall -Wextra -g

LIB_SOURCES = $(wildcard lib/src/*.c)
LIB_OBJECTS = $(LIB_SOURCES:lib/src/%.c=out/lib/%.o)
LIB_DEPENDS = $(LIB_OBJECTS:.o=.d)

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:src/%.c=out/%.o)
DEPENDS = $(OBJECTS:.o=.d)

all: out/lute out/lib/liblute.so

clean:
	rm -rf out

out/:
	mkdir -p out

out/lib/: out/
	mkdir -p out/lib

out/lib/%.o: lib/src/%.c out/lib/
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

out/lib/liblute.so: $(LIB_OBJECTS)
	$(CXX) $(CXXFLAGS) -shared $(LIB_OBJECTS) -o out/lib/liblute.so

out/%.o: src/%.c out/
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(LIB_DEPENDS)
-include $(DEPENDS)

out/lute: $(LIB_OBJECTS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LIB_OBJECTS) $(OBJECTS) -o out/lute
