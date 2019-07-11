 .code16
.org 0x7c00
main:
	movw %cs, %ax
	movw %ax, %ds	
	movw %ax, %ss	
	movw $main, %sp	

	pusha
#kernel load{
	
	movb $1, %dl 
	movb $0, %ch	#track

#section 1
	movb $2, %cl	#sector num ===========================
	movb $19, %al	#size

	movw $0x1100, %bx
	movw %bx, %es
	xor %bx, %bx

	mov $0x02, %ah
	int $0x13

#section 2
	movb $21, %cl	#sector num ===========================
	movb $8, %al	#size
	
	movw $0x1400, %bx
	movw %bx, %es
	xor %bx, %bx

	mov $0x02, %ah
	int $0x13

#kernel load}
	popa

	cli
	
	lgdt gdt_info
	
	inb $0x92, %al
	orb $2, %al
	outb %al, $0x92
	
	movl %cr0, %eax
	orb $1, %al
	movl %eax, %cr0

	ljmp $0x8, $prot_mod
	#конец 


.code32
prot_mod:
	movw $0x10, %ax
	movw %ax, %es
	movw %ax, %ds
	movw %ax, %ss

	#movl $0xb8000, %esi		#адрес начала консоли (видеопамяти)
	#add  $2, %esi 
	#movb $49, 0(%esi)			#записали символ по адресу
	#inc %esi 
	#movb $0x1f, 0(%esi)		#записали цвет

	call 0x11000

system_END:
jmp .

gdt:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9A, 0xCF, 0x00
	.byte 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00

gdt_info:
	.word gdt_info - gdt
	.word gdt, 0

.zero (510 - (. - main))

.byte 0x55
.byte 0xaa