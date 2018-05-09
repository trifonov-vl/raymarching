CXX = g++
CXXFLAGS = -g -Wall -std=c++0x -O1 -fopenmp

# Strict compiler options
CXXFLAGS += -Wformat-security -Wignored-qualifiers -Winit-self \
		-Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith \
		-Wtype-limits -Wempty-body -Wlogical-op \
		-Wmissing-field-initializers -Wctor-dtor-privacy \
		-Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast \
		-Woverloaded-virtual -Wsign-promo -lm

# Directories with source code

INCLUDE_DIR = include

# Add headers dirs to gcc search path
CXXFLAGS += -I $(INCLUDE_DIR)

all:main

main: main.o primitives.o objects.o
	$(CXX) $(CXXFLAGS) -o main main.o primitives.o objects.o

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp

primitives.o: src/primitives.cpp
	$(CXX) $(CXXFLAGS) -c src/primitives.cpp

objects.o: src/objects.cpp
	$(CXX) $(CXXFLAGS) -c src/objects.cpp

clean:
	rm -rf *.o main
