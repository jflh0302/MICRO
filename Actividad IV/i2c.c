#include "stm32f10x.h"
#include "i2c.h"

void init_i2c(void) {
    // 1. Habilitar el reloj para el periférico I2C2 en el bus APB1.
    RCC->APB1ENR |= (1 << 22);

    // 2. Resetear el periférico I2C para empezar desde un estado conocido.
    I2C2->CR1 |= (1 << 15);  // Poner I2C en estado de reset
    I2C2->CR1 &= ~(1 << 15); // Sacar I2C del estado de reset

    // 3. Configurar la frecuencia del reloj del bus (PCLK1).
    I2C2->CR2 = 8; // Escribimos '8' para indicar 8MHz

    // 4. Configurar la velocidad del reloj I2C (CCR - Clock Control Register).
    // CCR = 8,000,000 / 200,000 = 40
    I2C2->CCR = 40;
    
    // 5. Configurar el tiempo de subida (TRISE).
    // Para modo estándar (1000ns): (1000ns / 125ns) + 1 = 8 + 1 = 9
    I2C2->TRISE = 9;

    // 6. Habilitar el periférico I2C.
    I2C2->CR1 |= (1 << 0); // PE (Peripheral Enable)
}

void start_i2c(void) {
    // Generar una condición de START en el bus
    I2C2->CR1 |= (1 << 8); 
    // Esperar a que el flag SB (Start Bit) se ponga en alto, indicando que el START se envió
    while (!(I2C2->SR1 & I2C_SR1_SB));
}

void stop_i2c(void) {
    // Generar una condición de STOP en el bus
    I2C2->CR1 |= (1 << 9);
}

void address_i2c(uint8_t address, uint8_t r_w) {
    // Enviar la dirección de 7 bits del esclavo, desplazada un bit a la izquierda,
    // y añadir el bit de Lectura/Escritura (0 para escribir).
    I2C2->DR = (address | r_w);
    
    // Esperar a que el flag ADDR (Address sent) se ponga en alto.
    // Esto confirma que el esclavo ha reconocido (ACK) su dirección.
    while (!(I2C2->SR1 & I2C_SR1_ADDR));

    // Esto se hace mediante una lectura del registro SR1 (ya hecha en el 'while')
    // seguida de una lectura del registro SR2.
    (void)I2C2->SR2;
}

void data_i2c(uint8_t data) {
    // Esperar a que el flag TxE (Transmit buffer empty) sea 1.
    while (!(I2C2->SR1 & I2C_SR1_TXE));
    
    // Cargar el byte de datos en el registro de datos (DR).
    I2C2->DR = data;
}

void write_i2c(uint8_t address, uint8_t data[], uint64_t length) {
    uint64_t i = 0;
    
    start_i2c();
    address_i2c(address, 0); // 0 = Modo escritura

    // Enviar todos los bytes del array
    for (i = 0; i < length; i++) {
        data_i2c(data[i]);
    }

    // Esto asegura que el último byte se haya transmitido completamente
    // antes de enviar la señal de STOP.
    while (!(I2C2->SR1 & I2C_SR1_TXE) || !(I2C2->SR1 & I2C_SR1_BTF));

    stop_i2c();
}