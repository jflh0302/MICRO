; Definici�n de registros
numero  rn r3  ; n�mero l�mite (n = 47)
contar  rn r4    ; �ndice de ciclo
TR    rn r5    ; puntero a memoria
; r6= (�ltimo Fibonacci calculado, F(n)) r7= �ltimo numero usado en la suma)
;Calcular la serie de finobacci para el numero 20, en la localidad del resultado para f=12, rellenar con blancos
; �rea de programa
; ----------------
        area l_fibo, code, readonly
        entry
        export __main

__main
        ; cargar direcci�n inicial de SRAM
        ldr TR, =0x20000015

        ; l�mite n = 47
        mov numero, #20

        ; F(0) = 0
        mov contar, #0
        str contar, [TR], #4      ; guardar F(0)

        ; F(1) = 1
        mov contar, #1
        str contar, [TR], #4      ; guardar F(1)

        ; ahora count ser� el �ndice i = 2
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

        ; incrementar �ndice
        add contar, contar, #1
        cmp contar, numero
        ble ciclo
        ldr  r8,=0x20000015
        mov  r9, #20
		str  r9,[r8,#48]
fin
        b fin
        end
