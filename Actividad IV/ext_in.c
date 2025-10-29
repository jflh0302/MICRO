#include "stm32f10x.h"
#include "ext_in.h" 
#include "leds.h"   

// 'volatile' evita que el compilador haga optimizaciones incorrectas.
volatile int16_t int0_flag = 0;

void EXTI0_IRQHandler(void) {
    // 1. Limpiar el bit pendiente de la interrupción.
    EXTI->PR = (1 << 0); 

    // 2. Establecer nuestro flag de software para que el bucle main() sepa que ocurrió la interrupción.
    int0_flag = 1;
}

void init_ext_in(void) {
    // 1. Habilitar el reloj para el puerto B y para las funciones alternas (AFIO).
   
    RCC->APB2ENR |= (1 << 3) | (1 << 0); // Habilitar reloj para GPIOB y AFIO

    // 2. Configurar el pin PB0 como entrada con pull-down.
    GPIOB->CRL &= ~(0xF << 0); // Limpiar la configuración del pin 0
    GPIOB->CRL |= (0x8 << 0);  // CNF=10 (Input with pull-up/pull-down), MODE=00 (Input mode)
    GPIOB->ODR &= ~(1 << 0);   // Seleccionar pull-down para PB0

    // 3. Conectar la línea EXTI0 al pin PB0.
    AFIO->EXTICR[0] &= ~(0xF << 0); // Limpiar la configuración
    AFIO->EXTICR[0] |= (0x1 << 0);  // Seleccionar Puerto B (0001) para EXTI0

    // 4. Configurar la línea EXTI0.
    EXTI->IMR |= (1 << 0);    // Habilitar (no enmascarar) la interrupción en la línea 0.
    EXTI->RTSR |= (1 << 0);   // Configurar para que se dispare en el flanco de subida (Rising edge).
    EXTI->FTSR &= ~(1 << 0);  // Deshabilitar el flanco de bajada (Falling edge).

    // 5. Habilitar la interrupción en el controlador de interrupciones (NVIC).
    NVIC_EnableIRQ(EXTI0_IRQn);
}
