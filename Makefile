#! /usr/bin/make

.NULL: .exe

SOURCES = $(wildcard ./hw8-src/*.cpp)
HEADERS = $(wildcard ./hw8-src/*.h)
TARGETS = $(basename $(SOURCES))

INIT_SHADER = common/InitShader.o
CXX = g++ 

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CXXDEFS = -Wall -pedantic
else
	CXXDEFS = -DFREEGLUT_STATIC -DGLEW_STATIC
endif

CXXINCS = -Iinclude

CXXFLAGS = $(CXXDEFS) $(CXXINCS)

LDOPTS =
ifeq ($(UNAME_S),Darwin)
	LDLIBS = -framework OpenGL -framework GLUT
else
	LDLIBS = -lGLEW -lglut -lGL -lX11 -lm
endif

LDFLAGS = $(LDOPTS) $(LDDIRS) $(LDLIBS)

DIRT = $(wildcard *.o *.i *~ */*~ *.log)

#-----------------------------------------------------------------------------

.PHONY: Makefile

default all: $(INIT_SHADER)
	g++  -Wall -pedantic -Iinclude ./hw8-src/main.cpp common/InitShader.o   $(LDLIBS) -o hw8-src/hw8-src
$(TARGETS): $(INIT_SHADER) 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -c $@

clean:
	$(RM) $(DIRT)

rmtargets:
	$(RM) $(TARGETS)

clobber: clean rmtargets
