# mmap-testing-in-C
Copies input.txt into output.txt using mmap


This program copies an arbitrary sized file into stdout and output.txt.

This uses mmap() function to create a new mapping in the "memory mapping region" 
of the calling process' virtual space.

The fstat system call retrieves information on a file based on its
file descriptor, and stores it into a data-structure.
In the case of this small script, the struct is named "mystat"

Like any instance of a system call, one must always check the return
case.

To run:
	-have an input.txt file with any sort of text
	-compile:	gcc mmaptest.c
	-direct stdout to output.txt
	-run:		./a.out input.txt output.txt










