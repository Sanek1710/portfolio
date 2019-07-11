#ifndef _COMMANDS_H_
#define _COMMANDS_H_


#define MASK_MOV	(0b1111)
#define MASK_JMP	(0b1000)
#define MASK_BIN	(0b1101)
#define MASK_UNO	(0b1101)

#define CODE_MOV	(0b0001)
#define CODE_JMP	(0b1000)
#define CODE_BIN	(0b0100)
#define CODE_UNO	(0b0101)

#define T2_MOV_R_R	(0b0000)
#define T2_MOV_R_M	(0b0001)
#define T2_MOV_M_R	(0b0010)
#define T2_MOV_R_N	(0b0011)

#define T1_JE 		(0b1000)
#define T1_JNE		(0b1001)
#define T1_JL 		(0b1010)
#define T1_JLE		(0b1011)
#define T1_JG 		(0b1100)
#define T1_JGE		(0b1101)
#define T1_JMP		(0b1110)
#define T1_CALL		(0b1111)


#define T2_CMP		(0b1111)
#define T2_ADD		(0b0000)
#define T2_SUB		(0b0001)
#define T2_MLT		(0b0010)
#define T2_DIV		(0b0011)
#define T2_SHL		(0b0100)
#define T2_SHR		(0b0101)
#define T2_AND		(0b1000)
#define T2_OR 		(0b1001)
#define T2_XOR		(0b1011)

#define T2_TST		(0b1111)
#define T2_NEG		(0b1100)
#define T2_NOT		(0b1101)

#define CMD_RET		(0b0000)



#endif // !_COMMANDS_H_