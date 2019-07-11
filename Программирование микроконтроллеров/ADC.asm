.DEF PWM = R5

.DEF b0 = R10
.DEF b1 = R11
.EQU b2 = 0x00
.DEF  h = R12

.DEF Anum = R16
.DEF Bnum = R17
.DEF Cnum = R18

.DEF Parametr = R19

.DEF Segm0 = R20
.DEF Segm1 = R21
.DEF Segm2 = R22
.DEF Segm3 = R23

.MACRO OUTI
	PUSH R24
	LDI R24, @1
	OUT @0, R24
	POP R24
.ENDM

.MACRO WRITE //num, addr, offset 
	LDI R24, @2
	STS @0 + @1, R24
.ENDM	//to write in an array

.MACRO READ  //Rd, addr, Roffset
	LDI ZL, LOW(@1)
	LDI ZH, HIGH(@1)
	ADD R30, @2
	LD @0, Z
.ENDM	//to read from an array

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
.ORG 0x0060		seg_num: .BYTE 21

.CSEG
.ORG 0x0000		JMP start
.ORG 0x0002		JMP int0_handler
.ORG 0x0004		JMP int1_handler
.ORG 0x0020		JMP ADC_handler

int0_handler:	/*переключение режимов*/		
	BRTC	set_parametr_mode		//if T - clear: garland mode
	CLT			//set_garland_mode
RETI
set_parametr_mode:
	SET
RETI

int1_handler:	/*переключение параметров*/
	IN		R24, SREG
	INC		Parametr
	CPI		Parametr, 3
	BRNE	NOT_EQ0
	LDI		Parametr, 0
	OUT		SREG, R24
NOT_EQ0:
RETI

ADC_handler:
	PUSH	R24
	IN		R24, ADCH
	OUT		PORTB, R24
	POP		R24
RETI

start:
	OUTI	DDRA, 0b11011111
	OUTI	DDRB, 0b11111111
	OUTI	DDRC, 0b11111111
	OUTI	DDRD, 0b11110011

	/*STACK*/
	OUTI	SPL, 0xFF
	OUTI	SPH, 0x00	
	/*STACK*/

	/*INT*/ 
	OUTI	DDRD,	0b11110011 		
	OUTI	MCUCR,	0b00001010		;condition 0/1		
	OUTI	GICR,	0b01000000		;ENABLE INT0/1
	/*INT*/ 

	/*ADC*/
	OUTI	ADCSRA, 0b11100111		;[ADEN][ADSC][ADATE][ADIF][ADIE][ADPS2..0]
									; ВКЛ   ПРЕОБРАЗ-Я   ПРЕР  ЛОК   ПРЕДДЕЛИТЕЛЬ
	OUTI	ADMUX,	0b00100101		;[REFS(2)][ADLAR][MUX4..0]
									; напряж   вырав  номер_порта
	OUTI	SFIOR,	0b00000000		;[ADTS2..0]....режим
	/*ADC*/

	/*PWM Т2*/
	OUTI	TCCR2,	0b01111100
	OUTI	TCNT2,	0
	OUTI	OCR2,	1
	/*PWM Т2*/
	
	/*PWM Т1*/
	OUTI	TCCR1A, 0b00100001
	OUTI	TCCR1B, 0b00001011
	OUTI	OCR1BH, 0
	OUTI	OCR1BL, 1
	/*PWM Т1*/

	/*SEGMENT NUM INIT*/
	WRITE	seg_num,  0, 0x3F		;0	
	WRITE	seg_num,  1, 0x06		;1	
	WRITE	seg_num,  2, 0x5B		;2	
	WRITE	seg_num,  3, 0x4F		;3	
	WRITE	seg_num,  4, 0x66		;4	
	WRITE	seg_num,  5, 0x6D		;5	
	WRITE	seg_num,  6, 0x7D		;6	
	WRITE	seg_num,  7, 0x07		;7	
	WRITE	seg_num,  8, 0x7F		;8	
	WRITE	seg_num,  9, 0x6F		;9	
	WRITE	seg_num, 10, 0x77		;A	
	WRITE	seg_num, 11, 0x7C		;B	
	WRITE	seg_num, 12, 0x39		;C	
	WRITE	seg_num, 13, 0x5E		;D	
	WRITE	seg_num, 14, 0x79		;E	
	WRITE	seg_num, 15, 0x71		;F	
	WRITE	seg_num, 16, 0x40		;-	
	WRITE	seg_num, 17, 0x7C		;b	
	WRITE	seg_num, 18, 0xBF		;0.	
	WRITE	seg_num, 19, 0x86		;1.	
	WRITE	seg_num, 20, 0xF4		;h.	
	/*SEGMENT NUM INIT*/

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
	IN		b0,		ADCH
	IN		PWM,	ADCL
	LDI		R24,	17
	READ	Segm0,	seg_num, R24
	LDI		R24,	18
	JMP		Case12_End
Case1:		/*PARAMETR B1*/
	IN		b1, ADCH
	IN		PWM, ADCL
	LDI		R24, 17
	READ	Segm0, seg_num, R24
	LDI		R24, 19
Case12_End:	/*SAME INIT PART FOR BOTH B0, B1*/
	READ	Segm1, seg_num, R24
	IN		R24, ADCH
	IN		R25, ADCH
	LSR		R24 
	LSR		R24 
	LSR		R24 
	LSR		R24
	READ	Segm2, seg_num, R24
	ANDI	R25, 0x0F
	READ	Segm3, seg_num, R25
	JMP		EndCase
Case2:		/*PARAMETR H*/
	IN		R24, ADCH
	LDI		R25, 25
	MUL		R24, R25
	MOV		R26, R1
	SUBI	R26, 12
	MOV		h, R26

	LDI		R26, 2
find_PWM:
	CP		R26, R24
	BRLO	add_11
	JMP		Init_Pwm
add_11:		
	SUBI	R26, 245		;R26 += 11
	JMP		find_PWM
Init_Pwm:
	SUB		R26, R24
	MUL		R26, R25
	MOV		PWM, R0

	LDI		R24, 20
	READ	Segm0, seg_num, R24
	LDI		Segm1, 0

	MOV		R26, h
	CPI		R26, 0
	BRLT	less
	JMP		more
deliveryman_to_parameter_mode:		JMP parameter_mode
less:
	LDI     R24, 16
	READ	Segm2, seg_num, R24
	NEG		R26
	JMP		seg2_end
more:
	LDI		Segm2, 0
seg2_end:
	READ	Segm3, seg_num, R26
	JMP		EndCase
EndCase:
	OUT		OCR1BL, PWM
	OUT		OCR2, PWM
	LDI		R26, 12					//ДЛЯ ЗАДЕРЖКИ 0.5 сек 
	/*7-SEGMENT OUTPUT*/
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
	DEC		R26
	BRNE	Segm_out

	BRTS	deliveryman_to_parameter_mode
	/*CHANGE MODE*/
	OUTI	GICR, 0b01000000 
	OUTI	ADCSRA, 0b11100111
	OUTI	DDRA, 0b11111111
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
	OUTI	GICR, 0b11000000 
	OUTI	ADCSRA, 0b11101111
	OUTI	DDRA, 0b11011111
JMP parameter_mode