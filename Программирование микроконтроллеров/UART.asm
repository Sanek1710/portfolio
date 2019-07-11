.DEF Segm0 = R6
.DEF Segm1 = R7
.DEF Segm2 = R8
.DEF Segm3 = R9

.DEF b0	= R10
.DEF b1 = R11
.EQU b2 = 0x00
.DEF  h = R12

.DEF Anum = R16
.DEF Bnum = R17
.DEF Cnum = R18

.DEF Parametr = R19

.DEF send_indx = R20
.DEF resv_indx = R21

.MACRO OUTI
	PUSH R24
	LDI R24, @1
	OUT @0, R24
	POP R24
.ENDM

.MACRO WRITE	//addr, R_offset, reg
	LDI		ZL, LOW(@0)
	LDI		ZH, HIGH(@0)
	ADD		ZL, @1
	ST		Z, @2
.ENDM	//to write in an array

.MACRO WRITEI	//addr, offset, num
	LDI R24, @2
	STS @0 + @1, R24
.ENDM	//to write in an array


.MACRO READ		//Rd, addr, Roffset
	LDI ZL, LOW(@1)
	LDI ZH, HIGH(@1)
	ADD R30, @2
	LD @0, Z
.ENDM	//to read from an array

.MACRO READI	//Rd, addr, offset
	LDS @0, @1 + @2
.ENDM	//to read from an array

.MACRO GETC		//Rd <-- num or -1 as error
	CPI @0, '0'
	BRLO invalid_symb
	CPI @0, '9' + 1
	BRLO ok_symb_09
	CPI @0, 'A'
	BRLO invalid_symb
	CPI @0, 'F' + 1
	BRLO ok_symb_AF
invalid_symb:
	LDI @0, -1
	JMP getc_end
ok_symb_09:
	SUBI @0, '0'
	JMP getc_end
ok_symb_AF:
	SUBI @0, 'A' - 10
getc_end:
.ENDM

.MACRO ROR3				;объявление макроса, в качестве аргумента @0 - шаг смещения
	PUSH @0				;запоминаем шаг смещения
	TST @0				;тест шага смещения: если @0 == 0 то флаг Z станет равным 1
	BREQ end			;если флаг Z установлен - переходим в конец
rorloop:				;начало цикла смещения бит	
	LSR Anum
	ROR Bnum
	ROR Cnum
	BRCC l1
	SBR Anum, 0b10000000
l1:	
	DEC @0				;уменьшаем шаг тк один сдвиг уже сделали
	BRNE rorloop		;зацикливаем пока не сделаем все шаги, если сделали то
end:					;
	POP @0				;возвращаем на место значение шага
.ENDM					;заканчиваем

.MACRO ROL3
	PUSH @0
	TST @0
	BREQ end
	NEG @0
rolloop:
	LSL Cnum
	ROL Bnum
	ROL Anum
	BRCC l1
	SBR Cnum, 0b00000001
l1:	
	DEC @0
	BRNE rolloop
end:
	POP @0
.ENDM

.DSEG
.ORG 0x0060		
seg_num:	.BYTE 21	;0x60 .. 0x74
char_symb:  .BYTE 21	;0x75 .. 0x89
to_send0:	.BYTE 1		;0x8A
to_send1:	.BYTE 1		;0x8B
to_send2:	.BYTE 1		;0x8C
to_send3:	.BYTE 1		;0x8D
uart_buf0:	.BYTE 1		;0x8E
uart_buf1:	.BYTE 1		;0x8F

.CSEG
.ORG 0x0000		JMP start
.ORG 0x0002		JMP int0_handler
.ORG 0x0004		JMP int1_handler
.ORG 0x001A		JMP RECEIVE_UART_handler	;Прием байта приемщиком UART завершен
.ORG 0x001E		JMP TRANSMIT_UART_handler	;Передача данных передатчиком UART завершена

