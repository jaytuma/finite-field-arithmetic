# -*- makefile -*-
CONFIG_FILE = ./Makefile.config
DEPEND_FILE = ./Makefile.deps

-include $(CONFIG_FILE)

#I expect the following variables from the config file
#
#	OBJ_PATH 	- path of the object file
#	SRC_PATH 	- path of the source code (.c/.h/.s files)
#	TEST_PATH 	- path of the source code for testing
#	TIME_PATH	- path of the source code for timing
#	EXEC_PATH	- path of the executables
#
#	LIBS		- libraries to link
#	FLAG		- flags for the compiler

#list of all the programs that create executables without extension
PROG_BN = $(basename $(wildcard $(TEST_PATH)*.c) $(wildcard $(TIME_PATH)*.c))
PROG_O_TMP = $(subst $(TEST_PATH),$(OBJ_PATH),$(PROG_BN))
PROG_O = $(addsuffix .o,$(subst $(TIME_PATH),$(OBJ_PATH),$(PROG_O_TMP)))

#list of all the file that contain source code (without extension)	
SRC_BN = $(basename $(wildcard $(SRC_PATH)*.c))
SRC_O  = $(addsuffix .o,$(subst $(SRC_PATH),$(OBJ_PATH),$(SRC_BN)))

#list of all executable to produce
EXEC_BN_TMP = $(subst $(TEST_PATH),$(EXEC_PATH),$(PROG_BN))
EXEC_BN = $(subst $(TIME_PATH),$(EXEC_PATH),$(EXEC_BN_TMP))

EXEC_EXTENSION =.exe


CC = gcc #compiler
LD = gcc #linker

	#flag for the compiler
CFLAG += $(LIBS)	#add libraries to link
CFLAG += $(FLAG)

	#flag for the linker
LDFLAG += $(LIBS)
LDFLAG += $(FLAG)

	#tr special char - should not be present in deps auto-generated by gcc
TR_CHAR = <

	
	# - - recipes - - #


all: $(addsuffix $(EXEC_EXTENSION),$(EXEC_BN))								#check all the executable that need to be done

$(EXEC_PATH)%$(EXEC_EXTENSION): $(SRC_O) $(OBJ_PATH)%.o
	$(LD) $(SRC_O) $(OBJ_PATH)$*.o -o $@ $(LDFLAG)
	
#in order to keep the object files alive
.PRECIOUS: $(SRC_O) $(PROG_O)
	
clean:
	rm -f $(OBJ_PATH)* $(EXEC_PATH)* $(DEPEND_FILE)

depend:
	rm -f $(DEPEND_FILE)												
	for i in $(addsuffix .c, $(PROG_BN) $(SRC_BN));\
	do $(CC) -MM "$${i}";\
	done > $(DEPEND_FILE)
	@cat $(DEPEND_FILE)
	tr '\n' '$(TR_CHAR)' < $(DEPEND_FILE) > $(DEPEND_FILE)__tmp__
	sed -i 's/\\$(TR_CHAR)//g' $(DEPEND_FILE)__tmp__
	tr '$(TR_CHAR)' '\n' < $(DEPEND_FILE)__tmp__ > $(DEPEND_FILE)
	rm $(DEPEND_FILE)__tmp__
	sed -i 's/^/$(subst /,\/,$(OBJ_PATH))/' $(DEPEND_FILE)
	sed -i '/^.*/a __TAB_CHAR__$$(CC) -c $$(CFLAG) $$< -o $$@\n' $(DEPEND_FILE)
	sed -i 's/__TAB_CHAR__/	/' $(DEPEND_FILE)

-include $(DEPEND_FILE)
	












