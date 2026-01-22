.ORIG x3000

; --- Test AND Register Mode ---
; R1 = xAAAA (1010...), R2 = x5555 (0101...) -> R3 = R1 AND R2 = 0
LD R1, VAL_A
LD R2, VAL_5
AND R3, R1, R2
BRnp FAIL       ; Result should be 0 (Z set)

; R1 = xFFFF, R2 = x1234 -> R3 = x1234
LD R1, VAL_F
LD R2, VAL_1234
AND R3, R1, R2
; ADD R4, R3, #0  ; Update CC
BRz FAIL        ; Should not be zero
NOT R4, R3
ADD R4, R4, #1  ; R4 = -R3
ADD R4, R4, R2  ; R4 = -R3 + R2 = 0 if equal
BRnp FAIL

; --- Test AND Immediate Mode ---
; R1 = xFFFF AND #0 -> 0
LD R1, VAL_F
AND R1, R1, #0
BRnp FAIL       ; Should be 0 and Z set

; R1 = x000F AND #5 (0101) -> 5
AND R1, R1, #0
ADD R1, R1, #15 ; x000F
AND R1, R1, #5
ADD R4, R1, #-5
BRnp FAIL

; --- Test NOT Instruction ---
; NOT x0000 -> xFFFF
AND R1, R1, #0
NOT R1, R1
ADD R1, R1, #1  ; xFFFF + 1 = 0
BRnp FAIL

; Success
LEA R0, PASS_MSG
PUTS
HALT

FAIL
LEA R0, FAIL_MSG
PUTS
HALT

VAL_A .FILL xAAAA
VAL_5 .FILL x5555
VAL_F .FILL xFFFF
VAL_1234 .FILL x1234

PASS_MSG .STRINGZ "AND/NOT Test Passed\n"
FAIL_MSG .STRINGZ "AND/NOT Test Failed\n"

.END