int0_handler:	/*переключение режимов*/		
	LDI R26, 1
	BRTC	set_parametr_mode		//if T - clear: garland mode
	CLT			//set_garland_mode
RETI
set_parametr_mode:
	SET
RETI

int1_handler:	/*переключение параметров*/
	LDI R26, 1
	IN		R24, SREG
	INC		Parametr
	CPI		Parametr, 3
	BRNE	NOT_EQ0
	LDI		Parametr, 0
NOT_EQ0:
	OUT		SREG, R24
RETI

RECEIVE_UART_handler:
	IN		R22,UDR
	WRITE	uart_buf0, resv_indx, R22
	INC		resv_indx
	CPI		resv_indx, 2
	BREQ	DONE_2_LETTER
	RETI
DONE_2_LETTER:
	LDI resv_indx,0

	CPI		Parametr, 0
	BREQ	Case_0
	CPI		Parametr, 1
	BREQ	Case_0
	CPI		Parametr, 2
	BREQ	Case_2
	JMP		EndCase1
Case_0:
	LDS R22, uart_buf0 //first simbol
	GETC R22
	CPI		R22, -1 // simbol not found
	BREQ	deliver_to_error

	LDS R23, uart_buf1 //second simbol
	GETC R23
	CPI		R23, -1	// simbol not found
	BREQ	deliver_to_error

	LSL		R22		
	LSL		R22
	LSL		R22
	LSL		R22

	CPI		Parametr, 1
	BREQ	Case_1

	MOV		b0,R22 //put in b0 new count 
	OR		b0,R23

	JMP EndCase1
deliver_to_error: JMP ERROR

Case_1:
	MOV		b1,R22 //put in b1 new count 
	OR		b1,R23
	JMP EndCase1
Case_2:	//h
	LDS R22, uart_buf0 //first simbol must be  sign
	CPI		R22, '-' 
	BREQ	less1
	CPI		R22, '+' 
	BREQ	more1
	JMP ERROR
less1:	
	LDI R23, 255
	JMP CMP_END
more1:
	LDI R23, 0
CMP_END:
	LDS R22, uart_buf1 //first simbol
	CPI R22, '0'//<='0'
	BRLO ERROR
	CPI R22, 'C' + 1 //<='C'
	BRLO ok_symb_0_C
JMP ERROR
ok_symb_0_C:
	GETC R22
	MOV h, R22
	ADD h, R23
	JMP EndCase1
EndCase1: 
	LDI R26, 1
	RETI
ERROR:
	LDI R22, 'R'
	STS to_send1,R22
	LDI R22, 'O'
	STS to_send2,R22
	LDI R22, 'R'
	STS to_send3,R22
	LDI R22, 'E'
	STS to_send0,R22

UDRE_is_FULL2:	
	SBIS	UCSRA, UDRE
	JMP 	UDRE_is_FULL2
	OUT		UDR, R22
RETI

TRANSMIT_UART_handler:
	
	INC		send_indx
	CPI		send_indx, 4
	BRNE	NOT_FINISH
	LDI		send_indx ,0
	RETI
	NOT_FINISH:	
	READ	R22, to_send0, send_indx

UDRE_is_FULL1:	
	SBIS	UCSRA, UDRE
	JMP 	UDRE_is_FULL1
	OUT		UDR, R22	
RETI

