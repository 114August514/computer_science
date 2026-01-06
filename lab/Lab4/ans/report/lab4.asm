; 寄存器分配情况：
; R0 用以存储函数第一个参数和返回值
; R1 用以存储函数第二个参数
; R0-R2 装载函数的临时变量
; R4 用以暂时存储 STEPS(N, M)的值
; R5 用以存储 FP
; R6 用以存储 SP
; R7 用以存储返回地址

; 主程序
            .ORIG	x3000
MAIN                
            LEA R2, CONSTANTS; 初始化
            LDR R0, R2, #0
            LDR R1, R2, #1
            LDR R6, R2, #3

            ; 计算 STEPS(N, M)
            ADD R4, R0, R1; R4 = STEPS(N, M)

            ; 计算 ROUTES(N, M)
            JSR CAL_ROUTES; R0 = ROUTES(N, M)

            ; 计算 REC(N, M)
            ADD R1, R4, #0; R1 = STEPS(N, M)
            JSR CAL_REC
            HALT

            .END

; 常量表及子程序
            .ORIG x3100
CONSTANTS
    INPUT_N     .FILL #5; N (0 <= N <= 5)
    INPUT_M     .FILL #5; M (0 <= M <= 5)
    ANS_ADDR    .FILL x3200; 答案存储位置
    STACK_INIT  .FILL x6000; 栈起始位置

; 程序栈的实现
STACK
CAL_ROUTES  ; 递归计算 ROUTES(N, M)
            ; 输入：R0 = N, R1 = M
            ; 输出：R0 = ROUTES(N, M)

            ; 保存返回地址和帧指针
            ADD R6, R6, #-1
            STR R7, R6, #0
            ADD R6, R6, #-1
            STR R5, R6, #0; 一开始进来的 R5 是个未定义的值，不过应该不会调用它

            ; 创建新的帧指针（当前函数调用的帧指针）
            ADD R5, R6, #0

            ; 存储局部变量
            ADD R6, R6, #-1
            STR R0, R5, #-1
            ADD R6, R6, #-1
            STR R1, R5, #-2

            ; 递归退出条件
            ADD R0, R0, #0
            BRz BASE
            ADD R1, R1, #0
            BRz BASE
            BRnzp RECURSE

BASE        ; 基本情况： N == 0 || M == 0 时，return 1
            AND R0, R0, #0
            ADD R0, R0, #1
            BRnzp CLEAN_UP

RECURSE     ; 递归调用 ROUTES(N, M) = ROUTES(N - 1, M) + ROUTES(N, M - 1)
            ADD R0, R0, #-1; R0 = N - 1, R1 = M
            JSR CAL_ROUTES
            ADD R2, R0, #0; R2 = ROUTES(N - 1, M)
            ADD R6, R6, #-1
            STR R2, R5, #-3
            
            LDR R0, R5, #-1
            LDR R1, R5, #-2
            ADD R1, R1, #-1; R0 = N, R1 = M - 1
            JSR CAL_ROUTES; R0 = ROUTES(N, M - 1)

            LDR R2, R5, #-3
            ADD R0, R0, R2
            BRnzp CLEAN_UP

CLEAN_UP    ; 清理栈帧
            ADD R6, R5, #0; 清空局部变量
            LDR R5, R6, #0
            ADD R6, R6, #1; 恢复之前的帧指针（调用函数的）

            LDR R7, R6, #0
            ADD R6, R6, #1; 恢复返回地址
            RET

; 简单子程序的实现（不写程序栈）
CAL_REC     ; 计算推荐值
            ; 输入：R0 = ROUTES(N, M), R1 = STEPS(N, M)
            ; 输出：无输出（将最终值存储在 ANS_ADDR）。
            AND R2, R2, #0
            AND R3, R3, #0
            ADD R3, R3, #5

            NOT R1, R1
            ADD R1, R1, #1; R1 = -STEPS(N, M)

MULT_LOOP   ; 实现 R2 = R0 * R3，即 R2 = ROUTES(N, M) * 5   
            ADD R2, R2, R0
            ADD R3, R3, #-1
            BRnz DONE
            BRnzp MULT_LOOP

DONE
            ADD R0, R1, R2
            STI R0, ANS_ADDR
            RET

            .END