; 寄存器分配情况：
; R7 用以存储子程序，不调用
; R0 用以状态值比较
; R1 用以存储 键盘状态值/显示器状态值
; R2 用以存储 键盘数据值
; R3 用以存储 'y' 的 ASCII 值的负表示
; R4 用以存储 '0' 的 ASCII 值的负表示
; R5 用以进行 mod7 运算的结果存储
; R6 用以存储 '?' 的 ASCII 值
            .ORIG x3000
            
; 程序段
MAIN        ; 主函数
            ; 初始化
            LEA R0, CONSTANTS
            LDR R3, R0, #0
            LDR R4, R0, #1
            LDR R6, R0, #2; R6 初始化为 '?' 的 ASCII 值
            NOT R3, R3
            NOT R4, R4
            ADD R3, R3, #1; R3 用以存储 'y' 的 ASCII 值的负表示
            ADD R4, R4, #1; R4 用以存储 '0' 的 ASCII 值的负表示
            AND R5, R5, #0; R5 初始化为 #0
            
MAIN_LOOP   ; 主循环函数
            JSR GET_CHAR
            
            ; 判断逻辑
IS_y        ; 是否是 'y'
            ADD R0, R2, R3
            BRz PRINT
            
IS_0        ; 是否是 '0'
            ADD R0, R2, R4
            BRz MOD7_ZERO
            
IS_1        ; 是否是 '1'    
            ADD R0, R0, #-1
            BRz MOD7_ONE
            
            ; 若都不符合则提示
            JSR HINT_ERROR

; 预存储区域
CONSTANTS
    ASCII_y .FILL x0079; 'y' 的 ASCII 值
    ASCII_0 .FILL x0030; '0' 的 ASCII 值
    ASCII_? .FILL x003F; '?' 的 ASCII 值    
            .END


; 程序子段
            .ORIG x3050
; 子程序 1
GET_CHAR    ; 获取键盘值
WAIT_READ   ; 等待按键
            LDI R1, KBSR
            BRzp WAIT_READ; 非负数说明最高位为 0
            
READ_CHAR   ; 读取按键
            LDI R2, KBDR
            RET

; 子程序 2     
PRINT       ; 打印到显示器
WAIT_WRITE  ; 等待显示器就绪
            LDI R1, DSR
            BRzp WAIT_WRITE
WRITE_CHAR  ; 打印到显示器上
            LD R0, SUB_ASCII_0
            ADD R5, R5, R0; 将数字转换成 ASCII 码
            STI R5, DDR
            HALT

; 子程序 3
MOD7        ; 计算 Modulo 7 运算
MOD7_ZERO   ; 计算输入为 0 的情况
            ADD R5, R5, R5; R5 = R5 * 2
            BRnzp CHECK_MOD

MOD7_ONE    ; 计算输入为 1 的情况
            ADD R5, R5, R5
            ADD R5, R5, #1; R5 = R5 * 2 + 1
            BRnzp CHECK_MOD

CHECK_MOD   ; 用以检查是否 >7
            ADD R0, R5, #-7; R0 = R5 - 7
            BRn LESS_THAN_7
            ADD R5, R0, #0
LESS_THAN_7 BRnzp MAIN_LOOP
          
; 子程序 4
HINT_ERROR  ; 提示输入错误
WAIT_HINT   ; 等待显示器就绪
            LDI R1, DSR
            BRzp WAIT_HINT
WRITE_HINT  ; 写入提示
            STI R6, DDR
            BRnzp MAIN_LOOP

; 预存储地址            
ADDRESS
    KBSR        .FILL xFE00; 键盘状态寄存器地址 (其最高位（第 15 位）为 1 时表示有按键按下。)
    KBDR        .FILL xFE02; 键盘数据寄存器地址 (其最高位（第 15 位）为 1 时表示显示器就绪。)
    DSR         .FILL xFE04; 显示器状态寄存器地址
    DDR         .FILL xFE06; 显示器数据寄存器地址
    SUB_ASCII_0 .FILL x0030; 子程序使用的 '0' ，用以将数字转换成字符。

            .END
    