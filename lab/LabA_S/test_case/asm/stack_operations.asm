.ORIG x3000

; Stack starts at x4000
LD R6, STACK_TOP

; Push 10
AND R0, R0, #0
ADD R0, R0, #10
ADD R6, R6, #-1
STR R0, R6, #0

; Push 20
AND R0, R0, #0
ADD R0, R0, #15
ADD R0, R0, #5
ADD R6, R6, #-1
STR R0, R6, #0

; Pop (Should be 20)
LDR R1, R6, #0
ADD R6, R6, #1
ADD R2, R1, #-16
ADD R2, R2, #-4
BRnp FAIL

; Pop (Should be 10)
LDR R1, R6, #0
ADD R6, R6, #1
ADD R2, R1, #-10
BRnp FAIL

LEA R0, PASS_MSG
PUTS
HALT

FAIL
LEA R0, FAIL_MSG
PUTS
HALT

STACK_TOP .FILL x4000
PASS_MSG .STRINGZ "Stack Ops Test Passed\n"
FAIL_MSG .STRINGZ "Stack Ops Test Failed\n"

.END
