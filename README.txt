README

This is my first git repo and its aim is to build a finite field aritmetic library (ffa) written in C.

I'm planning to write a fine documentation of everything. Things are easier to document when you still remember why you did them right?

+ how to build:
  i'm planning to add a "install" command in the makefile, until then you should add 5 directories in your local repo (namely src, test, time, exec and bin), then run "make depend" and only then run make. You will find executables in the exec folder.
  
+ how to code:
  simply try to keep the style you see in other .c source file. I take inspiration from gmp in order to name functions - which may seem tedious, but in the end i believe it pays off.
 
+ what is the plan:
  right now just to build a library for finite field arithmetic. As a first approach i'll implement everything in the polynomial base (therefore i need a library for polynomial over large prime finite field). It would be nice if relative small "power of two" field (i.e. 2^16, 2^32 or 2^64) could have low-level code.
