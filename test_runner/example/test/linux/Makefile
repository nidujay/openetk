include ../../../../tools/gmsl-1.1.3/gmsl

# BLDDIR directory
BLDDIR=../../../../build/test_runner/linux/debug
EXEDIR=$(BLDDIR)/exe

ROOTDIR=../../../../test_runner
PROJDIR=$(ROOTDIR)/example/test

# List of sources
SRC = ../../../linux/main.c 			\
	../../../example/sample.c 			\
	../../../example/test/test_sample.c 

# Resolve the path of the object files
OBJ = $(SRC:../../../%.c=$(BLDDIR)/obj/%.o)

all: $(EXEDIR)/test_sample

clean:
	@rm -rf $(OBJ)
	@rm -rf $(EXEDIR)/test_sample

# rule to build the executable
$(EXEDIR)/test_sample: $(OBJ)
# make the directories if not already there
	@mkdir -p $(@D)
# link it all
	@gcc -Wextra -Wextra -I$(ROOTDIR) -I$(PROJDIR) $^ -o $@

# Rule to build the objects. We need to resolve back to the source dir
$(BLDDIR)/obj/%.o:  ../../../%.c
	@echo $<
# first make sure that the directory of the object exists
	@mkdir -p $(@D)
# now build the object file
	@gcc -g -Wextra -Wextra -I$(ROOTDIR) -I$(PROJDIR) -c $< -o $@
