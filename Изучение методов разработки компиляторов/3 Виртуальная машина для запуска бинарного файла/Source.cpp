#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "commands.h"

#define MEM_SIZE	

using namespace std;

const char *InputFileName;
FILE *InputFile;
unsigned char *memory;

union STATUS_REGISTER
{
	unsigned char reg;
	struct
	{
		unsigned char Z : 1;
		unsigned char N : 1;
		unsigned char C : 1;
		unsigned char T : 1;
	};
}SREG;

typedef union REGISTER
{
	int r[2];
	struct
	{
		int eax, ebx;
	};
}REGISTER;

typedef union COMMAND
{

	unsigned short code;

	union
	{
		struct
		{
			unsigned short arg2 : 4;
			unsigned short arg1 : 4;
			unsigned short type2 : 4;
			unsigned short type1 : 4;
		};

		struct
		{
			unsigned short addr : 12;
			unsigned short jmp_type : 4;
		};
	};

}COMMAND;

REGISTER reg = { 0, 0 };


COMMAND
read_command()
{
	COMMAND cmd;

	if (!fread((void*)&cmd.code, 2, 1, InputFile))
	{
		cmd.code = 0x0000;
	}

	return cmd;
}

int
handle_command(COMMAND cmd)
{
	fpos_t fpos;


	if ((cmd.type1 & MASK_MOV) == CODE_MOV)
	{
		SREG.reg = 0x00;
		switch (cmd.type2)
		{
		case T2_MOV_R_R: cout << "MOV_R_R" << endl;
		{
			reg.r[cmd.arg1] = reg.r[cmd.arg2];
		} break;

		case T2_MOV_R_M: cout << "MOV_R_M" << endl;
		{
			reg.r[cmd.arg1] = *(int *)(&memory[read_command().code]);
		} break;

		case T2_MOV_M_R: cout << "MOV_M_R" << endl;
		{
			*(int *)(&memory[read_command().code]) = reg.r[cmd.arg2];
		} break;

		case T2_MOV_R_N: cout << "MOV_R_N" << endl;
		{
			reg.r[cmd.arg1] = ((int)read_command().code << 16) + read_command().code;
		} break;

		default:
			break;
		}
	}
	else if ((cmd.type1 & MASK_JMP) == CODE_JMP)
	{
		int cmd_addr = cmd.addr;
		fpos = *(fpos_t*)(&cmd_addr);

		switch (cmd.type1)
		{
		case T1_JE: cout << "JE" << endl;
		{
			if (SREG.Z) fsetpos(InputFile, &fpos);
		} break;

		case T1_JNE: cout << "JNE" << endl;
		{
			if (!SREG.Z) fsetpos(InputFile, &fpos);
		} break;

		case T1_JL: cout << "JL" << endl;
		{
			if (SREG.N != SREG.C) fsetpos(InputFile, &fpos);
		} break;

		case T1_JLE: cout << "JLE" << endl;
		{
			if (SREG.N != SREG.C || SREG.Z) fsetpos(InputFile, &fpos);
		} break;

		case T1_JG: cout << "JG" << endl;
		{
			if (SREG.N == SREG.C && !SREG.Z) fsetpos(InputFile, &fpos);
		} break;

		case T1_JGE: cout << "JGE" << endl;
		{
			if (SREG.N == SREG.C) fsetpos(InputFile, &fpos);
		} break;

		case T1_JMP: cout << "JMP" << endl;
		{
			fsetpos(InputFile, &fpos);
		} break;

		case T1_CALL: cout << "CALL" << endl;
		{
			cout << reg.eax << endl;
		} break;

		default:
			break;
		}
	}
	else if ((cmd.type1 & MASK_BIN) == CODE_BIN)
	{
		SREG.reg = 0x00;
		switch (cmd.type2)
		{
		case T2_CMP: cout << "CMP" << endl;
		{
			int res = reg.r[cmd.arg1] - reg.r[cmd.arg2];

			if (res == 0) SREG.Z = true;
			if (res < 0) SREG.N = true;
			if ((reg.r[cmd.arg1] < -1 && reg.r[cmd.arg2] > 0 && res > 0)
				|| (reg.r[cmd.arg1] > -1 && reg.r[cmd.arg2] < 0 && res < 0))
			{
				SREG.C = true;
			}

		} break;

		case T2_ADD: cout << "ADD" << endl;
		{
			reg.r[cmd.arg1] = reg.r[cmd.arg1] + reg.r[cmd.arg2];
		} break;

		case T2_SUB: cout << "SUB" << endl;
		{
			reg.r[cmd.arg1] = reg.r[cmd.arg1] - reg.r[cmd.arg2];
		} break;

		case T2_MLT: cout << "MLT" << endl;
		{
			reg.r[cmd.arg1] = reg.r[cmd.arg1] * reg.r[cmd.arg2];
		} break;

		case T2_DIV: cout << "DIV" << endl;
		{
			if (reg.r[cmd.arg2] == 0)
			{
				cout << "error divide by zero" << endl;
				return -1;
			}
			int tmp_reg = reg.r[cmd.arg1];

			reg.r[cmd.arg1] = tmp_reg / reg.r[cmd.arg2];
			reg.r[cmd.arg2] = tmp_reg % reg.r[cmd.arg2];

		} break;

		case T2_SHL: cout << "SHL" << endl;
		{
			reg.r[cmd.arg1] = reg.r[cmd.arg1] << reg.r[cmd.arg2];
		} break;

		case T2_SHR: cout << "SHR" << endl;
		{
			reg.r[cmd.arg1] = reg.r[cmd.arg1] >> reg.r[cmd.arg2];
		} break;

		case T2_AND: cout << "AND" << endl;
		{
			reg.r[cmd.arg1] = reg.r[cmd.arg1] & reg.r[cmd.arg2];
		} break;

		case T2_OR: cout << "OR" << endl;
		{
			reg.r[cmd.arg1] = reg.r[cmd.arg1] | reg.r[cmd.arg2];
		} break;

		case T2_XOR: cout << "XOR" << endl;
		{
			reg.r[cmd.arg1] = reg.r[cmd.arg1] ^ reg.r[cmd.arg2];
		} break;

		default:
			break;
		}
	}
	else if ((cmd.type1 & MASK_UNO) == CODE_UNO)
	{
	SREG.reg = 0x00;
	switch (cmd.type2)
		{
		case T2_TST: cout << "TST" << endl;
		{
			if (reg.r[cmd.arg2] == 0) SREG.Z = true;
			if (reg.r[cmd.arg2] < 0) SREG.N = true;
		} break;

		case T2_NEG: cout << "NEG" << endl;
		{
			reg.r[cmd.arg2] = -reg.r[cmd.arg2];
		} break;

		case T2_NOT: cout << "NOT" << endl;
		{
			reg.r[cmd.arg2] = ~reg.r[cmd.arg2];
		} break;

		default:
			break;
		}
	}
	else if (cmd.code == CMD_RET)
	{
		cout << endl << InputFileName << " done with exit code: " << reg.eax << "." << endl;
		return 1;
	}
	else
	{
		cout << "error command bytecode" << endl;
		return -1;
	}
	
	return 0;
}

int
main(int argc, const char *argv[])
{
	if (argc > 1)
	{
		InputFileName = argv[1];
		InputFile = fopen(InputFileName, "rb");

		if (!InputFile)
		{
			cout << "no such file" << endl;
			return -1;
		}
	}
	else return -1;

	memory = new unsigned char[4 * 128];
	for (int i = 0; i < 4*128; i++) memory[i] = 0;

	int ret_code;

	do
	{
		ret_code = handle_command(read_command());
	}
	while (ret_code == 0);

	fclose(InputFile);
	delete[] memory;

	return 0;
}