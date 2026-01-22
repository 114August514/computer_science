.ORIG x3000

; --- Test LD (PC-Relative) ---
; Forward reference
LD R1, DATA_FWD
ADD R2, R1, #-10
BRnp FAIL

; Backward reference
BR SKIP_BACK
DATA_BACK .FILL #20
SKIP_BACK
LD R1, DATA_BACK
ADD R2, R1, #-16
ADD R2, R2, #-4
BRnp FAIL

; --- Test ST (PC-Relative) ---
AND R1, R1, #0
ADD R1, R1, #5
ST R1, STORE_LOC
LD R2, STORE_LOC
ADD R3, R2, #-5
BRnp FAIL

; --- Test LDI (Indirect) ---
LDI R1, PTR_TO_DATA
LD R2, REAL_DATA
NOT R2, R2
ADD R2, R2, #1
ADD R3, R1, R2
BRnp FAIL

; --- Test STI (Indirect) ---
AND R1, R1, #0
ADD R1, R1, #7
STI R1, PTR_TO_STORE
LDI R2, PTR_TO_STORE
ADD R3, R2, #-7
BRnp FAIL

; --- Test LDR (Base+Offset) ---
LEA R1, ARRAY_BASE
LDR R2, R1, #1      ; Load 2nd element
ADD R3, R2, #-2
BRnp FAIL

; --- Test STR (Base+Offset) ---
LEA R1, ARRAY_BASE
AND R2, R2, #0
ADD R2, R2, #9
STR R2, R1, #2      ; Store 9 into 3rd element
LDR R3, R1, #2
ADD R4, R3, #-9
BRnp FAIL

; --- Test LEA ---
LEA R1, DATA_FWD
LD R2, ADDR_DATA_FWD ;
LDR R3, R1, #0
LDR R4, R2, #0
NOT R4, R4
ADD R4, R4, #1
ADD R4, R4, R3
BRnp FAIL

; Success
LEA R0, PASS_MSG
PUTS
HALT

FAIL
LEA R0, FAIL_MSG
PUTS
HALT

DATA_FWD .FILL #10
STORE_LOC .BLKW 1
PTR_TO_DATA .FILL REAL_DATA
REAL_DATA .FILL #99
PTR_TO_STORE .FILL STORE_DEST
STORE_DEST .BLKW 1
ARRAY_BASE .FILL #1
           .FILL #2
           .FILL #3
ADDR_DATA_FWD .FILL DATA_FWD ; Assembler should resolve this

PASS_MSG .STRINGZ "Load/Store Test Passed\n"
FAIL_MSG .STRINGZ "Load/Store Test Failed\n"

.END
