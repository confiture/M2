##
# My Great Makefile v0.4.2
#
# Julien Fontanet <julien.fontanet@isonoe.net>
#
# Copyleft 2010
##

##
# This Makefile works only with gcc/g++ (g++ is used for linking).
# The C prefix is ".c".
# The C++ prefix is ".cpp".
##

########################
# Global configuration #
########################

# Enables colored messages during compilation.
COLORS := 1

# Compiles in debug mode. (Necessary for the tests.)
DEBUG := 1

# Compiles with OpenMP.
OPENMP := 0

# Enables profiling.
PROFILING := 0

# Strips the binary from all its debug information (reduce the binary size).
STRIP_BIN := 0

# Displays which commands are executed during compilation.
VERBOSE := 0

# Projects list.
PROJECTS := tp8

PREFIX := /usr/local

CXXFLAGS += -std=c++98 -pedantic -Wall -ggdb3
CXXFLAGS +=  -Wextra -Winline -Wconversion -ggdb3
CXXFLAGS += -I ../include/

LDFLAGS +=


#############################
# Per project configuration #
#############################

tp8_SRCS := src/image.cpp src/Util.cpp src/main.cpp
###########################################
# DO NOT MODIFY ANYTHING AFTER THIS LINE. #
###########################################

.DEFAULT_GOAL  := all
CC             := gcc
CFLAGS         += -MMD
CXX            := g++
CXXFLAGS       += -MMD
COMMON_OBJECTS := $(addsuffix .o,$(COMMON_SRCS))
COMMON_DEPS    := $(addsuffix .d,$(COMMON_SRCS))

ifneq ($(DEBUG),1)
CFLAGS += -DNDEBUG -fno-strict-aliasing -funroll-loops -O3 -g0
CXXFLAGS += -DNDEBUG -fno-strict-aliasing -funroll-loops -O3 -g0
endif

ifeq ($(OPENMP),1)
CFLAGS += -fopenmp
CXXFLAGS += -fopenmp
LDFLAGS  += -fopenmp
endif

ifeq ($(PROFILING),1)
CFLAGS   += -pg
CXXFLAGS += -pg
LDFLAGS  += -pg
endif

ifneq ($(VERBOSE),1)
.SILENT:
endif

ifeq ($(COLORS),1)
COLOR_RED := \033[0;22;31m
COLOR_GREEN := \033[0;22;32m
COLOR_YELLOW := \033[33m
COLOR_BLUE := \033[0;22;34m
COLOR_PURPLE := \033[0;22;35m
COLOR_CYAN := \033[0;22;36m

COLOR_RED_BOLD := \033[0;1;31m
COLOR_GREEN_BOLD := \033[0;1;32m
COLOR_YELLOW_BOLD := \033[0;1;33m
COLOR_BLUE_BOLD := \033[0;1;34m
COLOR_PURPLE_BOLD := \033[0;1;35m
COLOR_CYAN_BOLD := \033[0;1;36m

COLOR_NONE := \033[0m
COLOR_NONE_BOLD := \033[1m
endif

# Rules to always execute.
.PHONY: all clean distclean install uninstall

# Generic rule definition.
define PROJECT_TPL
# The default target is "bin/PROJECT_NAME".
$(1)_TARGET ?= bin/$(1)

# The default source is "PROJECT_NAME/main.cpp".
$(1)_SRCS ?= $(1)/main.cpp

# The default install directory is "PREFIX/bin".
$(1)_INSTALL_DIR ?= $(PREFIX)/bin
$(1)_INSTALL ?= $$($(1)_INSTALL_DIR)/$$(notdir $$($(1)_TARGET))

$(1)_OBJECTS := $$(addsuffix .o,$$($(1)_SRCS))
$(1)_DEPS    := $$(addsuffix .d,$$($(1)_SRCS))

$$($(1)_OBJECTS): CFLAGS := $(CFLAGS) $$($(1)_CFLAGS)
$$($(1)_OBJECTS): CXXFLAGS := $(CXXFLAGS) $$($(1)_CXXFLAGS)
$$($(1)_OBJECTS): Makefile

-include $$($(1)_DEPS)

all: $$($(1)_TARGET)
$$($(1)_TARGET): $$($(1)_OBJECTS) $(COMMON_OBJECTS)
	mkdir -p -- '$$(@D)'
	@printf '  $(COLOR_PURPLE_BOLD)L  %s$(COLOR_NONE)\n' '$$@'
	$(CXX) $(LDFLAGS) $$($(1)_LDFLAGS) -o '$$@' $$^
ifeq ($(STRIP_BIN),1)
	strip '$$@'
endif

$$($(1)_INSTALL): $$($(1)_TARGET)
	@printf '  $(COLOR_YELLOW_BOLD)I  %s$(COLOR_NONE)\n' '$$($(1)_INSTALL)'
	mkdir -p -- '$$($(1)_INSTALL_DIR)'
	cp -f $$^ '$$($(1)_INSTALL)'

.PHONY: install-$(1) uninstall-$(1) clean-$(1) distclean-$(1)

install: install-$(1)
install-$(1): $$($(1)_INSTALL)

uninstall: uninstall-$(1)
uninstall-$(1):
	@printf '  $(COLOR_YELLOW_BOLD)U  %s$(COLOR_NONE)\n' '$$($(1)_INSTALL)'
	$(RM) -v '$$($(1)_INSTALL)'

clean: clean-$(1)
clean-$(1):
	$(RM) -v $$($(1)_DEPS) $$($(1)_OBJECTS)

distclean: distclean-$(1)
distclean-$(1): clean-$(1)
	$(RM) -v $$($(1)_TARGET)
	rmdir --parents --ignore-fail-on-non-empty -- '$$(dir $$($(1)_TARGET))'
endef

# Creates rules for each project.
$(foreach project,$(PROJECTS),$(eval $(call PROJECT_TPL,$(project))))

distclean: clean-COMMON
clean: clean-COMMON
clean-COMMON:
	$(RM) -v $(COMMON_DEPS) $(COMMON_OBJECTS)

# Generic rules
.SUFFIXES: # Disable auto rules

%.c.o: %.c
	@printf '  $(COLOR_BLUE_BOLD)CC %s$(COLOR_NONE)\n' '$@'
	$(CC) $(CFLAGS) -c -o '$@' '$<'

%.cpp.o: %.cpp
	@printf '  $(COLOR_BLUE_BOLD)CC %s$(COLOR_NONE)\n' '$@'
	$(CXX) $(CXXFLAGS) -c -o '$@' '$<'
