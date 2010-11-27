# Name of the executable to create:
PRG_NAME=bin/tp5

# Sources:
SOURCES=\
	src/Util.cpp\
	src/image.cpp\
	src/main.cpp

# Where to search for the sources:
VPATH=src

CXX = g++
CXXFLAGS += -MMD

DEPS=$(patsubst %.cpp,%.d,$(SOURCES))
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

BIN_DIR:=$(shell dirname "$(PRG_NAME)")

# Rules to always execute.
.PHONY: all clean mrproper

# Default action.
all: $(PRG_NAME)

# Delete the object files.
clean:
	$(RM) $(OBJECTS) $(DEPS)

# Delete the object files and the compiled program.
mrproper: clean
	$(RM) $(PRG_NAME)

# Linkage.
$(PRG_NAME): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Create the BIN_DIR.
$(BIN_DIR):
	mkdir -p $@

# The compilation depends on this Makefile.
$(OBJECTS): Makefile

-include $(DEPS)

