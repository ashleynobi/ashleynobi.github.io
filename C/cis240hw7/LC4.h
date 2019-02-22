/*
* LC4.h
*/
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void convertInstructions(char *outputfilename,unsigned short int arr[], unsigned short int theRegion[]);
void branch(unsigned short curr, int i);
void arith(unsigned short curr, int i);
void logic(unsigned short curr, int i);
void comparator(unsigned short curr, int i);
void shift(unsigned short curr, int i);
void jump_subroutine(unsigned short curr, int i);
void jump(unsigned short curr, int i);
