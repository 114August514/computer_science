.ORIG x3000

; Calculate Factorial(5) = 120
; Input R0 = 5
; Output R1

LD R6, STACK_BASE
AND R0, R0, #0
ADD R0, R0, #5
JSR FACTORIAL

; Check result (120)
; 120 = x78
LD R2, VAL_NEG_120
ADD R3, R1, R2
BRnp FAIL

LEA R0, PASS_MSG
PUTS
HALT

FAIL
LEA R0, FAIL_MSG
PUTS
HALT

; Recursive Factorial
; Input R0
; Output R1
FACTORIAL
ADD R6, R6, #-1
STR R7, R6, #0  ; Save Return Address
ADD R6, R6, #-1
STR R0, R6, #0  ; Save R0

ADD R2, R0, #-1 ; Check base case R0 <= 1
BRnz BASE_CASE

; Recursive step
ADD R0, R0, #-1
JSR FACTORIAL   ; R1 = Factorial(N-1)

; Restore R0
LDR R0, R6, #0
; Multiply R1 = R1 * R0
; We need a multiply subroutine or loop here.
; Let's do a simple loop for R1 * R0 -> R1
; Save R2, R3
ADD R6, R6, #-1
STR R2, R6, #0
ADD R6, R6, #-1
STR R3, R6, #0

AND R2, R2, #0  ; Result accumulator
ADD R3, R0, #0  ; Counter = N
MULT_LOOP
ADD R2, R2, R1
ADD R3, R3, #-1
BRp MULT_LOOP
ADD R1, R2, #0  ; Update R1

; Restore regs
LDR R3, R6, #0
ADD R6, R6, #1
LDR R2, R6, #0
ADD R6, R6, #1

BR FACT_RET

BASE_CASE
AND R1, R1, #0
ADD R1, R1, #1

FACT_RET
LDR R0, R6, #0  ; Restore R0
ADD R6, R6, #1
LDR R7, R6, #0  ; Restore RA
ADD R6, R6, #1
RET

STACK_BASE .FILL x4000
VAL_NEG_120 .FILL #-120
PASS_MSG .STRINGZ "Recursion Test Passed\n"
FAIL_MSG .STRINGZ "Recursion Test Failed\n"

.END
