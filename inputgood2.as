		
la	label
call AscizLabel
jmp $7

notInSymbolTable:	.entry	label

label:	.dh	1,2,14,8
.asciz "maximum line length is eighty characters- this line is exactly that len"


;Data Commands 
dbLabel: .db		+34, -4, 5   , -9, 16, 127, -128 
dhLabel: .dh		+34, -4, 5   , -9, 16,-32768,32767 
dwLabel: .dw		+34, -4, 5   , -9, 16,-2147483648,2147483647  
AscizLabel:	.asciz	"words are weird"
		.asciz	"!@#$3%^&scd45Tt45y6uU87L"    
		.asciz	"	"		

		
;	R Commands
commandR:	add	$4 ,$15 ,$12
		sub	$17 ,$24 ,$25
		and	$2 ,$7 ,$8
commandR2:	or	$13 ,$10,$18
		nor	$2,$5,$14
		
commandR3:	move	$5,$6
		mvhi	$23,$19
		mvlo	$9,$2

;	I Commands

commandI:	addi	$14  ,	1233  ,  $1
		subi	$26 , 	-34 ,$27  
		andi	$18,	-30,	$19
		ori	$31,-32768,$31 
		nori	$17,67,$20

		bne	$3 , 	$24,	END   
commandI2:	beq	$26,	$26,	commandI
		blt	$15,	$0,	commandJ  
		bgt	$29,	$15,	commandR2

		lb	$0,-32768,$31  
		sb	$6 , 	0	,	$27
		lw	$31,	-30,	$29
		sw	$24  ,	576  ,  $30   
commandI3:	lh	$17,32767,$20
		sh	$15 , 	0	,	$9  
  
;	J Commands
commandJ:	jmp		maxLabelLengthIsExactlyThirty1c
		jmp		$7
		la		commandI2
		la		exLabel1
		call	AscizLabel
		call	exLabel1
.entry AscizLabel
.entry dbLabel
.entry dhLabel
.entry dbLabel
.entry AscizLabel

callingItself:	call	callingItself

jmp		exLabel2  

la		dbLabel
		
		jmp		AscizLabel



.extern exLabel1   
.extern exLabel2
.extern    	exLabel2
.extern 			exLabel2

	la		exLabel1   
				

maxLabelLengthIsExactlyThirty1c: sw	$1  ,	-2132  ,  $0  

END:	stop




