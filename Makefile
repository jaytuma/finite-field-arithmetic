# -*- makefile -*-
CONFIG_FILE = ./Makefile.config
OBJ_PATH = ./objects/

 -include $(CONFIG_FILE)

all: lib_vec.exe

lib_vec.exe: $(OBJ_PATH)lib_vec.o
	gcc $(OBJ_PATH)lib_vec.o -o lib_vec $(LIB)

$(OBJ_PATH)lib_vec.o: lib_vec.c
	gcc -c lib_vec.c -o $(OBJ_PATH)lib_vec.o
