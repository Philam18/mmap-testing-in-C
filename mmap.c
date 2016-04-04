#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>

/**
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

**/

int main(int argc, char *argv[]){
	//ERROR: did not provide a file
	if(argc < 3){
		perror("Please provide input & output file");
		return 0;
	}

	//open file
	// -open for reading/writing
	int fileDescriptor = open(argv[1], O_RDWR);
	struct stat mystat;

	//ERROR: cannot open file
	if (fileDescriptor == -1){
		perror("error opening file");
		exit(1);
	}

	//retrieve file size
	//ERROR: failure to 
	if (fstat(fileDescriptor, &mystat) < 0){
		perror("error getting file size");
		close(fileDescriptor);
		exit(1);
	}

	//ERROR: file is empty!
	if (mystat.st_size == 0){
		perror("size of file is ZERO!");
		exit(1);
	}

	//map the input file into the virtual memory
	// -address 0
	// -the mapping will be the size of the input file
	// -map the file, read and write only protection
	// -mapdata can be accessed/changed by other processes
	// -map the fileDescriptor
	// -offset of 0, map entire file
	char *map = mmap(0, mystat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	//ERROR: if the return by mmap() is "MAP_FAILED"
	if (map == MAP_FAILED){
		perror("mmap failed");
		close(fileDescriptor);
		exit(1);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//writing portion
	off_t i;
	//open a new output file
	FILE *f = fopen(argv[2],"w");
	if (f== NULL){
		perror("unable to open file");
	exit(1);
	} 
	//go through each character in the mapped memory and write to an output file
	//and print to stdout
	for (i = 0; i < mystat.st_size; i++){
		printf("%c", map[i]);
		fprintf(f,"%c",map[i]);
	}
	//close output file
	fclose(f);
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//unmap the memory to free the memory, then close the fileDescriptor
	if (munmap(map,mystat.st_size) == -1){
		close(fileDescriptor);
		perror("unmap error");
		exit(1);
	} 
	close(fileDescriptor);
	return 0;
}



