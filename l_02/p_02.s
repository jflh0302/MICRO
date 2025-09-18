
; Definición de registros
numero  rn r3  ; número límite (n = 47)
contar  rn r4    ; índice de ciclo
TR    rn r5    ; puntero a memoria

; Área de programa
; ----------------
        area l_fibo, code, readonly
        entry
        export __main

__main
        ; cargar dirección inicial de SRAM
        ldr TR, =0x20000000

        ; límite n = 47
        mov numero, #47

        ; F(0) = 0
        mov contar, #0
        str contar, [TR], #4      ; guardar F(0)

        ; F(1) = 1
        mov contar, #1
        str contar, [TR], #4      ; guardar F(1)

        ; ahora count será el índice i = 2
        mov contar, #2

ciclo
        ; cargar F(i-1)
        ldr r6, [TR, #-4]

        ; cargar F(i-2)
        ldr r7, [TR, #-8]

        ; sumar => F(i) = F(i-1) + F(i-2)
        add r6, r6, r7

        ; guardar en memoria
        str r6, [TR], #4

        ; incrementar índice
        add contar, contar, #1
        cmp contar, numero
        ble ciclo

fin
        b fin
        end