start:
	OUTI	DDRA, 0b11111111
	OUTI	DDRB, 0b11111111
	OUTI	DDRC, 0b11111111
	OUTI	DDRD, 0b11110010
	LDI		resv_indx,0
	LDI		send_indx, 0
	/*STACK*/
	OUTI	SPL, 0xFF
	OUTI	SPH, 0x00 
	/*STACK*/

	/*INTerrupt*/ 
	OUTI	MCUCR,	0b00001010		;condition 0/1		
	OUTI	GICR,	0b01000000		;ENABLE INT0/1
	/*INTerrupt*/ 

	/*UART*/ 
	OUTI UCSRA, 0b00100000		;[RXC][TXC][UDRE][FE][DOR][PE][U2X][MPCM]
	OUTI UCSRB, 0b11000000
	OUTI UCSRC, 0b10100100
	OUTI UBRRH, 0
	OUTI UBRRL, 51				;boud
	OUTI UDR,	0b00000000
	/*UART*/ 

	/*SEGMENT NUM INIT*/
	WRITEI 	seg_num,  0, 0x3F		;0	
	WRITEI	seg_num,  1, 0x06		;1	
	WRITEI	seg_num,  2, 0x5B		;2	
	WRITEI	seg_num,  3, 0x4F		;3	
	WRITEI	seg_num,  4, 0x66		;4	
	WRITEI	seg_num,  5, 0x6D		;5	
	WRITEI	seg_num,  6, 0x7D		;6	
	WRITEI	seg_num,  7, 0x07		;7	
	WRITEI	seg_num,  8, 0x7F		;8	
	WRITEI	seg_num,  9, 0x6F		;9	
	WRITEI	seg_num, 10, 0x77		;A	
	WRITEI	seg_num, 11, 0x7C		;B	
	WRITEI	seg_num, 12, 0x39		;C	
	WRITEI	seg_num, 13, 0x5E		;D	
	WRITEI	seg_num, 14, 0x79		;E	
	WRITEI	seg_num, 15, 0x71		;F	
	WRITEI	seg_num, 16, 0x40		;-	
	WRITEI	seg_num, 17, 0x7C		;b	
	WRITEI	seg_num, 18, 0xBF		;0.	
	WRITEI	seg_num, 19, 0x86		;1.	
	WRITEI	seg_num, 20, 0xF4		;h.	
	/*SEGMENT NUM INIT*/

	/*CHAR INIT*/
	WRITEI	char_symb,  0, '0'		;48	
	WRITEI	char_symb,  1, '1'		;49	
	WRITEI	char_symb,  2, '2'		;50	
	WRITEI	char_symb,  3, '3'		;51	
	WRITEI	char_symb,  4, '4'		;52	
	WRITEI	char_symb,  5, '5'		;53	
	WRITEI	char_symb,  6, '6'		;54	
	WRITEI	char_symb,  7, '7'		;55	
	WRITEI	char_symb,  8, '8'		;56	
	WRITEI	char_symb,  9, '9'		;57	
	WRITEI	char_symb, 10, 'A'		;65	
	WRITEI	char_symb, 11, 'B'		;66	
	WRITEI	char_symb, 12, 'C'		;67	
	WRITEI	char_symb, 13, 'D'		;68	
	WRITEI	char_symb, 14, 'E'		;69	
	WRITEI	char_symb, 15, 'F'		;70	
	WRITEI	char_symb, 16, 'b'		;98
	WRITEI	char_symb, 17, 'h'		;104	
	WRITEI	char_symb, 18, '-'		;45
	WRITEI	char_symb, 19, '+'		;43
	WRITEI	char_symb, 20, 'R'		;82 
	/*CHAR INIT*/

	/*BEGIN PARAMETERS*/
	LDI		R24,	0b10000000
	MOV		b0,		R24
	LDI		R24,	0b00000000
	MOV		b1,		R24
	LDI		R24,	1
	MOV		h,		R24
	LDI		Parametr, 0
	CLT
	/*BEGIN PARAMETERS*/

	SEI

RJMP garland_mode


parameter_mode:
	CPI		Parametr, 0
	BREQ	Case0
	CPI		Parametr, 1
	BREQ	Case1
	CPI		Parametr, 2
	BREQ	Case2
	JMP		EndCase
Case0:		/*PARAMETR B0*/
	LDI R22,'b'
	STS to_send0, R22
	LDI R22,'0'
	STS to_send1, R22

	MOV R23, b0
	MOV R22, b0; 

	READI	Segm0,	seg_num, 17
	READI	Segm1,	seg_num, 18
	MOV		R24,	b0
	JMP		Case01_End
