#! /usr/bin/make

.NULL: .exe

SOURCES = $(wildcard ./project1/*.cpp)
HEADERS = $(wildcard ./project1/*.h)
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
	g++  -Wall -pedantic -Iinclude ./project1/main.cpp common/InitShader.o   $(LDLIBS) -o project1/project1
$(TARGETS): $(INIT_SHADER) 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -c $@

clean:
	$(RM) $(DIRT)

rmtargets:
	$(RM) $(TARGETS)

clobber: clean rmtargets
