/*
* LC4.h
*/
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

typedef struct MachineState_tag{
	unsigned short PC;
	unsigned short r0;
	unsigned short r1;
	unsigned short r2;
	unsigned short r3;
	unsigned short r4;
	unsigned short r5;
	unsigned short r6;
	unsigned short r7;
	unsigned short RegFileWE;
	unsigned short nzpWE; 
	unsigned short nzp;
	unsigned short dataWE;
	unsigned short dataMemAddr;
	unsigned short dataMemVal;
	unsigned short PSR;
} Machine;

void convertInstructions(char *outputfilename,unsigned short arr[], unsigned short theRegion[]);
void setBits(unsigned short curr);
void setNZP(unsigned short val);
unsigned short getRegister(unsigned short reg);
void setRegister(unsigned short reg, unsigned short val);
void modifyData(int data);
void modifyNextPC(unsigned short offset);
void branch(unsigned short curr, int i);
void arith(unsigned short curr, int i);
void logic(unsigned short curr, int i);
void comparator(unsigned short curr, int i);
void shift(unsigned short curr, int i);
void jump_subroutine(unsigned short curr, int i);
void jump(unsigned short curr, int i);
