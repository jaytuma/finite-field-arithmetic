# -*- makefile -*-
CONFIG_FILE = ./Makefile.config
OBJ_PATH = ./objects/

 -include $(CONFIG_FILE)

all: test_lib.exe

test_lib.exe: $(OBJ_PATH)lib_vec.o $(OBJ_PATH)test_vec.o
	gcc $(OBJ_PATH)lib_vec.o $(OBJ_PATH)test_vec.o -o test_vec.exe $(LIB)

$(OBJ_PATH)test_vec.o: lib_vec.o lib_vec.h test_vec.c test_vec.h
	gcc -c $(LIB) test_vec.c -o $(OBJ_PATH)test_vec.o

$(OBJ_PATH)lib_vec.o: lib_vec.c lib_vec.h macro.h
	gcc -c $(LIB) lib_vec.c -o $(OBJ_PATH)lib_vec.o

lib_vec.h:


test_vec.h:


macro.h:
