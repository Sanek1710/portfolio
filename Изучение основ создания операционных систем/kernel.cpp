extern "C" int kmain();
__declspec(naked) void
startup()
{
	__asm
	{
		call kmain;
	}
}

//_CONSOLE DEFINES
#define VID_BUF_HEIGHT		(25)
#define VID_BUF_WIDTH		(80)
#define VID_BUF_SIZE		VID_BUF_HEIGHT * VID_BUF_WIDTH

#define VIDEO_BUF_PTR		(0xB8000)
#define VIDEO_BUF_END		(0xB8000 + 2 * VID_BUF_SIZE) 

#define IOBUF				(0xC6000)
#define IOBUF_SIZE			(0x00500)

int InputBuf = 0; //temp buffer of input
char *OutputBuf = (char*)(IOBUF + IOBUF_SIZE);	//Virtual output buffer

//last usable address of OutputBuf is (0xC7FFF) 
//at 0xC8000 virtual video buf ends

//_INTERRUPT DEFINES
#define IDT_TYPE_INTR		(0x0E)
#define IDT_TYPE_TRAP		(0x0F)
#define GDT_CS				(0x08)		//селектор секции кода установленный загрузчиком ОС
#define PIC1_PORT			(0x20)		//контроллер прерываний 
#define	CURSOR_PORT			(0x3D4)


#pragma region _CURSOR

struct Cursor
{
	int x;
	int y;
	int vision;
}cursor;

void cursor_setpos(int x, int y); //set position of cursor
void cursor_moveto(); //move cursor to set position
void cursor_forward(); //move cursor forward
void cursor_backward(); //--//--
void cursor_new_line(); //move cursor on next line
void cursor_vision(int vision); //set vision of cursor 1 - visible, 0 - unvisible
int  cursor_get_pos(); //get absolute position of cursor (y * wide + x)

#pragma endregion

#pragma region _STRING
//base string functions
int 
strlen(const char *str);

void 
strrev(char *str);

int
strcmp(const char * str1, const char * str2);

void
strcpy(char *dst, const char *src);

#pragma endregion

#pragma region _CONSOLE
//oh its structure due to console symbol representation
// [4 bits] [4 bits] [8 bits]
//  backclr  textclr  symbol
#pragma pack(push, 1) //turn of pack
struct cchar //console char
{
	unsigned char symbol;
	unsigned char color;
};
#pragma pack(pop) //turn on pack

cchar * const
ConsoleBuf = (cchar*)VIDEO_BUF_PTR; //console buf, all in this buf translate to console instant
unsigned char CONSOLE_COLOR = 0b10001111; //default console colour

int ConsoleInputSema = 0; //semaphore of key bottom pressed, and pressed symbol is ready
void //obviously
sema_up(int *sema)
{
	(*sema)++;
}
void 
sema_down(int *sema)
{
	while (*sema == 0);
	(*sema)--;
}

void 
clear(); //clear all console (just fill them with ' ')

int 
putc(int c); //put one symbol c to console

int 
puts(const char *str); //put string str of symbols to console


int 
getc(); //get char from input of console (it use semaphor waiting key pressed)

int 
gets(char *str); //same

void
print_number(int n); //print number to console

#pragma endregion

#pragma region _INTERRUPT

typedef void(*intr_handler) ();

#pragma pack(push, 1)
struct idt_entry //check manual
{
	unsigned short base_lo;
	unsigned short segm_sel;
	unsigned char always0;
	unsigned char flags;
	unsigned short base_hi;
};
struct idt_ptr
{
	unsigned short limit;
	unsigned int base;
};
#pragma pack(pop)

struct idt_entry	g_idt[256];
struct idt_ptr		g_idtp;

__declspec(naked) void
default_intr_handler()
{
	__asm pusha;
	//handing
	__asm
	{
		popa
		iretd
	}
}

void //register handler to interrupt
intr_reg_handler(int num, unsigned short segm_sel, unsigned short flags, intr_handler hndlr);

void //initialize interrupts
intr_init();

void 
intr_start();

void 
intr_enable();

void 
intr_disable();

__inline unsigned char //read data from port
inb(unsigned short port);

__inline void //write data to port
outb(unsigned short port, unsigned char data);

#pragma endregion

#pragma region _KEYBOARD

int Symbols[256] = { 0 };

void 
symbol_init(); //initializarion symbols due them scan code for example 'a' has scan code 30, 's' - 31, 'd' - 32 and so on

void 
keyb_init(); //init keyboard interrupt

char SHFT = 0, CAPS = 0; //shift and capslock pushed flags 

void 
keyb_process_key(); //write symbol to inputbuf and release semaphore that says data is ready

__declspec(naked) void //handler for keyboard press (or release) call keyb_process_key
keyb_handler()
{
	__asm pusha;

	keyb_process_key();

	outb(PIC1_PORT, 0x20);
	__asm
	{
		popa
		iretd
	}
}

#pragma endregion


#pragma region _SOLVER_OS

#define INTMAX	(int)(2147483647)
#define NULL	0

int
FLAG_OVERFLOW = 0,
FLAG_EVAL_ERROR = 0,
FLAG_CMD_ERROR = 0,
FLAG_DECIMAL = 1;


void
clear_flags();

void
print_errors();

int
gcd(int a, int b);

int
lcm(int a, int b);

//in 32 kernel mode we have no double or float so we use our own representation of fraction
struct fraction
{ 
	int sgn = 1;
	int numerator = 0;
	int denominator = 0;
};

fraction
divf(int a, int b);

void
print_decimal(fraction fr);

void
print_fraction(fraction fr);

int
Sum(int a, int b);

fraction
solve(int a, int b, int c);


//split str into 2 part: left and lost using separator symbol (for ex: ' ')
//it changes src string, put symbols into left str and returns pointer to lost str or NULL if there is no separator
//example: 
//buf = "gcd 12 15"
//split(left, ' ', buf) ->
//  buf = "gcd 12 15"
//  left = "gcd"
//  returns: pointer to "12 15" in buf str
const char *
split(char *left, int separator, const char *str);

int //atoi 80 lvl that can signalize us about errors
atoi(const char *str);

void
parse_command(const char *scmd);

void
chose_color();

#pragma endregion

__inline void //shutdown is out 0x2000 to 0x0604 port
shutdown()
{
	__asm
	{
		push ax
		push dx
		mov dx, 0x0604
		mov ax, 0x2000
		out dx, ax
		pop dx
		pop ax
	}
}

extern "C" int kmain()
{
	symbol_init();

	cursor_vision(0);

	intr_init();
	keyb_init();
	intr_start();
	intr_enable();

	chose_color();


	clear_flags();

	cursor_vision(1);

	while (1)
	{
		puts("command> ");
		gets(buf);

		if (cursor.y >= VID_BUF_HEIGHT - 1)
		{
			clear();
		}

		parse_command(buf);
	}

	while (1)
	{
		__asm hlt;
	}

	return 0;
}





#pragma region _CURSOR

void cursor_setpos(int x, int y)
{
	if (0 <= y && y < VID_BUF_HEIGHT &&
		0 <= x && x < VID_BUF_WIDTH)
	{ 
		cursor.x = x;
		cursor.y = y;
	}
}
	 
void cursor_moveto()
{
	unsigned short new_pos = cursor.y * VID_BUF_WIDTH + cursor.x;

	if (cursor.vision)
	{
		outb(CURSOR_PORT, 0x0F);
		outb(CURSOR_PORT + 1, (unsigned char)(new_pos & 0xFF));
		outb(CURSOR_PORT, 0x0E);
		outb(CURSOR_PORT + 1, (unsigned char)((new_pos >> 8) & 0xFF));
	}
}
	 
void cursor_forward()
{
	if (cursor.x == VID_BUF_WIDTH - 1)
	{
		if (cursor.y != VID_BUF_HEIGHT - 1)
		{
			cursor.y++;
			cursor.x = 0;
		}
	}
	else cursor.x++;

	cursor_moveto();
}
	 
void cursor_backward()
{
	if (cursor.x == 0)
	{
		if (cursor.y != 0)
		{
			cursor.y--;
			cursor.x = VID_BUF_WIDTH - 1;
		}
	}
	else cursor.x--;

	cursor_moveto();
}
	 
void cursor_new_line()
{
	if (cursor.y != VID_BUF_HEIGHT - 1)
	{
		cursor.y++;
		cursor.x = 0;
	}
}
	 
void cursor_vision(int vision)
{
	cursor.vision = vision;
	if (vision == 0)
	{
		outb(CURSOR_PORT, 0x0F);
		outb(CURSOR_PORT + 1, (unsigned char)(2000 & 0xFF));
		outb(CURSOR_PORT, 0x0E);
		outb(CURSOR_PORT + 1, (unsigned char)((2000 >> 8) & 0xFF));
	}
	else cursor_moveto();
}

int  cursor_get_pos()
{
	return (cursor.y * VID_BUF_WIDTH + cursor.x);
}

#pragma endregion

#pragma region _STRING

int 
strlen(const char *str)
{
	const char *beg = str;
	while (*str++);
	return str - beg - 1;
}

void 
strrev(char *str)
{
	int len = strlen(str);

	for (int i = 0; i < len / 2; i++)
	{
		int t = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = t;
	}
}

int 
strcmp(const char * str1, const char * str2)
{
	while (*str1 && *str2 && (*str1 == *str2)) str1++, str2++;
	if (*str1 > *str2) return 1;
	if (*str1 < *str2) return -1;
	if (*str1 == *str2) return 0;
	return 0;
}

void
strcpy(char *dst, const char *src)
{
	while (*dst++ = *src++);
}

#pragma endregion

#pragma region _CONSOLE

void 
clear()
{
	for (int i = 0; i < VID_BUF_SIZE; i++)
	{
		ConsoleBuf[i].color = CONSOLE_COLOR;
		ConsoleBuf[i].symbol = 0;
	}

	cursor_setpos(0, 0);
	cursor_moveto();
}

int 
putc(int c)
{
	int pos = cursor_get_pos();
	if (pos != VID_BUF_SIZE)
	{
		if (c == '\n')
		{
			cursor_new_line();
		}
		else if (c == '\b')
		{
			cursor_backward();
		}
		else
		{
			ConsoleBuf[cursor_get_pos()].color = CONSOLE_COLOR;
			ConsoleBuf[cursor_get_pos()].symbol = c;
			cursor_forward();
		}
		return c;
	}
	return -1;
}

int 
puts(const char *str)
{
	while (*str != '\0')
	{
		if (putc(*str) == -1)
		{
			return -1;
		}
		str++;
	}
	return 0;
}

int 
getc()
{
	int c;

	while (1)
	{
		sema_down(&ConsoleInputSema);
		c = InputBuf;
		
		if (0 <= c && c <= 255)
		{
			putc(c);
			return c;
		}
	}
}

int 
gets(char *buf)
{
	int c, i = 0;
	int lim = cursor_get_pos();
	
	while (1)
	{
		c = getc();
		if (c == '\n')
		{
			break;
		}
		else if (c == '\b')
		{
			if (cursor_get_pos() >= lim)
			{
				putc(' ');
				cursor_backward();
				i--;
			}
			else
			{
				cursor_forward();
			}
		}
		else
		{
			buf[i++] = c;
		}
	}
	buf[i] = '\0';

	return i;
}

void
print_number(int n)
{
	char str[20];

	int beg = 0, end = 0;

	if (n < 0)
	{
		n *= -1;
		str[end++] = '-';
		beg = 1;
	}

	do
	{
		str[end++] = n % 10 + '0';
		n /= 10;
	} while (n);

	str[end++] = '\0';

	strrev(str + beg);

	puts(str);
}

#pragma endregion

#pragma region _INTERRUPT

void
intr_reg_handler(int num, unsigned short segm_sel, unsigned short flags, intr_handler hndlr)
{
	unsigned int hndlr_addr = (unsigned int)hndlr;

	g_idt[num].base_hi = (unsigned short)(hndlr_addr >> 16);
	g_idt[num].base_lo = (unsigned short)(hndlr_addr & 0xFFFF);
	g_idt[num].segm_sel = segm_sel;
	g_idt[num].always0 = 0;
	g_idt[num].flags = flags;
}

void
intr_init()
{
	int i;
	int idt_count = sizeof(g_idt) / sizeof(g_idt[0]); //256

	for (i = 0; i < idt_count; i++)
	{
		intr_reg_handler(i, GDT_CS, 0x80 | IDT_TYPE_INTR, default_intr_handler);	//segm_sel = 0x80, P = 1, DPL = 0, Type = Intr
	}
}

void
intr_start()
{
	int idt_count = sizeof(g_idt) / sizeof(g_idt[0]); //256

	g_idtp.base = (unsigned int)(&g_idt[0]);
	g_idtp.limit = (sizeof(struct idt_entry) * idt_count) - 1;

	__asm
	{
		lidt g_idtp
	}
}

void
intr_enable()
{
	__asm sti;
}

void
intr_disable()
{
	__asm cli;
}

__inline unsigned char
inb(unsigned short port)
{
	unsigned char data;
	__asm
	{
		push dx
		mov dx, port
		in al, dx
		mov data, al
		pop dx
	}
	return data;
}

__inline void
outb(unsigned short port, unsigned char data)
{
	__asm
	{
		push dx
		mov dx, port
		mov al, data
		out dx, al
		pop dx
	}
}

#pragma endregion

#pragma region _KEYBOARD

void
keyb_init()
{
	intr_reg_handler(0x09, GDT_CS, 0x80 | IDT_TYPE_INTR, keyb_handler);
	outb(PIC1_PORT + 1, 0xFF ^ 0x02);
}

void
keyb_process_key()
{
	intr_disable();
	// Проверка что буфер PS/2 клавиатуры не пуст (младший бит )
	if (inb(0x64) & 0x01)
	{
		unsigned char scan_code;
		unsigned char state;

		scan_code = inb(0x60); // Считывание символа с PS/2 клавиатуры 

		InputBuf = Symbols[scan_code];

		switch (InputBuf)
		{
		case 'SHFT':
		{
			SHFT = 1;
		} break;

		case 'UNSH':
		{
			SHFT = 0;
		} break;

		case 'CAPS':
		{
			CAPS = !CAPS;
		} break;

		default: 
			if (scan_code < 128)  // Скан-коды выше 128 - это отпускание клавиши 
			{
				if ((SHFT && !CAPS) || (!SHFT && CAPS))
				{
					InputBuf = Symbols[scan_code + 128];
				}
				
				sema_up(&ConsoleInputSema);
			}
			break;
		}
	}
	intr_enable();
}

void
symbol_init()
{
	int i;
	Symbols[0] = 'EMPT';			Symbols[128] = 'EMPT';
	Symbols[1] = 'ESC';/*ESC*/		Symbols[129] = 'EMPT';/*UNESC*/
	Symbols[2] = '1';				Symbols[130] = '!';
	Symbols[3] = '2';				Symbols[131] = '@';
	Symbols[4] = '3';				Symbols[132] = '#';
	Symbols[5] = '4';				Symbols[133] = '$';
	Symbols[6] = '5';				Symbols[134] = '%';
	Symbols[7] = '6';				Symbols[135] = '^';
	Symbols[8] = '7';				Symbols[136] = '&';
	Symbols[9] = '8';				Symbols[137] = '*';
	Symbols[10] = '9';				Symbols[138] = '(';
	Symbols[11] = '0';				Symbols[139] = ')';
	Symbols[12] = '-';				Symbols[140] = '_';
	Symbols[13] = '=';				Symbols[141] = '+';
	Symbols[14] = '\b';				Symbols[142] = 'UN\b';
	Symbols[15] = '\t';				Symbols[143] = '\t';
	Symbols[16] = 'q';				Symbols[144] = 'Q';
	Symbols[17] = 'w';				Symbols[145] = 'W';
	Symbols[18] = 'e';				Symbols[146] = 'E';
	Symbols[19] = 'r';				Symbols[147] = 'R';
	Symbols[20] = 't';				Symbols[148] = 'T';
	Symbols[21] = 'y';				Symbols[149] = 'Y';
	Symbols[22] = 'u';				Symbols[150] = 'U';
	Symbols[23] = 'i';				Symbols[151] = 'I';
	Symbols[24] = 'o';				Symbols[152] = 'O';
	Symbols[25] = 'p';				Symbols[153] = 'P';
	Symbols[26] = '[';				Symbols[154] = '{';
	Symbols[27] = ']';				Symbols[155] = '}';
	Symbols[28] = '\n';				Symbols[156] = '\n';
	Symbols[29] = 'EMPT';			Symbols[157] = 'EMPT';
	Symbols[30] = 'a';				Symbols[158] = 'A';
	Symbols[31] = 's';				Symbols[159] = 'S';
	Symbols[32] = 'd';				Symbols[160] = 'D';
	Symbols[33] = 'f';				Symbols[161] = 'F';
	Symbols[34] = 'g';				Symbols[162] = 'G';
	Symbols[35] = 'h';				Symbols[163] = 'H';
	Symbols[36] = 'j';				Symbols[164] = 'J';
	Symbols[37] = 'k';				Symbols[165] = 'K';
	Symbols[38] = 'l';				Symbols[166] = 'L';
	Symbols[39] = ';';				Symbols[167] = ':';
	Symbols[40] = '\'';				Symbols[168] = '"';

	Symbols[41] = 'EMPT';			Symbols[169] = 'EMPT';
	Symbols[42] = 'SHFT';/*SHIFT*/	Symbols[170] = 'UNSH';/*UNSHIFT*/

	Symbols[43] = '\\';				Symbols[171] = '|';
	Symbols[44] = 'z';				Symbols[172] = 'Z';
	Symbols[45] = 'x';				Symbols[173] = 'X';
	Symbols[46] = 'c';				Symbols[174] = 'C';
	Symbols[47] = 'v';				Symbols[175] = 'V';
	Symbols[48] = 'b';				Symbols[176] = 'B';
	Symbols[49] = 'n';				Symbols[177] = 'N';
	Symbols[50] = 'm';				Symbols[178] = 'M';
	Symbols[51] = ',';				Symbols[179] = '<';
	Symbols[52] = '.';				Symbols[180] = '>';
	Symbols[53] = '/';				Symbols[181] = '?';

	Symbols[54] = 'SHFT';/*SHIFT*/	Symbols[182] = 'UNSH';/*UNSHIFT*/
	Symbols[55] = 'EMPT';			Symbols[183] = 'EMPT';
	Symbols[56] = 'EMPT';			Symbols[184] = 'EMPT';
	Symbols[57] = ' ';				Symbols[185] = 'EMPT';
	Symbols[58] = 'CAPS';/*CAPS*/	Symbols[186] = 'UNCP';/*UNCAPS*/

	for (i = 59; i < 103; i++) 		for (i = 187; i < 231; i++)
		Symbols[i] = 'EMPT';			Symbols[i] = 'EMPT';

	Symbols[72] = 'UP';				Symbols[200] = 'UNUP';
	Symbols[75] = 'LEFT';			Symbols[203] = 'ULFT';
	Symbols[77] = 'RIGT';			Symbols[205] = 'URGT';
	Symbols[80] = 'DOWN';			Symbols[208] = 'UDWN';

	Symbols[103] = '7';				Symbols[231] = '7';
	Symbols[104] = '8';				Symbols[232] = '8';
	Symbols[105] = '9';				Symbols[233] = '9';
	Symbols[106] = '4';				Symbols[234] = '4';
	Symbols[107] = '5';				Symbols[235] = '5';
	Symbols[108] = '6';				Symbols[236] = '6';
	Symbols[109] = '1';				Symbols[237] = '1';
	Symbols[110] = '2';				Symbols[238] = '2';
	Symbols[111] = '3';				Symbols[239] = '3';
	Symbols[112] = '0';				Symbols[240] = '0';

	for (i = 113; i < 128; i++)		for (i = 241; i < 256; i++)
		Symbols[i] = 'EMPT';			Symbols[i] = 'EMPT';
}

#pragma endregion


#pragma region _SOLVER_OS

void
clear_flags()
{
	FLAG_OVERFLOW = 0;
	FLAG_EVAL_ERROR = 0;
	FLAG_CMD_ERROR = 0;
}

void
print_errors()
{
	if (FLAG_CMD_ERROR) puts("  > ERROR: incorrect command\n");
	else if (FLAG_OVERFLOW) puts("  > ERROR: int overflow\n");
	else if (FLAG_EVAL_ERROR) puts("  > ERROR: evaluation error");
}

int
gcd(int a, int b)
{
	while (a && b)
	{
		(a > b) ? (a %= b) : (b %= a);
	}

	return a + b;
}

int
lcm(int a, int b)
{
	if (a == 0 && b == 0) return 0;

	a = a / gcd(a, b);
	a = a * b;

	return a;
}

