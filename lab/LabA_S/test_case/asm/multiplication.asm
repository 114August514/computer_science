.ORIG x3000

; Multiply R0 * R1 -> R2
; 6 * 7 = 42
AND R0, R0, #0
ADD R0, R0, #6
AND R1, R1, #0
ADD R1, R1, #7

AND R2, R2, #0  ; Result
ADD R0, R0, #0
BRz DONE        ; If R0 is 0, result is 0

MULT_LOOP
ADD R2, R2, R1
ADD R0, R0, #-1
BRp MULT_LOOP

DONE
; Check if 42
ADD R3, R2, #-16
ADD R3, R3, #-16
ADD R3, R3, #-10 ; -42
BRnp FAIL

LEA R0, PASS_MSG
PUTS
HALT

FAIL
LEA R0, FAIL_MSG
PUTS
HALT

PASS_MSG .STRINGZ "Multiplication Test Passed\n"
FAIL_MSG .STRINGZ "Multiplication Test Failed\n"

.END
