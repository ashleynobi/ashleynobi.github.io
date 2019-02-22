/*
* draftLC4.c
*/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "LC4.h"

#define MEMORY_SIZE 65536
#define OPCODE(I) ((I) >> 12)
#define B_0(I) ((I) & 0x1)
#define B_1(I) (((I) >> 1) & 0x1)
#define B_2(I) (((I) >> 2) & 0x1)
#define B_3(I) (((I) >> 3) & 0x1)
#define B_4(I) (((I) >> 4) & 0x1)
#define B_5(I) (((I) >> 5) & 0x1)
#define B_6(I) (((I) >> 6) & 0x1)
#define B_7(I) (((I) >> 7) & 0x1)
#define B_8(I) (((I) >> 8) & 0x1)
#define B_9(I) (((I) >> 9) & 0x1)
#define B_10(I) (((I) >> 10) & 0x1)
#define B_11(I) (((I) >> 11) & 0x1)
#define B_12(I) (((I) >> 12) & 0x1)
#define B_13(I) (((I) >> 13) & 0x1)
#define B_14(I) (((I) >> 14) & 0x1)
#define B_15(I) ((I) >> 15)
#define B_11_9(I) (((I) >> 9) & 0x7)
#define B_8_6(I) (((I) >> 6) & 0x7)
#define B_5_3(I) (((I) >> 3) & 0x7)
#define B_2_0(I) ((I) & 0x7)
#define B_5_0(I) ((I) & 0x3F)
#define B_8_0(I) ((I) & 0x1FF)
#define B_8_7(I) (((I) >> 7) & 0x3)
#define B_5_4(I) (((I) >> 4) & 0x3)
#define B_4_0(I) ((I) & 0x1F)
#define B_3_0(I) ((I) & 0xF)
#define B_7_0(I) ((I) & 0xFF)
#define B_6_0(I) ((I) & 0x7F)
#define B_10_0(I) ((I) & 0x7FF)





FILE *outfile;
unsigned short theLC4[MEMORY_SIZE];
unsigned short theRegion[MEMORY_SIZE];
unsigned short addr = 0;
unsigned short curr;
unsigned short int isCodeAgain = 0;
//unsigned short int RegFileWE;
//unsigned short int nzpWE;
//unsigned short int nzp;
//unsigned short int DataWE;
//unsigned short int dataMemAddr = 0;
//unsigned short int dataMemVal = 0;
unsigned short int b15;
unsigned short int b14;
unsigned short int b13;
unsigned short int b12;
unsigned short int b11;
unsigned short int b10;
unsigned short int b9;
unsigned short int b8;
unsigned short int b7;
unsigned short int b6;
unsigned short int b5;
unsigned short int b4;
unsigned short int b3;
unsigned short int b2;
unsigned short int b1;
unsigned short int b0;
unsigned short int pc;
unsigned short rd;
unsigned short rs;
unsigned short rt;
short imm5;
short imm6;
short imm7;
short imm9;
short imm11;
unsigned short uimm4;
unsigned short uimm7;
unsigned short uimm8;
unsigned short twoC;
unsigned short subop;
int pcModified = 0;
unsigned short zero = 0;
int exception = 0;
Machine *machine;