fraction
divf(int a, int b)
{
	fraction res;
	res.sgn = 1;
	res.numerator = 0;
	res.denominator = 0;

	if (b == 0)
	{
		FLAG_EVAL_ERROR = 1;
		return res;
	}

	if (a < 0 && b < 0 || a >= 0 && b > 0) res.sgn = 1;
	else res.sgn = -1;

	a = (a >= 0) ? (a) : (-a);
	b = (b >= 0) ? (b) : (-b);

	int d = gcd(a, b);
	res.numerator = a / d;
	res.denominator = b / d;

	return res;
}

void
print_decimal(fraction fr)
{
	int res = fr.numerator;

	if (fr.denominator == 0)
	{
		puts("(inf)");
		return;
	}

	if (fr.sgn < 0) puts("-");

	print_number(res / fr.denominator);

	res = res % fr.denominator;
	res *= 10;
	if (res == 0) return;

	putc('.');

	for (int i = 0; i < 5; i++)
	{
		print_number(res / fr.denominator);
		res = res % fr.denominator;
		res *= 10;
		if (res == 0) break;
	}
}

void
print_fraction(fraction fr)
{
	if (fr.denominator == 0)
	{
		puts("(inf)");
		return;
	}

	if (fr.sgn < 0) puts("-");

	print_number(fr.numerator);
	puts("/");
	print_number(fr.denominator);
}

int
Sum(int a, int b)//a+b
{
	if (a >= 0 && b >= 0) return (INTMAX - a >= b) ? (a + b) : (FLAG_OVERFLOW = 1);
	if (a < 0 && b < 0)   return (INTMAX + a + 1 >= -b) ? (a + b) : (FLAG_OVERFLOW = 1);
	return a + b;
}

fraction
solve(int a, int b, int c)
{
	c = Sum(c, -b);

	fraction res = divf(c, a);

	return res;
}

const char *
split(char *left, int separator, const char *str)
{
	if (str == NULL)
	{
		FLAG_CMD_ERROR = 1;
		return NULL;
	}

	while (*str == separator || *str == ' ') str++;

	while (*str != separator && *str)
	{
		*left++ = *str++;
	}
	*left = '\0';

	while (*str == separator || *str == ' ') str++;

	if (*str == '\0' || *str == '\n') return NULL;
	else return str;
}

int
atoi(const char *str)
{
	int num = 0, old = 0, sgn = 1;

	while (*str == '+' || *str == ' ') str++;

	while (*str == '-')
	{
		sgn = -sgn;
		str++;
		while (*str == '+' || *str == ' ') str++;
	}

	while ('0' <= *str && *str <= '9')
	{
		old = num;
		num = num * 10 + (*str - '0');

		if (num < old)
		{
			FLAG_OVERFLOW = 1;
			break;
		}

		str++;
	}

	if (*str != ' ' && *str != '\0')
	{
		FLAG_CMD_ERROR = 1;
	}

	return num * sgn;
}

