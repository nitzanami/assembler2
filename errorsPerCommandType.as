;arithmetic no space after command
H: add$5,$4,$3
;arithmetic no space after label
X:add $5, $3, $28
;arithmetic invalid registers:
Y: and $55, $1, $0
Z: or $-1, $2, $3
damn: add $1, or, $2
ohNo: add $5, "or", $2
ayeNo: add 21, $5, $29
;arithmetic excessive arguments:
J: add $1, $2, $3, $4
excessiveWithComma: add $2, $3, $2,
;arithmetic missing arguments:
missing: add $1, $2
missingWithComma: add $3, $5, 


;conditional no space after command
H: bne$5,$4,Z
;conditional invalid registers:
Y: bne $55, $1, loop
Z: bne $-1, $2, damn
damn: bne $1, or, why
ohNo: bne $5, "or", X
ayeNo: bne 21, $5, xmen
registerInstead: bne $3, $5, $2
;conditional excessive arguments:
J: bne $1, $2, loop, shmoop
excessiveWithComma: bne $2, $3, doop,
;conditional missing arguments:
missing: bne $1, $2
missingWithComma: bne $3, $5, 


;copy no space after command
H: move$5,$4
;copy no space after label
X:move $5, $3
;copy invalid registers:
Y: move $55, $1
Z: mvhi $-1, $2
damn: mvlo $1, or
ohNo: move $5, "or"
ayeNo: move 21, $5
;copy excessive arguments:
J: move $1, $2, $3
excessiveWithComma: move $2, $3,
;copy missing arguments:
missing: move $1
missingWithComma: move $3,


;arithmeticI no space after command
H: addi$5,-45,$3
;arithmeticI no space after label
X:addi $5, -45, $28
;arithmeticI invalid registers:
Y: andi $55, -45, $0
Z: ori $-1, -45, $3
damn: addi $1, or, $2
ohNo: addi $5, "or", $2
ayeNo: addi 21, 45, $29
outOfRange: addi $2, 34563456346546, $29
outOfRangeToo: ori $2, -23452345234523452, $20
outOfRangeCloseToMin: $3, -32769, $21
outOfRangeCloseToMax: $3, 32768, $15
outOfRangeLimitOfIntMax: $3, 2147483648, $13
outOfRangeLimitOfIntMin: $10, -2147483649, $14 
;arithmeticI excessive arguments:
J: addi $1, 55, $3, $4
excessiveWithComma: addi $2, 3, $2,
;arithmeticI missing arguments:
missing: addi $1, 2
missingWithComma: addi $3, 5, 


;call no space after command
H: call$5,$4,Z
;call invalid argument:
Y: call 1loop
call $22
ayeNo: call "xmen"
;call excessive arguments:
J: call loop, shmoop
excessiveWithComma: call doop,
;call missing arguments:
missing: call 
missingWithComma: call , 
;jmp illegal register:
jojo: jmp $-2
joji: jmp $33

;stop excessive arguments:
lastOnes: stop 555
Iswear: stop hi
