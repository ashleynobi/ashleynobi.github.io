/*
*parser.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "loader.h"
#include "LC4.h"

#define MEMORY_SIZE 65536
unsigned short int theLC4[MEMORY_SIZE];
unsigned short int theRegion[MEMORY_SIZE];
char *fname;
char *oname;

void initMemoryArray(unsigned short int arr[], int size){
	for(int i = 0; i < size; i++){
		arr[i] = 0;
	}
}

int main(int argc, char** argv){
	int open_unsucessful;
	initMemoryArray(theLC4,MEMORY_SIZE);
	initMemoryArray(theRegion,MEMORY_SIZE);
	while(1){
		for(int i = 2; i < argc; i++){
			oname = argv[1];
			fname = argv[i];
			open_unsucessful = open(theLC4,fname,theRegion);
			if(!open_unsucessful){
				convertInstructions(oname, theLC4,theRegion);
			}else{
				return 1;
			}
		}

		return 0;
	}

	return 0;
}