void
parse_command(const char *scmd)
{
	char cmd[80];
	char arg[80];
	const char *lost;

	int a, b, c;

	lost = split(cmd, ' ', scmd);

	do //switch(str)
	{
		if (!strcmp(cmd, "info")) //case "info:"
		{
			puts("  > SolveOS by Bread Pitt 00000/0\n");
		}
		else if (!strcmp(cmd, "dec"))
		{
			FLAG_DECIMAL = 1;
			puts("  > decimal output was set\n");
		}
		else if (!strcmp(cmd, "frac"))
		{
			FLAG_DECIMAL = 0;
			puts("  > fraction output was set\n");
		}
		else if (!strcmp(cmd, "gcd"))
		{
			lost = split(arg, ' ', lost);
			a = atoi(arg);
			lost = split(arg, ' ', lost);
			b = atoi(arg);

			if (lost) FLAG_CMD_ERROR = 1;

			if (a <= 0 || b <= 0 || FLAG_CMD_ERROR || FLAG_OVERFLOW)
			{
				FLAG_EVAL_ERROR = 1;
				break;
			}
			puts("  > ");
			print_number(gcd(a, b));
			puts("\n");
		}
		else if (!strcmp(cmd, "lcm"))
		{
			lost = split(arg, ' ', lost);
			a = atoi(arg);
			lost = split(arg, ' ', lost);
			b = atoi(arg);

			if (lost) FLAG_CMD_ERROR = 1;

			if (a <= 0 || b <= 0 || FLAG_CMD_ERROR || FLAG_OVERFLOW)
			{
				FLAG_EVAL_ERROR = 1;
				break;
			}

			puts("  > ");
			print_number(lcm(a, b));
			puts("\n");
		}
		else if (!strcmp(cmd, "solve"))
		{
			lost = split(arg, 'x', lost);
			a = atoi(arg);
			lost = split(arg, '=', lost);
			b = atoi(arg);
			lost = split(arg, '-1', lost); //-1 = any symbol
			c = atoi(arg);

			fraction fr = solve(a, b, c);

			if (lost) FLAG_CMD_ERROR = 1;

			if (FLAG_CMD_ERROR || FLAG_OVERFLOW || FLAG_EVAL_ERROR)
			{
				break;
			}

			puts("  > ");
			if (FLAG_DECIMAL)
			{
				print_decimal(fr);
			}
			else
			{
				print_fraction(fr);
			}
			puts("\n");
		}
		else if (!strcmp(cmd, "div"))
		{
			lost = split(arg, ' ', lost);
			a = atoi(arg);
			lost = split(arg, ' ', lost);
			b = atoi(arg);

			if (lost) FLAG_CMD_ERROR = 1;

			if (b == 0 || FLAG_CMD_ERROR || FLAG_OVERFLOW)
			{
				FLAG_EVAL_ERROR = 1;
				break;
			}

			fraction fr = divf(a, b);

			puts("  > ");
			if (FLAG_DECIMAL)
			{
				print_decimal(fr);
			}
			else
			{
				print_fraction(fr);
			}
			puts("\n");
		}
		else if (!strcmp(cmd, "shutdown"))
		{
			shutdown();
		}
		else if (!strcmp(cmd, "help"))
		{
			puts("  > info         - information of OS\n");
			puts("  > dec/frac     - switch float representation\n");
			puts("  > solve ax+b=c - solve linear equation\n");
			puts("  > gcd a b      - find gcd of a and b\n");
			puts("  > lcm a b      - find lcm of a and b\n");
			puts("  > div a b      - divide a to b\n");
			puts("  > shutdown     - power off the computer\n");
		}
		else if (!strcmp(cmd, "\0"))
		{
			break;
		}
		else
		{
			FLAG_CMD_ERROR = 1;
		}

	} while (0); //end

	print_errors();
	clear_flags();
}

void
chose_color()
{
	int colours[6] = { 0x0F, 0xF0, 0x70, 0x01, 0x42, 0x21 };
	int i, c = 0, colornum = 0;

	CONSOLE_COLOR = 0b01110000;
	clear();

	for (i = 0; i < 6; i++)
	{
		CONSOLE_COLOR = colours[i];
		cursor_setpos(35, 9 + i);
		cursor_moveto();

		puts("SolveOS");
	}

	CONSOLE_COLOR = 0b01110000;
	cursor_setpos(34, 9);
	cursor_moveto();
	putc('>');
	cursor_setpos(42, 9);
	cursor_moveto();
	putc('<');

	while (1)
	{
		sema_down(&ConsoleInputSema);
		c = InputBuf;

		switch (c)
		{
		case 'UP':   colornum = (colornum + 4) % 6;
		case 'DOWN': colornum = (colornum + 1) % 6;
			for (i = 0; i < 6; i++)
			{
				CONSOLE_COLOR = 0b01110000;

				if (colornum == i)
				{
					cursor_setpos(34, 9 + i);
					cursor_moveto();
					putc('>');
					cursor_setpos(42, 9 + i);
					cursor_moveto();
					putc('<');
				}
				else
				{
					cursor_setpos(34, 9 + i);
					cursor_moveto();
					putc(' ');
					cursor_setpos(42, 9 + i);
					cursor_moveto();
					putc(' ');
				}
				CONSOLE_COLOR = colours[i];
				cursor_setpos(35, 9 + i);
				cursor_moveto();
				puts("SolveOS");
			}
			break;
		case '\n':
			CONSOLE_COLOR = colours[colornum];
			clear();
			return;
		default: break;
		}
	}
}

#pragma endregion
