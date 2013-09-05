#! /usr/bin/make

.NULL: .exe

SOURCES = $(wildcard ./hw*-src/*.cpp)
HEADERS = $(wildcard ./hw*-src/*.h)
TARGETS = $(basename $(SOURCES))

INIT_SHADER = common/InitShader.o
 
CXX = g++ 

UNAME := $(shell uname)
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

default all: $(TARGETS)

$(TARGETS): $(INIT_SHADER)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#-----------------------------------------------------------------------------

%.i: %.cpp
	$(CXX) -E $(CXXFLAGS) $< | uniq > $@

#-----------------------------------------------------------------------------

clean:
	$(RM) $(DIRT)

rmtargets:
	$(RM) $(TARGETS)

clobber: clean rmtargets