void convertInstructions(char *outfilename, unsigned short int theLC4[], unsigned short int theRegion[]){
	outfile = fopen(outfilename, "w");
	if(outfile == NULL){
		return;
	}

	machine = malloc(sizeof(Machine));

	machine->PC = 0x8200;
	machine->r0 = 0;
	machine->r1 = 0;
	machine->r2 = 0;
	machine->r3 = 0;
	machine->r4 = 0;
	machine->r5 = 0;
	machine->r6 = 0;
	machine->r7 = 0;
	machine->nzp = 2;
	machine->PSR = 1;


	while(machine->PC != 0x80FF){
		unsigned short i = machine->PC;
		curr = theLC4[i];
		setBits(curr);
		isCodeAgain = theRegion[i];

		if(isCodeAgain == 1){
			//printf("%d\n",isCodeAgain);
			if(isCodeAgain == 1){
				int opcode = OPCODE(curr);
				switch(opcode){
					case 0:
						branch(curr,i);
						break;
					case 1:
						arith(curr,i);
						break;
					case 5:
						logic(curr,i);
						break;
					case 6:
						machine->RegFileWE = 1;

						rd = B_11_9(curr);
						rs = B_8_6(curr);
						imm6 = B_5_0(curr);
						twoC = B_5(curr);
						if(twoC){
							imm6 = imm6 - 64;
						}
					
						modifyData(1);
						//fprintf(outfile, "%04hX : %04hX -> LDR R%d R%d %d\n",addr,curr,rd,rs,imm6);
						if((machine->dataMemAddr >= 0xA000) && machine->PSR != 1){
							fclose(outfile);
							free(machine);
							exit(1);
						}
						setRegister(rd, theLC4[(getRegister(rs) + imm6)]);
						setNZP(getRegister(rd));
						fprintf(outfile, "%04hX", machine->PC);
						fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
						fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
						break;
					case 7:
						machine->RegFileWE = 0;
						machine->nzpWE = 0;

						rs = B_8_6(curr);
						rt = B_11_9(curr);
						imm6 = B_5_0(curr);
						twoC = B_5(curr);
						if(twoC){
							imm6 = imm6 - 64;
						}

						modifyData(2);
						//fprintf(outfile, "%04hX : %04hX -> STR R%d R%d %d\n",addr,curr,rt,rs,imm6);
						if(machine->dataMemAddr >= 0xA000 && machine->PSR != 1){
							fclose(outfile);
							free(machine);
							exit(1);
						}

						theLC4[getRegister(rs) + imm6] = getRegister(rt);
						fprintf(outfile, "%04hX", machine->PC);
						fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
						fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
						break;
					case 9:
						machine->RegFileWE = 1;
						modifyData(0);

						rd = B_11_9(curr);
						imm9 = B_8_0(curr);
						twoC = B_8(curr);
						if(twoC){
							imm9 = imm9 - 512;
						}

						//fprintf(outfile, "%04hX : %04hX -> CONST R%d %d\n",addr,curr,rd,imm9);
						setRegister(rd,imm9);
						setNZP(getRegister(rd));
						fprintf(outfile, "%04hX", machine->PC);
						fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
						fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
						break;
					case 13:
						machine->RegFileWE = 1;
						modifyData(0);

						rd = B_11_9(curr);
						uimm8 = B_7_0(curr);
						//fprintf(outfile, "%04hX : %04hX -> HICONST R%d %d\n",addr,curr,rd,uimm8);
						setRegister(rd,(getRegister(rd) & 0xFF) | (uimm8 << 8));
						setNZP(getRegister(rd));
						fprintf(outfile, "%04hX", machine->PC);
						fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
						fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
						break;
					case 2:
						comparator(curr,i);
						break;
					case 10:
						shift(curr,i);
						break;
					case 4:
						jump_subroutine(curr,i);
						break;
					case 12:
						jump(curr,i);
						break;
					case 15:
						machine->RegFileWE = 1;
						modifyData(0);

						rd = 7;
						uimm8 = B_7_0(curr);
						//fprintf(outfile, "%04hX : %04hX -> TRAP %d\n",addr,curr,uimm8);
						setRegister(7, (machine->PC + 1));
						setNZP(getRegister(7));
						fprintf(outfile, "%04hX", machine->PC);
						fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
						fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
						modifyNextPC(0x8000 | uimm8);
						machine->PSR = 1;
						break;
					case 8:
						machine->RegFileWE = 0;
						machine->nzpWE = 0;
						modifyData(0);
				   		//fprintf(outfile, "%04hX : %04hX -> RTI\n",addr,curr);
						fprintf(outfile, "%04hX", machine->PC);
						fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
						fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
						modifyNextPC(getRegister(7));
						machine->PSR = 0;
				   		break;
				}	
			}else{
				//fprintf(outfile, "%04hX : %04hX\n",addr,curr);
				printf("data\n");
				fprintf(outfile, "%04hX 0000000000000000 0 0 0000 0 0 0 0000 0000",machine->PC);
			}
		} 

		if(pcModified == 0){
			(machine->PC)++;
		}

		pcModified = 0;
	}

	fclose(outfile);
	free(machine);
}

