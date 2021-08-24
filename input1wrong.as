spaceInlabel  : adss
labelDefTwice: jmp spaceInLabel
labelDefTwice: jmp label
%badLabel: 
addBadLabel:
ascizBadLabel:
dataLabel:.asciz "123456789"
label: addddd
label2: .dwww
label5: extraLabel:
;label3: addddd
label3:.dw ,34
.dw 34, ,3
.dh 34 5, 4
.dh 34, 5 5, 5
.dh 34,
.dh 
.dh 34,
.dh 34, notANumber
.db 256
.db -257
label4:.asciz "dfdd
.asciz sdfs"
.asciz ssdf
.asciz 

.extern dataLabel
.extern &notAlabel
.extern


