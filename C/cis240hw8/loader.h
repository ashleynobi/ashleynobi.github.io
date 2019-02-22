/*
*loader.h
*/
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int open(unsigned short theLC4[], char* name, unsigned short theRegion[]);
int readSection(int hasAddr, int parseData, int hasN, int isCode, FILE *thefile);
int inputEnds(int n, FILE *thefile);
int parseDatafunc(unsigned short theLC4[], int n, int mem_addr, FILE *thefile, int isCode, unsigned short theRegion[]);