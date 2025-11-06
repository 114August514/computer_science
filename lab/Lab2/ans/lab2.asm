; 说明 R1, R2 用来存 Q[n-2], Q[n-1]; R5, R6 用来存 -R1, -R2
            .ORIG   x3000
MAIN        AND R2, R2, #0
            ADD R2, R2, #1; R2 = 1

            LD R3, N; R3 = N
            ADD R3, R3, #-2; R3 = N - 2
            
            ; 非正，短逻辑处理
            BRnz DONE

            ; 常规逻辑
PROCESS     AND R4, R4, #0
            ADD R4, R4, #3; R4 = 3
            ADD R1, R2, #0; R1 = 1
            LEA R0, Q_ARRAY; R0 用以记录访问到数组值的位置
            ADD R0, R0, #-1; 方便后续操作
            
            ; 初始化 Q[1], Q[2]
            STR R1, R0, #1; Q[1] = 1
            STR R2, R0, #2; Q[2] = 1
            
            ADD R3, R3, #0
LOOP        BRnz DONE
            NOT R5, R1
            ADD R5, R5, #1; R5 = -Q[n-2]
            NOT R6, R2
            ADD R6, R6, #1; R6 = -Q[n-1]
            
            ADD R5, R4, R5; R5 = n - Q[n-2]
            ADD R6, R4, R6; R6 = n - Q[n-1]
            
            ADD R5, R0, R5
            ADD R6, R0, R6
            LDR R5, R5, #0; R5 = Q[n - Q[n-2]]
            LDR R6, R6, #0; R6 = Q[n - Q[n-1]]
            
            ADD R1, R2, #0; R1 = Q[n-1]
            ADD R2, R5, R6; R2 = Q[n]
            
            ADD R7, R0, R4
            STR R2, R7, #0; 储存 Q[n]
            
            ADD R4, R4, #1
            ADD R3, R3, #-1
            BRnzp LOOP

            ; 结束
DONE        ST R2, ANS
            TRAP x25; HALT
            .END


            .ORIG   x3100; 数据存储区域
DATA_AREA
    N       .FILL   #100; 仅允许输入 #1 - #100;
    ANS     .FILL   #0;
    Q_ARRAY .BLKW   100
    
            .END