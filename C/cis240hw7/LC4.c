/*
* draftLC4.c
*/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "LC4.h"

#define MEMORY_SIZE 65536
#define OPCODE(I) ((I) >> 12)
#define B_11_9(I) (((I) >> 9) & 0x7)
#define B_8_6(I) (((I) >> 6) & 0x7)
#define B_5_3(I) (((I) >> 3) & 0x7)
#define B_2_0(I) ((I) & 0x7)
#define B_5_0(I) ((I) & 0x3F)
#define B_8_0(I) ((I) & 0x1FF)
#define B_5(I) (((I) >> 5) & 0x1)
#define B_8_7(I) (((I) >> 7) & 0x3)
#define B_11(I) (((I) >> 11) & 0x1)
#define B_5_4(I) (((I) >> 4) & 0x3)
#define B_4_0(I) ((I) & 0x1F)
#define B_3_0(I) ((I) & 0xF)
#define B_7_0(I) ((I) & 0xFF)
#define B_6_0(I) ((I) & 0x7F)
#define B_10_0(I) ((I) & 0x7FF)
#define B_15(I) ((I) >> 15)
#define B_10(I) (((I) >> 10) & 0x1)
#define B_8(I) (((I) >> 8) & 0x1)
#define B_4(I) (((I) >> 4) & 0x1)
#define B_6(I) (((I) >> 6) & 0x1)

FILE *outfile;
unsigned short theLC4[MEMORY_SIZE];
unsigned short theRegion[MEMORY_SIZE];
unsigned short addr = 0;
unsigned short curr;
unsigned short int isCodeAgain = 0;

void convertInstructions(char *outfilename, unsigned short int theLC4[], unsigned short int theRegion[]){

	outfile = fopen(outfilename, "w");
	if(outfile == NULL){
		return;
	}


	for(int i = 0; i < MEMORY_SIZE; i++){
		curr = theLC4[i];
		isCodeAgain = theRegion[i];

		unsigned short rd;
		unsigned short rs;
		unsigned short rt;
		short imm6;
		short imm9;
		unsigned short uimm8;
		unsigned short twoC;

		if(curr != 0){
			printf("%d\n",isCodeAgain);
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
						rd = B_11_9(curr);
						rs = B_8_6(curr);
						imm6 = B_5_0(curr);
						twoC = B_5(curr);
						if(twoC){
							imm6 = imm6 - 512;
						}
					
						fprintf(outfile, "%04hX : %04hX -> LDR R%d R%d %d\n",addr,curr,rd,rs,imm6);
						break;
					case 7:
						rs = B_8_6(curr);
						rt = B_11_9(curr);
						imm6 = B_5_0(curr);
						twoC = B_5(curr);
						if(twoC){
							imm6 = imm6 - 512;
						}
						fprintf(outfile, "%04hX : %04hX -> STR R%d R%d %d\n",addr,curr,rt,rs,imm6);
						break;
					case 9:
						rd = B_11_9(curr);
						imm9 = B_8_0(curr);
						twoC = B_8(curr);
						if(twoC){
							imm9 = imm9 - 512;
						}

						fprintf(outfile, "%04hX : %04hX -> CONST R%d %d\n",addr,curr,rd,imm9);
						break;
					case 13:
						rd = B_11_9(curr);
						uimm8 = B_7_0(curr);
						fprintf(outfile, "%04hX : %04hX -> HICONST R%d %d\n",addr,curr,rd,uimm8);
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
						uimm8 = B_7_0(curr);
						fprintf(outfile, "%04hX : %04hX -> TRAP %d\n",addr,curr,uimm8);
						break;
					case 8:
				   		fprintf(outfile, "%04hX : %04hX -> RTI\n",addr,curr);
				   		break;
				}	
			}else{
				fprintf(outfile, "%04hX : %04hX\n",addr,curr);
			}
		} 

		addr++;
	}
}

void branch(unsigned short curr, int i){
	short imm9 = B_8_0(curr);
	unsigned short subop = B_11_9(curr);
	unsigned short twoC = B_8(curr);
	if(twoC){
		imm9 = imm9 - 512;
	}

	if(subop == 0){
		fprintf(outfile, "%04hX : %04hX -> NOP\n",addr,curr);
	}else if(subop == 1){
		fprintf(outfile, "%04hX : %04hX -> BRp %d\n",addr,curr,imm9);
	}else if(subop == 2){
		fprintf(outfile, "%04hX : %04hX -> BRz %d\n",addr,curr,imm9);
	}else if(subop == 3){
		fprintf(outfile, "%04hX : %04hX -> BRzp %d\n",addr,curr,imm9);
	}else if(subop == 4){
		fprintf(outfile, "%04hX : %04hX -> BRn %d\n",addr,curr,imm9);
	}else if(subop == 5){
		fprintf(outfile, "%04hX : %04hX -> BRnp %d\n",addr,curr,imm9);
	}else if(subop == 6){
		fprintf(outfile, "%04hX : %04hX -> BRnz %d\n",addr,curr,imm9);
	}else if(subop == 7){
		fprintf(outfile, "%04hX : %04hX -> BRnzp %d\n",addr,curr,imm9);
	}else{
		fprintf(outfile, "invalid command\n");
	}

}

