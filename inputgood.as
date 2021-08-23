.extern exLabel
    ; comment line

labelShouldNotdefine: .entry checkIfDefinedHere
labelShouldNotdefine: .entry checkIfDefinedHere2

checkIfDefinedHere: add $4, $3, $5
checkIfDefinedHere2: .asciz "123 567 9"

.dw 1,1,0
.db 45, -98

label: .asciz "four"
label2: move $2, $31
MMMM:         ori $19     ,	-23458	, $0

jmp label2
call label
END: stop

blt $4, $30, END

;end of file
