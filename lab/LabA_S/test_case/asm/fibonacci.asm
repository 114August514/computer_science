.ORIG x3000

; Calculate Fibonacci(N)
; Input: N in R0 (e.g., 6)
; Output: Result in R1
; F(0)=0, F(1)=1, F(2)=1, F(3)=2, F(4)=3, F(5)=5, F(6)=8

AND R0, R0, #0
ADD R0, R0, #6  ; Calculate F(6)

AND R1, R1, #0  ; Current (F_n)
AND R2, R2, #0  ; Previous (F_n-1)
ADD R2, R2, #1  ; Init F(1) = 1
ADD R1, R1, #0  ; Init F(0) = 0

ADD R0, R0, #-1 ; If N=0, result 0. If N=1, loop once?
BRn DONE        ; N=0 -> R1=0

; Logic:
; F(0) = 0
; F(1) = 1
; F(2) = F(1)+F(0) = 1
; ...
; Let's do iterative:
; a = 0, b = 1
; loop N times:
;   temp = a + b
;   a = b
;   b = temp
; Result is a (for F(N))? No, usually F(N) is the Nth number.
; F0=0, F1=1, F2=1, F3=2, F4=3, F5=5, F6=8.
; If N=6, we want 8.

; Reset
AND R0, R0, #0
ADD R0, R0, #6  ; N=6

AND R1, R1, #0  ; a = 0
AND R2, R2, #0
ADD R2, R2, #1  ; b = 1
ADD R0, R0, #0
BRz DONE        ; F(0) = 0

LOOP
ADD R3, R1, R2  ; temp = a + b
ADD R1, R2, #0  ; a = b
ADD R2, R3, #0  ; b = temp
ADD R0, R0, #-1
BRp LOOP

DONE
; Check if R1 == 8
ADD R4, R1, #-8
BRnp FAIL

LEA R0, PASS_MSG
PUTS
HALT

FAIL
LEA R0, FAIL_MSG
PUTS
HALT

PASS_MSG .STRINGZ "Fibonacci Test Passed\n"
FAIL_MSG .STRINGZ "Fibonacci Test Failed\n"

.END
