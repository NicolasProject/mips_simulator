CC := gcc
TARGET ?= emul-mips
DEBUG ?= 1
SRCDIR := src
INCDIR := inc
BUILDDIR := obj
TARGETDIR := bin

# c compiler flags
CFLAGS := -Wall -Wextra
# c++ compiler flags
CXXFLAGS := 

# ----------------------- DO NOT MODIFY BELOW --------------------------
LIB := -lm

ifeq ($(CC), gcc)
	FLAGS := $(CFLAGS)
	SRCEXT := c
else
	FLAGS := $(CXXFLAGS)
	SRCEXT := cpp
endif
ifeq ($(DEBUG),1)
	FLAGS += -g
	TARGETDIR_FULL := $(TARGETDIR)/debug
else
	TARGETDIR_FULL := $(TARGETDIR)/release
endif

PATH_TARGET := $(TARGETDIR_FULL)/$(TARGET)
SOURCES := $(shell ls $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC := -I$(INCDIR)

$(PATH_TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR_FULL)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(PATH_TARGET) $(LIB)"; $(CC) $^ -o $(PATH_TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(FLAGS) $(INC) -c -o $@ $< $(LIB)"; $(CC) $(FLAGS) $(INC) -c -o $@ $< $(LIB)

clean:
	@echo " Cleaning..."; 
	@echo " rm -r $(BUILDDIR) $(TARGETDIR)"; rm -r $(BUILDDIR) $(TARGETDIR)
.PHONY: clean
