.MACRO ROR3				;объявление макроса, в качестве аргумента @0 - шаг смещения
	PUSH @0				;запоминаем шаг смещения
	TST @0				;тест шага смещения: если @0 == 0 то флаг Z станет равным 1
	BREQ end			;если флаг Z установлен - переходим в конец
rorloop:				;начало цикла смещения бит
	CLT					;очищаем пользовательский флаг
	LSR R18				;сдвигаем битово вправо крайний правый регистр из тройки
	BRCC l1				;если было переполнение то
	SET					;устанавливаем пользовательский флаг T = 1
l1:	LSR R17				;сдвигаем битово вправо второй регистр
	BRCC l2				;если было переполнение то
	SBR R18, 0b10000000	;записываем эту единичку в следующий регистр (тот что правее)
l2:	LSR R16				;/*
	BRCC l3				; ----//----
	SBR R17, 0b10000000	;*/
l3: BRTC l4				;если пользовательский флаг был установлен (было переполнение в R18) то
	SBR R16, 0b10000000	;записываем единичку в крайний левый регистр
l4:	DEC @0				;уменьшаем шаг тк один сдвиг уже сделали
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
	CLT
	LSL R16
	BRCC l1
	SET
l1:	LSL R17
	BRCC l2
	SBR R16, 0b00000001
l2:	LSL R18
	BRCC l3
	SBR R17, 0b00000001
l3: BRTC l4
	SBR R18, 0b00000001
l4:	DEC @0
	BRNE rolloop
end:
	POP @0
.ENDM

.CSEG		//начало описания сегмента программы
.ORG 0x0000	JMP reset			; обработчик прерывания RESET
.ORG INT0addr	JMP INT0_handler	; обработчик прерывания INT0
.ORG INT1addr	JMP INT1_handler	; обработчик прерывания INT1

INT0_handler:	;изменение числа линий
	INC R22
	CPI R22, 4
	BRNE NOT_EQ0
	LDI R22, 1
 NOT_EQ0:
	IN R28, PORTD
	CBR R28, 0x03
	OR R28, R22 
	OUT PORTD, R28

	PUSH R22
	LDI R21, 0b00000000
init:
	LSR R21
	ORI R21, 0b10000000
	DEC R22
	BRNE init

	POP R22
	MOV R16, R21
	LDI	R17, 0
	LDI	R18, 0

	/**/RCALL DELAY2/**/																																				BRNE delay_loop1
RETI



INT1_handler:	;изменение шага
	INC R20
	CPI R20, 5  ;Сравнить регистр R20 и 5
    	BRNE NOT_EQ1
	LDI R20, -4
NOT_EQ1:
	MOV R27, R20
	CBR R27, 0xF0
	SWAP R27
	IN R28, PORTD
	CBR R28, 0xF0
	OR R27, R28
	OUT PORTD, R27

	/**/RCALL DELAY2/**/	
RETI

/*==================*/
DELAY2:
	LDI R23, 0	
delay_loop1: 
	DEC R23
	BRNE delay_loop1
RET
/*==================*/

reset: 
	CLI
	SER R16 
	OUT DDRA, R16 
	OUT DDRB, R16 
	OUT DDRC, R16		;сконфигурировали все порты как порты вывода 

	OUT SPL,R16			; Set Stack Pointer to 255 
	CLR R16 
	OUT SPH,R16 
	
	LDI R16, 0b11110011 
	OUT DDRD, R16 
	SBI PORTD, 2
	SBI PORTD, 3

	LDI R16, 0b00001010
	OUT MCUCR, R16		; Set int0 and int1 on condition 0/1
	
	LDI R16, 0b11000000 
	OUT GICR, R16		; Enable int0 and int1 

	LDI R21, 0b10000000	; begin lines
	LDI R22, 1			; begin lines COUNT
	LDI R20, 1
	SBI PORTD, 4
	SBI PORTD, 0

	MOV R16, R21
	LDI	R17, 0
	LDI	R18, 0

garland_loop:

	CLI
	TST R20
	BRPL positive
	ROL3 R20
	JMP output
positive:
	ROR3 R20
output:
	SEI

	OUT PORTA, R16
	OUT PORTB, R17
	OUT PORTC, R18

	RCALL delay
RJMP garland_loop

delay:
	LDI R23, 255
	LDI R24, 250
	LDI R25, 20
delay_loop:
	DEC R23
	BRNE delay_loop
	DEC R24
	BRNE delay_loop
	DEC R25
	BRNE delay_loop
RET