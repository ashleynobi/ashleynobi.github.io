/*
*loader.h
*/
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int open(unsigned short int theLC4[], char* name, unsigned short int theRegion[]);
int readSection(int hasAddr, int parseData, int hasN, unsigned short int isCode, FILE *infile);
int inputEnds(int n, FILE *infile);
int parseDatafunc(unsigned short int theLC4[], int n, int mem_addr, FILE *thefile, unsigned short int isCode, unsigned short int theRegion[]);