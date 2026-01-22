.ORIG x3000

; --- Test ADD Register Mode ---
; R2 = 5, R3 = 3 -> R1 = R2 + R3 = 8
AND R2, R2, #0
ADD R2, R2, #5
AND R3, R3, #0
ADD R3, R3, #3
ADD R1, R2, R3
ADD R4, R1, #-8 ; Check if result is 8
BRnp FAIL

; --- Test ADD Immediate Mode (Max Positive #15) ---
; R1 = 0 + 15 = 15
AND R1, R1, #0
ADD R1, R1, #15
ADD R4, R1, #-15
BRnp FAIL

; --- Test ADD Immediate Mode (Min Negative #-16) ---
; R1 = 0 + (-16) = -16
AND R1, R1, #0
ADD R1, R1, #-16
ADD R4, R1, #15
ADD R4, R4, #1  ; Check if result is -16 (by adding 16)
BRnp FAIL

; --- Test Condition Codes (CC) ---

; Test Zero (Z)
AND R1, R1, #0
ADD R1, R1, #0  ; Result is 0
BRnp FAIL       ; Should be Z, so N and P branches should not take.

; Test Positive (P)
ADD R1, R1, #1  ; Result is 1
BRnz FAIL       ; Should be P

; Test Negative (N)
ADD R1, R1, #-2 ; Result is -1
BRzp FAIL       ; Should be N

; Success
LEA R0, PASS_MSG
PUTS
HALT

FAIL
LEA R0, FAIL_MSG
PUTS
HALT

PASS_MSG .STRINGZ "ADD Test Passed\n"
FAIL_MSG .STRINGZ "ADD Test Failed\n"

.END
