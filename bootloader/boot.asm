[BITS 16]
[ORG 0x7C00]

MOV SI, HelloString
CALL PrintString
HLT

PrintString:
next_character:
MOV AL, [SI]
OR AL, AL
JZ exit_function
CALL PrintCharacter
INC SI
JMP next_character
exit_function:
RET

PrintCharacter:
MOV AH, 0x0E
MOV BH, 0x00
MOV BL, 0x07
INT 0x10
RET

HelloString db 'Carlos F. Almonte Ibarra', 0

TIMES 510 - ($ - $$) DB 0 

DW 0xAA55

