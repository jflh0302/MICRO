	         AREA    Constantes, DATA, READONLY   ; Área de datos de solo lectura
RCC_APB2ENR_ADDR    EQU     0x40021018       ; Registro de habilitación de reloj (APB2)
GPIOC_CRL_ADDR      EQU     0x40011000       ; Configuración baja de puerto C
GPIOC_CRH_ADDR      EQU     0x40011004       ; Configuración alta de puerto C
GPIOC_IDR_ADDR      EQU     0x40011008       ; Datos de entrada del puerto C
GPIOC_ODR_ADDR      EQU     0x4001100C       ; Datos de salida del puerto C
GPIOC_BSRR_ADDR     EQU     0x40011010       ; Registro de set/reset
GPIOC_BRR_ADDR      EQU     0x40011014       ; Registro de reset
SRAM_BASE_ADDR      EQU     0x20000100       ; Memoria base para almacenamiento
FLAG_GENERATED_ADDR EQU     0x20000000       ; Bandera de estado de números generados

        AREA    PRO3, CODE, READONLY
        ENTRY
        EXPORT  __main

; --- Inicio del programa ---
__main
        ; Encender reloj del puerto C
        LDR     R1, =RCC_APB2ENR_ADDR
        MOV     R0, #0x10
        STR     R0, [R1]

        ; Configurar PC0 y PC1 como entradas
        LDR     R1, =GPIOC_CRL_ADDR
        LDR     R0, [R1]
        BIC     R0, R0, #(0x0F + 0xF0)   ; Limpia campos de PC0 y PC1
        STR     R0, [R1]

        ; Configurar PC13 como salida push-pull
        LDR     R1, =GPIOC_CRH_ADDR
        LDR     R0, [R1]
        BIC     R0, R0, #0x00F00000
        ORR     R0, R0, #0x00200000
        STR     R0, [R1]

        ; LED apagado inicialmente
        LDR     R1, =GPIOC_BRR_ADDR
        MOV     R0, #0x2000
        STR     R0, [R1]

        B       inicio

; --- Estado: Inicio ---
inicio
        ; LED apagado
        LDR     R1, =GPIOC_BRR_ADDR
        MOV     R0, #0x2000
        STR     R0, [R1]

        ; Reset de bandera
        LDR     R1, =FLAG_GENERATED_ADDR
        MOV     R0, #0
        STR     R0, [R1]

        B       entradas

; --- Estado: Entradas ---
entradas
        LDR     R1, =GPIOC_IDR_ADDR
        LDR     R0, [R1]
        AND     R0, R0, #0x03
        CMP     R0, #0x00
        BEQ     inicio
        CMP     R0, #0x01
        BEQ     opcion1
        CMP     R0, #0x02
        BEQ     opcion2
        B       entradas

; --- Estado: Opción 1 (Generar números) ---
opcion1
        ; LED encendido
        LDR     R1, =GPIOC_BSRR_ADDR
        MOV     R0, #0x2000
        STR     R0, [R1]

        ; Bandera en 1
        LDR     R1, =FLAG_GENERATED_ADDR
        MOV     R0, #1
        STR     R0, [R1]

        ; Generador simple de 100 números
        LDR     R2, =SRAM_BASE_ADDR
        MOV     R3, #0
        MOV     R4, #12345
        MOV     R5, #12345

genera_loop
        LSL     R4, R4, #5
        ADD     R4, R4, R5
        AND     R4, R4, #0xFF
        STRB    R4, [R2], #1
        ADD     R3, R3, #1
        CMP     R3, #100
        BNE     genera_loop

op1
        LDR     R1, =GPIOC_IDR_ADDR
        LDR     R0, [R1]
        AND     R0, R0, #0x03
        CMP     R0, #0x00
        BEQ     inicio
        B       op1

; --- Estado: Opción 2 (Ordenar) ---
opcion2
        ; Verifica si ya se generaron datos
        LDR     R1, =FLAG_GENERATED_ADDR
        LDR     R0, [R1]
        CMP     R0, #0
        BEQ     inicio

        ; Ordenamiento por selección
        LDR     R2, =SRAM_BASE_ADDR   ; inicio del arreglo
        MOV     R3, #99               ; 100 elementos - 1

outer_sort
        LDR     R4, =SRAM_BASE_ADDR
        ADD     R4, R4, R3            ; límite superior de esta pasada
        MOV     R5, R2                ; posición del mínimo
        MOV     R6, R2                ; puntero de exploración

inner_sort
        ADD     R6, R6, #1            ; avanzar exploración
        CMP     R6, R4
        BHI     fin_inner             ; salir si pasamos el límite

        LDRB    R7, [R6]              ; elemento actual
        LDRB    R8, [R5]              ; elemento mínimo guardado
        CMP     R7, R8
        BGE     inner_sort            ; si R7 >= R8, seguir buscando
        MOV     R5, R6                ; nuevo mínimo encontrado
        B       inner_sort

fin_inner
        ; Intercambio: [R2] <-> [R5]
        LDRB    R7, [R2]
        LDRB    R8, [R5]
        STRB    R8, [R2]
        STRB    R7, [R5]

        ADD     R2, R2, #1            ; siguiente posición base
        SUB     R3, R3, #1
        CMP     R3, #0
        BPL     outer_sort            ; repetir hasta ordenar todo

        ; LED encendido y bandera activa
        LDR     R1, =GPIOC_BSRR_ADDR
        MOV     R0, #0x2000
        STR     R0, [R1]
        LDR     R1, =FLAG_GENERATED_ADDR
        MOV     R0, #1
        STR     R0, [R1]

op2
        LDR     R1, =GPIOC_IDR_ADDR
        LDR     R0, [R1]
        AND     R0, R0, #0x03
        CMP     R0, #0x00
        BEQ     inicio
        CMP     R0, #0x02
        BEQ     op2
        B       opcion1

; --- Fin del programa ---
        END

