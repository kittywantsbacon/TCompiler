# Copyright 2019 Justin Hu
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This file is part of the T Language Compiler.

# command options
CC := gcc
RM := rm -rf
MV := mv
MKDIR := mkdir -p
DOXYGEN := doxygen


# File options
SRCDIRPREFIX := src
OBJDIRPREFIX := bin
DEPDIRPREFIX := dependencies
MAINSUFFIX := main
TESTSUFFIX := test

# Main file options
SRCDIR := $(SRCDIRPREFIX)/$(MAINSUFFIX)
SRCS := $(shell find -O3 $(SRCDIR)/ -type f -name '*.c')

OBJDIR := $(OBJDIRPREFIX)/$(MAINSUFFIX)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

DEPDIR := $(DEPDIRPREFIX)/$(MAINSUFFIX)
DEPS := $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.dep,$(SRCS))

# Test file options
TSRCDIR := $(SRCDIRPREFIX)/$(TESTSUFFIX)
TSRCS := $(shell find -O3 $(TSRCDIR)/ -type f -name '*.c')

TOBJDIR := $(OBJDIRPREFIX)/$(TESTSUFFIX)
TOBJS := $(patsubst $(TSRCDIR)/%.c,$(TOBJDIR)/%.o,$(TSRCS))

TDEPDIR := $(DEPDIRPREFIX)/$(TESTSUFFIX)
TDEPS := $(patsubst $(TSRCDIR)/%.c,$(TDEPDIR)/%.dep,$(TSRCS))


# final executable name
EXENAME := tlc
TEXENAME := tlc-test


# compiler warnings
WARNINGS := -pedantic -pedantic-errors -Wall -Wextra -Wdouble-promotion\
-Winit-self -Wunused -Wswitch-unreachable -Wuninitialized\
-Wstringop-truncation -Wsuggest-attribute=format -Wsuggest-attribute=malloc\
-Wmissing-include-dirs -Wswitch-bool -Wduplicated-branches -Wduplicated-cond\
-Wtrampolines -Wfloat-equal -Wundef -Wshadow -Wunsafe-loop-optimizations\
-Wunused-macros -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings\
-Wconversion  -Wdate-time -Wjump-misses-init -Wlogical-op -Waggregate-return\
-Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes\
-Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline\
-Winvalid-pch -Wdisabled-optimization -Wstack-protector\
-Wunsuffixed-float-constants

# compiler options
OPTIONS := -std=c18 -m64 -D_POSIX_C_SOURCE=202002L -I$(SRCDIR) $(WARNINGS)
DEBUGOPTIONS := -Og -ggdb -Wno-unused-parameter
RELEASEOPTIONS := -O3 -D NDEBUG
TOPTIONS := -I$(TSRCDIR)
LIBS :=


.PHONY: debug release clean diagnose
.SECONDEXPANSION:
.SUFFIXES:

debug: OPTIONS := $(OPTIONS) $(DEBUGOPTIONS)
debug: $(EXENAME) $(TEXENAME)
	@echo "Generating documentation"
	@$(DOXYGEN)
	@echo "Running tests"
	@./$(TEXENAME)
	@echo "Done building debug!"

release: OPTIONS := $(OPTIONS) $(RELEASEOPTIONS)
release: $(EXENAME) $(TEXENAME)
	@echo "Generating documentation"
	@$(DOXYGEN)
	@echo "Running tests"
	@./$(TEXENAME)
	@echo "Done building release!"


clean:
	@echo "Removing all generated files and folders."
	@$(RM) $(OBJDIRPREFIX) $(DEPDIRPREFIX) $(EXENAME) $(TEXENAME)


$(EXENAME): $(OBJS)
	@echo "Linking $@"
	@$(CC) -o $(EXENAME) $(OPTIONS) $(OBJS) $(LIBS)

$(OBJS): $$(patsubst $(OBJDIR)/%.o,$(SRCDIR)/%.c,$$@) $$(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.dep,$$@) | $$(dir $$@)
	@echo "Compiling $@"
	@$(CC) $(OPTIONS) -c $< -o $@

$(DEPS): $$(patsubst $(DEPDIR)/%.dep,$(SRCDIR)/%.c,$$@) | $$(dir $$@)
	@set -e; $(RM) $@; \
	 $(CC) $(OPTIONS) -MM -MT $(patsubst $(DEPDIR)/%.dep,$(OBJDIR)/%.o,$@) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$


$(TEXENAME): $(TOBJS) $(OBJS)
	@echo "Linking $@"
	@$(CC) -o $(TEXENAME) $(OPTIONS) $(TOPTIONS) $(filter-out %main.o,$(OBJS)) $(TOBJS) $(LIBS)

$(TOBJS): $$(patsubst $(TOBJDIR)/%.o,$(TSRCDIR)/%.c,$$@) $$(patsubst $(TOBJDIR)/%.o,$(TDEPDIR)/%.dep,$$@) | $$(dir $$@)
	@echo "Compiling $@"
	@$(CC) $(OPTIONS) $(TOPTIONS) -c $< -o $@

$(TDEPS): $$(patsubst $(TDEPDIR)/%.dep,$(TSRCDIR)/%.c,$$@) | $$(dir $$@)
	@set -e; $(RM) $@; \
	 $(CC) $(OPTIONS) $(TOPTIONS) -MM -MT $(patsubst $(TDEPDIR)/%.dep,$(TOBJDIR)/%.o,$@) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$


%/:
	@$(MKDIR) $@


-include $(DEPS) $(TDEPS)