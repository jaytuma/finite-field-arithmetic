# -*- makefile -*-

all: lib_vec

lib_vec: lib_vec.o
	gcc lib_vec.o -o lib_vec

lib_vec.o: lib_vec.c
	gcc -c lib_vec.c -o lib_vec.o
