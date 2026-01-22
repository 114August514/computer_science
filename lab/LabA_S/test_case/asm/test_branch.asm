.ORIG x3000

; --- Test BRn ---
AND R1, R1, #0
ADD R1, R1, #-1 ; N set
BRn PASS_N
BR FAIL
PASS_N

; --- Test BRz ---
AND R1, R1, #0  ; Z set
BRz PASS_Z
BR FAIL
PASS_Z

; --- Test BRp ---
AND R1, R1, #0
ADD R1, R1, #1  ; P set
BRp PASS_P
BR FAIL
PASS_P

; --- Test BRnz (with N) ---
AND R1, R1, #0
ADD R1, R1, #-1
BRnz PASS_NZ_1
BR FAIL
PASS_NZ_1

; --- Test BRnz (with Z) ---
AND R1, R1, #0
BRnz PASS_NZ_2
BR FAIL
PASS_NZ_2

; --- Test BRzp (with Z) ---
AND R1, R1, #0
BRzp PASS_ZP_1
BR FAIL
PASS_ZP_1

; --- Test BRzp (with P) ---
AND R1, R1, #0
ADD R1, R1, #1
BRzp PASS_ZP_2
BR FAIL
PASS_ZP_2

; --- Test BRnp (with N) ---
AND R1, R1, #0
ADD R1, R1, #-1
BRnp PASS_NP_1
BR FAIL
PASS_NP_1

; --- Test BRnp (with P) ---
AND R1, R1, #0
ADD R1, R1, #1
BRnp PASS_NP_2
BR FAIL
PASS_NP_2

; --- Test BR (Unconditional) ---
BR PASS_ALL
BR FAIL


FAIL
LEA R0, FAIL_MSG
PUTS
HALT

PASS_ALL

; Success
LEA R0, PASS_MSG
PUTS
HALT

PASS_MSG .STRINGZ "Branch Test Passed\n"
FAIL_MSG .STRINGZ "Branch Test Failed\n"

.END