void setBits(unsigned short curr){
		b15 = B_15(curr);
		b14 = B_14(curr);
		b13 = B_13(curr);
		b12 = B_12(curr);
		b11 = B_11(curr);
		b10 = B_10(curr);
		b9 = B_9(curr);
		b8 = B_8(curr);
		b7 = B_7(curr);
		b6 = B_6(curr);
		b5 = B_5(curr);
		b4 = B_4(curr);
		b3 = B_3(curr);
		b2 = B_2(curr);
		b1 = B_1(curr);
		b0 = B_0(curr);
}

void setNZP(unsigned short val){
	machine->nzpWE = 1;
	twoC = B_15(val);
	if(twoC){
		machine->nzp = 4;
	}else{
		if(val == 0){
			machine->nzp = 2;
		}else{
			machine->nzp = 1;
		}
	}
}

unsigned short getRegister(unsigned short reg){
	unsigned short out;
	switch(reg){
		case 0:
			out = machine->r0;
			break;
		case 1:
			out = machine->r1;
			break;
		case 2:
			out = machine->r2;
			break;
		case 3:
			out = machine->r3;
			break;
		case 4:
			out = machine->r4;
			break;
		case 5:
			out = machine->r5;
			break;
		case 6:
			out = machine->r6;
			break;
		case 7:
			out = machine->r7;
			break;
	}

	return out;
}

void setRegister(unsigned short reg, unsigned short val){
	switch(reg){
		case 0:
			machine->r0 = val;
			break;
		case 1:
			machine->r1 = val;
			break;
		case 2:
			machine->r2 = val;
			break;
		case 3:
			machine->r3 = val;
			break;
		case 4:
			machine->r4 = val;
			break;
		case 5:
			machine->r5 = val;
			break;
		case 6:
			machine->r6 = val;
			break;
		case 7:
			machine->r7 = val;
			break;
	}
}

void modifyData(int data){
	switch(data){
		case 0:
			machine->dataWE = 0;
			machine->dataMemAddr = 0000;
			machine->dataMemVal = 0000;
			break;
		case 1:
			machine->dataWE = 0;
			machine->dataMemAddr = (getRegister(rs) + imm6);
			machine->dataMemVal = theLC4[(getRegister(rs) + imm6)];
			break;
		case 2:
			machine->dataWE = 1;
			machine->dataMemAddr = (getRegister(rs) + imm6);
			machine->dataMemVal = getRegister(rt);
			break;
	}
}

void modifyNextPC(unsigned short nextPC){
	machine->PC = nextPC;
	pcModified = 1;
}


