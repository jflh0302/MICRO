#include "stm32f10x.h"
#include "gpio.h"

void init_ports(void) {
    // 1. Habilitar relojes para GPIOB y Funciones Alternas 
    RCC->APB2ENR |= (1 << 3) | (1 << 0); // Habilitar reloj para GPIOB y AFIO

    // 2. Configurar pines I2C2 (PB10: SCL, PB11: SDA)
    GPIOB->CRH &= ~((0xF << 8) | (0xF << 12)); 

    // Establecer la nueva configuración para PB10 (SCL)
    GPIOB->CRH |= (0xF << 8);

    // Establecer la nueva configuración para PB11 (SDA)
    GPIOB->CRH |= (0xF << 12);


}