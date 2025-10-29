#include "leds.h"       
#include "stm32f10x.h"  

/**
 * @brief Enciende y apaga el LED conectado a PC13 varias veces.
 */
void led_on_off(void) {
    int x;
    // Bucle para hacer parpadear el LED 4 veces (8 cambios de estado)
    for (x = 0; x < 8; x++) {
        GPIOC->ODR ^= (1 << 13); // Invierte el estado del pin PC13
        delay_ms(100);           // Espera 100 milisegundos
    }
}

void delay_ms(volatile uint32_t ms) {
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 6000; j++);
    }
}