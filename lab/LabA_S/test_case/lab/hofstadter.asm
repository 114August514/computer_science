        .ORIG x3000


; --------------------------
; 初始化 Q(1)=1, Q(2)=1
; --------------------------
        LD   R0, Q_BASE     ;R0 = Q_BASE
        AND  R1, R1, #0
        ADD  R1, R1, #1     ;R1 = Q[n-1]
        ADD  R2, R1, #0     ;R2 = Q[n-2]
        STR  R1, R0, #0    ; Q[1] = 1
        STR  R2, R0, #1    ; Q[2] = 1

; --------------------------
; 取输入 n
; --------------------------
        LDI  R3, N_ADDR    ; R3 = n
        ;ADD  R3, R3, #10
        ; n <= 2 直接输出 1
        ADD  R4, R3, #-2    
        BRp  INIT_LOOP     ; n > 2 则进入循环
        BR   DONE

; --------------------------
; 计算 Q(3..n)
; --------------------------

INIT_LOOP
        AND  R4, R4, #0
        ADD  R4, R4, #2     ;R4 = count


LOOP
        
        ; ---------- i - Q(i-1) ----------
        ADD  R5, R4, #0
        NOT  R6, R2
        ADD  R6, R6, #1
        ADD  R5, R5, R6
        ADD  R5, R5, R0
        LDR  R5, R5, #0    ; R5 = Q(i - Q(i-1))

        ; ---------- i - Q(i-2) ----------
        ADD  R6, R4, #0
        NOT  R7, R1
        ADD  R7, R7, #1
        ADD  R6, R6, R7
        ADD  R6, R6, R0
        LDR  R6, R6, #0    ; R6 = Q(i - Q(i-2))

        ; ---------- Q(i) = R5 + R6 ----------
        ADD  R2, R1, #0;
        ADD  R1, R5, R6
        ADD  R5, R0, R4
        STR  R1, R5, #0    ; 存储 Q(i)

        ; i++
        ADD  R4, R4, #1
        
        NOT  R5, R4
        ADD  R5, R5, #1
        ADD  R6, R3, R5
        BRp   LOOP

; --------------------------
; 输出结果 Q(n)
; --------------------------
DONE
        STI   R1, RES_ADDR

        HALT
        
; --------------------------
; 符号定义
; --------------------------
N_ADDR   .FILL x3100       ; 输入 n 的地址
RES_ADDR .FILL x3101       ; 输出结果地址
Q_BASE   .FILL x3200       ; Q[1..100] 存放起始地址

        .END
