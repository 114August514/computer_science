; 寄存器分配情况：
; R0 用以参数输入
; R1 用以动态改变注入的字符串
; R2 用以保存恶意程序地址

; 主程序
                .ORIG	x3000
MAIN            
                LD  R2, ATTACK_ADDR
                AND R1, R1, #0
                ADD R1, R1, #1
                LEA R0, FOO
                STR R1, R0, #19                 ; 初始化

                TRAP x30
                PUTSP                           ; 攻击程序
                
                HALT                            ; 保护程序

MAIN_CONSTANT   
    ATTACK_ADDR .FILL   x4000
    FOO         .STRINGZ "0123456789012345678"  ; 加上 '\0' 合计 20 个字符
                
                ; 以下是注入的程序
                JSRR R2
    DONE        .FILL	x0000                   ; 虽然内存的默认值好像就是 x0000
                .END

; 陷阱向量表
                .ORIG	x30
HIJACK_ADDR     .FILL   x930                    ; TSR_HIJACK 服务历程位置  
                .END

; TRAP 实际服务历程
                .ORIG	x930
TSR_HIJACK      ; 替换 IN 历程的提示词
                ; 输入：R0 = 替换的字符串的首地址
                ; 输出：无
                ; 用到的寄存器都被保护了
                ST R0, SAVED_R0
                ST R1, SAVED_R1
                ST R2, SAVED_R2

                LD R1, PROMPT_ADDR
LOOP            LDR R2, R0, #0
                STR R2, R1, #0
                BRz LEAVE                       ; 根据 LDR 得到的数据是否为 '\0' 判断是否结束程序
                ADD R0, R0, #1
                ADD R1, R1, #1
                BRnzp LOOP

LEAVE           LD R0, SAVED_R0
                LD R1, SAVED_R1
                LD R2, SAVED_R2
                RTI

HIJACK_CONSTANT
    PROMPT_ADDR .FILL	x032C
    SAVED_R0    .BLKW	1
    SAVED_R1    .BLKW	1
    SAVED_R2    .BLKW	1
                .END

; 恶意程序
                .ORIG   x4000
; ATTACK        ; 非法读取系统空间的 x0001 的内存
                ; 如果不抛出 ACV 异常则通过
                LDI R0, ADDR
                LEA R0, WOW
                PUTS
                HALT

ATTACK_CONSTANT
    ADDR        .FILL   x1
    WOW         .STRINGZ "I made it!"
                .END