void branch(unsigned short curr, int i){
	//PC: normal (next one based on last stored value and current condition)  [4d HEX]
	//Instruction: given [16d BINARY]
	//RegFile.WE: 0 [1d HEX]
	//Written to: 0 [1d HEX]
	//Value written:0000 [4d HEX]
	//NZP.WE: 0 [1d HEX]
	//Value to NZP:0 [1d HEX]
	//DATA.WE: 0 [1d HEX]
	//Data Mem Address: 0000 [4d HEX]
	//Value moved to/from memory: 0000 [4d HEX]
	machine->RegFileWE = 0;
	machine->nzpWE = 0;
	modifyData(0);

	imm9 = B_8_0(curr);
	subop = B_11_9(curr);
	twoC = B_8(curr);
	if(twoC){
		imm9 = imm9 - 512;
	}

	if(subop == 0){
		//fprintf(outfile, "%04hX : %04hX -> NOP\n",addr,curr);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 1){
		//fprintf(outfile, "%04hX : %04hX -> BRp %d\n",addr,curr,imm9);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		
		if(machine->nzp == 1){
			modifyNextPC((machine->PC) + imm9 + 1);  
		}

	}else if(subop == 2){
		//fprintf(outfile, "%04hX : %04hX -> BRz %d\n",addr,curr,imm9);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		
		if(machine->nzp == 2){
			modifyNextPC((machine->PC) + imm9 + 1);		  
		}

	}else if(subop == 3){
		//fprintf(outfile, "%04hX : %04hX -> BRzp %d\n",addr,curr,imm9);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		
		if(machine->nzp == 2 | machine->nzp == 1){
			modifyNextPC((machine->PC) + imm9 + 1);
			  
		}


	}else if(subop == 4){
		//fprintf(outfile, "%04hX : %04hX -> BRn %d\n",addr,curr,imm9);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		
		if(machine->nzp == 4){
			modifyNextPC((machine->PC) + imm9 + 1);
			  
		}

	}else if(subop == 5){
		//fprintf(outfile, "%04hX : %04hX -> BRnp %d\n",addr,curr,imm9);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		
		if(machine->nzp == 4 | machine->nzp == 1){
			modifyNextPC((machine->PC) + imm9 + 1);
			  
		}

	}else if(subop == 6){
		//fprintf(outfile, "%04hX : %04hX -> BRnz %d\n",addr,curr,imm9);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		
		if(machine->nzp == 4  | machine->nzp == 2){
			modifyNextPC((machine->PC) + imm9 + 1);
			  
		}

	}else if(subop == 7){
		//fprintf(outfile, "%04hX : %04hX -> BRnzp %d\n",addr,curr,imm9);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		
		modifyNextPC((machine->PC) + imm9 + 1);
		  

	}else{
		//fprintf(outfile, "invalid command\n");
	}
}