Case1:		/*PARAMETR B1*/
	LDI R22,'b'
	STS to_send0, R22
	LDI R22,'1'
	STS to_send1, R22

	MOV R23, b1
	MOV R22, b1;
	
	
	READI	Segm0,	seg_num, 17
	READI	Segm1,	seg_num, 19
	MOV		R24,	b1
Case01_End:	/*SAME INIT PART FOR BOTH B0, B1*/

	LSR R22
	LSR R22
	LSR R22
	LSR R22 
	READ R22, char_symb, R22
	STS to_send2, R22

	
	ANDI R23, 0x0F
	READ R23, char_symb, R23
	STS to_send3, R23

	MOV		R25,	R24
	LSR		R24
	LSR		R24
	LSR		R24
	LSR		R24
	ANDI	R25, 0x0F
	READ	Segm2,	seg_num, R24
	READ	Segm3,	seg_num, R25
	JMP		EndCase

Case2:		/*PARAMETR H*/
LDI R22,'h'
	STS to_send0, R22
	LDI R22,'.'
	STS to_send1, R22
	MOV R23, h
		
	READI	Segm0,	seg_num, 20
	LDI     R24, 0
	MOV		Segm1, R24

	MOV		R24, h
	CPI		R24, 0
	BRLT	less
	JMP		more
less:
	LDI     R25, 16
	READ	Segm2, seg_num, R25
	NEG		R24	
	LDI		R22,'-'
	STS		to_send2, R22	
	JMP		seg2_end
more:
	LDI     R25, 0
	MOV		Segm2, R25
	LDI		R22,'+'
	STS		to_send2, R22
seg2_end:
	READ	R22, char_symb, R24
	STS		to_send3, R22
	READ	Segm3, seg_num, R24
	JMP EndCase
deliveryman_to_parameter_mode:		JMP parameter_mode
EndCase:
	LDS		R22, to_send0	
UDRE_is_FULL:	
	SBIS	UCSRA, UDRE
	JMP 	UDRE_is_FULL
	OUT		UDR, R22
Segm_out:
	OUTI	PORTA, 0b00001000
	OUT		PORTC, Segm0
	CALL	delay_10ms
	OUTI	PORTA, 0b00000100
	OUT		PORTC, Segm1
	CALL	delay_10ms
	OUTI	PORTA, 0b00000010
	OUT		PORTC, Segm2
	CALL	delay_10ms
	OUTI	PORTA, 0b00000001
	OUT		PORTC, Segm3
	CALL	delay_10ms
	CPI R26, 1
	BREQ Stop_segm_out
	JMP Segm_out

Stop_segm_out:
	

	LDI R26, 0
	BRTS	deliveryman_to_parameter_mode
	/*CHANGE MODE*/
	OUTI	GICR,	0b01000000 
	OUTI	UCSRB,	0b00000000
JMP garland_mode


/*DELAY*/
delay_10ms:
	LDI		R29, 1
	LDI		R28, 104
	LDI		R27, 228
	JMP		delay_loop
delay_666ms:
	LDI		R29, 28
	LDI		R28, 15
	LDI		R27, 84
delay_loop:
	DEC		R27
	BRNE	delay_loop
	DEC		R28
	BRNE	delay_loop
	DEC		R29
	BRNE	delay_loop
RET
/*DELAY*/


garland_mode:
	/*INIT GARLAND OUTPUT*/
	MOV		Anum, b0
	MOV		Bnum, b1
	LDI		Cnum, b2
	
garland_mode_loop:	
	
	OUT		PORTA, Anum
	OUT		PORTB, Bnum
	OUT		PORTC, Cnum
	RCALL	delay_666ms

	CLI
	TST		h
	BRPL	positive
	ROL3	h
	JMP		output
positive:
	ROR3	h
output:
	SEI

	BRTC	garland_mode_loop
	/*CHANGE MODE*/
	OUTI	GICR,	0b11000000 
	OUTI	UCSRB,	0b11011000
JMP parameter_mode
