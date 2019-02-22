/*
*loader.c
*/
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "loader.h"

#define MEMORY_SIZE 65536
#define COMBINE(I1,I2) (((I1) << 8) | I2)
#define END_BYTES(I1,I2) (I1 == 0xFFFF || I2 == 0xFFFF)

FILE *thefile;
unsigned short int curr;
unsigned short int next;
unsigned short int head;
int testFails;
unsigned short int theLC4[MEMORY_SIZE];
unsigned short int theRegion[MEMORY_SIZE];

int open(unsigned short int theLC4[], char* name, unsigned short int theRegion[]){
	thefile = fopen(name, "r");

	if(thefile == NULL){
		return 1;
	}

	while(feof(thefile) == 0){
		curr = fgetc(thefile);
		next = fgetc(thefile);

		if(END_BYTES(curr,next) || feof(thefile)){
			break;
		}
		
		head = COMBINE(curr,next);

		if(head == 0xCADE){
			testFails = readSection(1,1,1,1,thefile);
		}else if(head == 0xDADA){
			testFails = readSection(1,1,1,0,thefile);
		}else if(head == 0xC3B7){
			testFails = readSection(1,0,1,0,thefile);
		}else if(head == 0xF17E){
			testFails = readSection(0,0,1,0,thefile);
		}else if(head == 0x715E){
			testFails = readSection(0,0,0,0,thefile);
		}else{
			testFails = 1;
		}

		if(testFails == 1){
			fclose(thefile);
			return 1;
		}
	}

	fclose(thefile);
	return 0;
}

int readSection(int hasAddr, int parseData, int hasN, unsigned short int isCode, FILE *thefile){
	int n;
	int mem_addr;

	if(hasAddr == 1 && hasN == 1){
		//get address
		curr = fgetc(thefile);
		next = fgetc(thefile);

		if(END_BYTES(curr,next)){
			return 1;
		} 
		mem_addr = COMBINE(curr,next);

		//get n
		curr = fgetc(thefile);
		next = fgetc(thefile);

		if(END_BYTES(curr,next)){
			return 1;
		} 

		n = COMBINE(curr,next);

		if(parseData == 1){
			testFails = parseDatafunc(theLC4, n, mem_addr,thefile,isCode,theRegion);
			if(testFails == 1){
				return 1;
			}
		}else{
			testFails = inputEnds(n, thefile);
			if(testFails == 1){
				return 1;
			}
		}

	}
	if(hasAddr == 0 && hasN == 1){
		//get n
		curr = fgetc(thefile);
		next = fgetc(thefile);

		if(END_BYTES(curr,next)){
			return 1;
		} 

		n = COMBINE(curr,next);
		testFails = inputEnds(n, thefile);

		if(testFails  == 1){
			return 1;
		}
	}
	if(hasAddr == 0 && hasN == 0){
		testFails = inputEnds(6, thefile);

		if(testFails  == 1){
			return 1;
		}
	}

	return 0;
}

int inputEnds(int n, FILE *thefile){
	for(int i = 0; i < n;i++){
		curr = fgetc(thefile);
		if(curr == 0xFFFF){
			return 1;
		}
	}

	return 0;
}

int parseDatafunc(unsigned short int theLC4[], int n, int mem_addr, FILE *thefile, unsigned short int isCode, unsigned short int theRegion[]){
	unsigned short int ins;

	for(int i = 0; i < n;i++){
		curr = fgetc(thefile);
		next = fgetc(thefile);

		if(END_BYTES(curr,next)){
			return 1;
		}

		ins = COMBINE(curr,next);
		theLC4[mem_addr] = ins;
		//printf("%d\n", mem_addr);
		theRegion[mem_addr] = isCode;
		mem_addr++;
	}

	return 0;
}
