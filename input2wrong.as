label:.asciz "for checking"
add 
add $3
add $3, 5
add $3, $5, $4, $2
move 
move $3
move $3, 5
move $3, $5, $4, $2
addi 
addi $3
addi $4, $5, $6
addi $3, -5, $4, $2
bne 
bne $3
bne $3, $5, $4
bne $3, $5, labelNotExist
bne $3, $5, label, $2
sw 
sw $3
sw $3, $5, $4
sw $3, -5, $4, $2
jmp 
jmp 4g
jmp labelNotExist
jmp label, $5, $4, $2
jmp $3, $5, $4, $2
call 
call $3
call label, $3
stop $3

.entry externLabel
ignored: .entry labelNotExist
.extern externLabel