void arith(unsigned short curr, int i){
	unsigned short rd = B_11_9(curr);
	unsigned short rs = B_8_6(curr);
	unsigned short rt = B_2_0(curr);
	short imm5 = B_4_0(curr);
	unsigned short b5 = B_5(curr);
	unsigned short subop = B_5_3(curr);
	unsigned short twoC = B_4(curr);
	if(twoC){
		imm5 = imm5 - 32;
	}

	if(subop == 0){
		fprintf(outfile, "%04hX : %04hX -> ADD R%d R%d R%d\n",addr,curr,rd,rs,rt);
	}else if(subop == 1){
		fprintf(outfile, "%04hX : %04hX -> MUL R%d R%d R%d\n",addr,curr,rd,rs,rt);
	}else if(subop == 2){
		fprintf(outfile, "%04hX : %04hX -> SUB R%d R%d R%d\n",addr,curr,rd,rs,rt);
	}else if(subop == 3){
		fprintf(outfile, "%04hX : %04hX -> DIV R%d R%d R%d\n",addr,curr,rd,rs,rt);
	}else{
		if(b5 == 1){
			fprintf(outfile, "%04hX : %04hX -> ADD R%d R%d %d\n",addr,curr,rd,rs,imm5);
		}else{
				fprintf(outfile, "invalid command\n");
		}
	}
	
}

void logic(unsigned short curr, int i){
	unsigned short rd = B_11_9(curr);
	unsigned short rs = B_8_6(curr);
	unsigned short rt = B_2_0(curr);
	short imm5 = B_4_0(curr);
	unsigned short b5 = B_5(curr);
	unsigned short subop = B_5_3(curr);
	unsigned short twoC = B_4(curr);
	if(twoC){
		imm5 = imm5 - 32;
	}

	if(subop == 0){
		fprintf(outfile, "%04hX : %04hX -> AND R%d R%d R%d\n",addr,curr,rd,rs,rt);
	}else if(subop == 1){
		fprintf(outfile, "%04hX : %04hX -> NOT R%d R%d\n",addr,curr,rd,rs);
	}else if(subop == 2){
		fprintf(outfile, "%04hX : %04hX -> OR R%d R%d R%d\n",addr,curr,rd,rs,rt);
	}else if(subop == 3){
		fprintf(outfile, "%04hX : %04hX -> XOR R%d R%d R%d\n",addr,curr,rd,rs,rt);
	}else{
		if(b5 == 1){
			fprintf(outfile, "%04hX : %04hX -> AND R%d R%d %d\n",addr,curr,rd,rs,imm5);
		}else{
			fprintf(outfile, "invalid command\n");
		}
	}
	
}

void comparator(unsigned short curr, int i){
	unsigned short rs = B_11_9(curr);
	unsigned short rt = B_2_0(curr);
	short imm7 = B_6_0(curr);
	unsigned short uimm7 = B_6_0(curr);
	unsigned short subop = B_8_7(curr);
	unsigned short twoC = B_6(curr);
	if(twoC){
		imm7 = imm7 - 128;
	}

	if(subop == 0){
		fprintf(outfile, "%04hX : %04hX -> CMP R%d R%d\n",addr,curr,rs,rt);
	}else if(subop == 1){
		fprintf(outfile, "%04hX : %04hX -> CMPU R%d R%d\n",addr,curr,rs,rt);
	}else if(subop == 2){
		fprintf(outfile, "%04hX : %04hX -> CMPI R%d %d\n",addr,curr,rs,imm7);
	}else if(subop == 3){
		fprintf(outfile, "%04hX : %04hX -> CMPIU R%d %d\n",addr,curr,rs,uimm7);
	}else{
		fprintf(outfile, "invalid command\n");
	}
}

void shift(unsigned short curr, int i){
	unsigned short rd = B_11_9(curr);
	unsigned short rs = B_8_6(curr);
	unsigned short rt = B_2_0(curr);
	unsigned short uimm4 = B_3_0(curr);
	unsigned short b5 = B_5(curr);
	unsigned short subop = B_5_4(curr);
	
	if(subop == 0){
		fprintf(outfile, "%04hX : %04hX -> SLL R%d R%d %d\n",addr,curr,rd,rs,uimm4);
	}else if(subop == 1){
		fprintf(outfile, "%04hX : %04hX -> SRA R%d R%d %d\n",addr,curr,rd,rs,uimm4);
	}else if(subop == 2){
		fprintf(outfile, "%04hX : %04hX -> SRL R%d R%d %d\n",addr,curr,rd,rs,uimm4);
	}else{
		if(b5 == 1){
			fprintf(outfile, "%04hX : %04hX -> MOD R%d R%d R%d\n",addr,curr,rd,rs,rt);
		}else{
			fprintf(outfile, "invalid command\n");
		}
	}
	
}

void jump_subroutine(unsigned short curr, int i){
	unsigned short rs = B_8_6(curr);
	short imm11 = B_10_0(curr);
	unsigned short subop = B_11(curr);
	unsigned short twoC = B_10(curr);
	if(twoC){
		imm11 = imm11 - 2048;
	}

	if(subop == 0){
		fprintf(outfile, "%04hX : %04hX -> JSRR R%d\n",addr,curr,rs);
	}else if(subop == 1){
		fprintf(outfile, "%04hX : %04hX -> JSR %d\n",addr,curr, imm11);
	}else{
		fprintf(outfile, "invalid command\n");
	}
	

	
}

void jump(unsigned short curr, int i){
	unsigned short rs = B_8_6(curr);
	short imm11 = B_10_0(curr);
	unsigned short subop = B_11(curr);
	unsigned short twoC = B_10(curr);
	if(twoC){
		imm11 = imm11 - 2048;
	}
		
	if(subop == 0){
		fprintf(outfile, "%04hX : %04hX -> JMPR R%d\n",addr,curr,rs);
	}else if(subop == 1){
		fprintf(outfile, "%04hX : %04hX -> JMP %d\n",addr,curr, imm11);
	}else{
		fprintf(outfile, "invalid command\n");
	}
	
}
