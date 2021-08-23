spaceInlabel  : adss
labelDefTwice: jmp spaceInLabel
labelDefTwice: jmp label
%badLabel: 
addBadLabel:
ascizBadLabel:

label: addddd
label2: .dwww

label3:.dw ,34
.dw 34, ,3
.dh 34 5, 4
.dh 34, 5 5, 5
.dh 34,
.dh 
.dh 34,
.dh 34, notANumber
label4:.asciz "dfdd
.asciz sdfs"
.asciz ssdf
.entry labelDoesntExist
.entry &notAlabel
.entry
.extern label2
.extern &notAlabel
.extern