void arith(unsigned short curr, int i){
	//PC: normal  [4d HEX]
	//Instruction: given [16d BINARY]
	//RegFile.WE: 1 [1d HEX]
	//Written to: B_11_9 (rd) [1d HEX]
	//Value written: B_8_6(rs) (arithmetic) B_2_0(rt) [4d HEX]
	//NZP.WE: 1 [1d HEX]
	//Value to NZP:  based on B_8_6(rs) (arithmetic) B_2_0(rt)  [HEX]
	//DATA.WE: 0 [1d HEX]
	//Data Mem Address: 0000 [4d HEX]
	//Value moved to/from memory: 0000 [4d HEX]
	machine->RegFileWE = 1;
	modifyData(0);

	rd = B_11_9(curr);
	rs = B_8_6(curr);
	rt = B_2_0(curr);
	imm5 = B_4_0(curr);
	b5 = B_5(curr);
	subop = B_5_3(curr);
	twoC = B_4(curr);
	if(twoC){
		imm5 = imm5 - 32;
	}

	if(subop == 0){
		//add
		//fprintf(outfile,"R%d  = %d + %d\n",rd,getRegister(rs),getRegister(rt));
		setRegister(rd,(getRegister(rs) + getRegister(rt)));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 1){
		//fprintf(outfile,"R%d  = %d * %d\n",rd,rs,rt);
		setRegister(rd,(getRegister(rs) * getRegister(rt)));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 2){
		//sub
		setRegister(rd,(getRegister(rs) - getRegister(rt)));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 3){
		//div
		setRegister(rd,(getRegister(rs) / getRegister(rt)));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else{
		if(b5 == 1){
			//add immediate
			setRegister(rd,(getRegister(rs) + imm5));
			setNZP(getRegister(rd));
			fprintf(outfile, "%04hX", machine->PC);
			fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
			fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		}else{
			//fprintf(outfile, "invalid command\n");
		}
	}
}

void logic(unsigned short curr, int i){
	//machine->PC: normal  [4d HEX]
	//Instruction: given [16d BINARY]
	//RegFile.WE: 1 [1d HEX]
	//Written to: B_11_9 (rd) [1d HEX]
	//Value written: B_8_6(rs) (logic) B_2_0(rt) [4d HEX]
	//NZP.WE: 1 [1d HEX]
	//Value to NZP:  based B_8_6(rs) (logic) B_2_0(rt) [1d HEX]
	//DATA.WE: 0 [1d HEX]
	//Data Mem Address: 0000 [4d HEX]
	//Value moved to/from memory: 0000 [4d HEX]
	machine->RegFileWE = 1;
	modifyData(0);

	rd = B_11_9(curr);
	rs = B_8_6(curr);
	rt = B_2_0(curr);
	imm5 = B_4_0(curr);
	b5 = B_5(curr);
	subop = B_5_3(curr);
    twoC = B_4(curr);
	if(twoC){
		imm5 = imm5 - 32;
	}

	if(subop == 0){
		//and
		setRegister(rd,(getRegister(rs) & getRegister(rt)));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 1){
		//not
		setRegister(rd,~(getRegister(rs)));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 2){
		//or
		setRegister(rd,(getRegister(rs) | getRegister(rt)));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 3){
		//xor
		setRegister(rd,(getRegister(rs) ^ getRegister(rt)));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else{
		if(b5 == 1){
			//and imm
			setRegister(rd,(getRegister(rs) & imm5));
			setNZP(getRegister(rd));
			fprintf(outfile, "%04hX", machine->PC);
			fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
			fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		}else{
			//fprintf(outfile, "invalid command\n");
		}
	}
}

void comparator(unsigned short curr, int i){
	//machine->PC: normal  [4d HEX]
	//Instruction: given [16d BINARY]
	//RegFile.WE: 1 [1d HEX]
	//Written to: B_11_9 (rd) [1d HEX]
	//Value written: instruction specific [4d HEX]
	//NZP.WE: 1 [1d HEX]
	//Value to NZP:  based on value written [HEX]
	//DATA.WE: 0 [1d HEX]
	//Data Mem Address: 0000 [4d HEX]
	//Value moved to/from memory: 0000 [4d HEX]
	machine->RegFileWE = 0;
	modifyData(0);

	rd = 0;
	  
	rs = B_11_9(curr);
	rt = B_2_0(curr);
	imm7 = B_6_0(curr);
	uimm7 = B_6_0(curr);
	subop = B_8_7(curr);
	twoC = B_6(curr);
	if(twoC){
		imm7 = imm7 - 128;
	}

	if(subop == 0){
		//CMP
		//if(B_15(getRegister(rs)))
		setNZP(getRegister(rs) - getRegister(rt));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 1){
		//CMPU
		setNZP(getRegister(rs) - getRegister(rt));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 2){
		//CMPI 
		setNZP(getRegister(rs) - imm7);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 3){
		//CMPIU
		setNZP(getRegister(rs) - uimm7);
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else{ 
		//fprintf(outfile, "invalid command\n");
	}
}

void shift(unsigned short curr, int i){
	//machine->PC: normal  [4d HEX]
	//Instruction: given [16d BINARY]
	//RegFile.WE: 1 [1d HEX]
	//Written to: B_11_9 (rd) [1d HEX]
	//Value written: B_8_6(rs) (shift) B_2_0(rt) [4d HEX]
	//NZP.WE: 1 [1d HEX]
	//Value to NZP:  B_8_6(rs) (shift) B_2_0(rt)  [HEX]
	//DATA.WE: 0 [1d HEX]
	//Data Mem Address: 0000 [4d HEX]
	//Value moved to/from memory: 0000 [4d HEX]
	machine->RegFileWE = 1;
	modifyData(0);

	rd = B_11_9(curr);
	rs = B_8_6(curr);
	rt = B_2_0(curr);
	uimm4 = B_3_0(curr);
	b5 = B_5(curr);
	subop = B_5_4(curr);
	
	if(subop == 0){
		//SLL
		setRegister(rd,(getRegister(rs) << uimm4));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 1){
		//SRA
		twoC = B_15(getRegister(rs));
		if(twoC){
			signed short tempval = (signed short) (getRegister(rs) - 65536);
			setRegister(rd,(tempval >> uimm4));
		}else{
			setRegister(rd,(getRegister(rs) >> uimm4));
		}
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else if(subop == 2){
		//SRL
		setRegister(rd,(getRegister(rs) >> uimm4));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
	}else{
		if(b5 == 1){
			//MOD
			setRegister(rd,(getRegister(rs) % getRegister(rt)));
			setNZP(getRegister(rd));
			fprintf(outfile, "%04hX", machine->PC);
			fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
			fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		}else{
			fprintf(outfile, "invalid command\n");
		}
	}
}

void jump_subroutine(unsigned short curr, int i){
	//PC: normal  [4d HEX]
	//Instruction: given [16d BINARY]
	//RegFile.WE: 1 [1d HEX]
	//Written to: B_11_9 (rd) [1d HEX]
	//Value written: B_8_6(rs) (arithmetic) B_2_0(rt) [4d HEX]
	//NZP.WE: 1 [1d HEX]
	//Value to NZP:  B_8_6(rs) (arithmetic) B_2_0(rt)  [HEX]
	//DATA.WE: 0 [1d HEX]
	//Data Mem Address: 0000 [4d HEX]
	//Value moved to/from memory: 0000 [4d HEX]
	machine->RegFileWE = 1;
	machine->nzpWE = 0;
	modifyData(0);

	rd = 7;
	  
	rs = B_8_6(curr);
	unsigned short tempval = getRegister(rs);
	imm11 = B_10_0(curr);
	subop = B_11(curr);
	twoC = B_10(curr);
	if(twoC){
		imm11 = imm11 - 2048;
	}

	if(subop == 0){
		//JSRR
		setRegister(rd, (machine->PC+ 1));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		modifyNextPC(tempval);
		  
	}else if(subop == 1){
		//JSR
		setRegister(rd, (machine->PC + 1));
		setNZP(getRegister(rd));
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,rd,getRegister(rd),machine->nzpWE,machine->nzp,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		modifyNextPC(((machine->PC & 0x8000) | imm11 << 4));
		  
	}else{
		fprintf(outfile, "invalid command\n");
	}
}

void jump(unsigned short curr, int i){
	//PC: normal  [4d HEX]
	//Instruction: given [16d BINARY]
	//RegFile.WE: 1 [1d HEX]
	//Written to: B_11_9 (rd) [1d HEX]
	//Value written: B_8_6(rs) (arithmetic) B_2_0(rt) [4d HEX]
	//NZP.WE: 1 [1d HEX]
	//Value to NZP:  B_8_6(rs) (arithmetic) B_2_0(rt)  [HEX]
	//DATA.WE: 0 [1d HEX]
	//Data Mem Address: 0000 [4d HEX]
	//Value moved to/from memory: 0000 [4d HEX]
	machine->RegFileWE = 0;
	machine->nzpWE = 0;
	modifyData(0);

	rs = B_8_6(curr);
	imm11 = B_10_0(curr);
	subop = B_11(curr);
	twoC = B_10(curr);

	if(twoC){
		imm11 = imm11 - 2048;
	}
		
	if(subop == 0){
		//JMPR
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		modifyNextPC(getRegister(rs));
		  
	}else if(subop == 1){
		//JMP
		fprintf(outfile, "%04hX", machine->PC);
		fprintf(outfile, " %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0);
		fprintf(outfile, " %hX %hX %04hX %hX %hX %hX %04hX %04hX\n",machine->RegFileWE,zero,zero,machine->nzpWE,zero,machine->dataWE,machine->dataMemAddr,machine->dataMemVal);
		modifyNextPC((machine->PC) + (imm11 + 1));
		  
	}else{
		fprintf(outfile, "invalid command\n");
	}
}
