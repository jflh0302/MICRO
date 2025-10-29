#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

// Inicializa el perif�rico I2C2 en modo est�ndar (100kHz).
void init_i2c(void);

// Genera una condici�n de START en el bus I2C.
void start_i2c(void);

//Genera una condici�n de STOP en el bus I2C.
void stop_i2c(void);

// Env�a un byte de datos al bus I2C.
void data_i2c(uint8_t data);

// Env�a la direcci�n de un esclavo y el bit de lectura/escritura.
void address_i2c(uint8_t address, uint8_t r_w);

// Escribe una secuencia de bytes a un esclavo I2C.
void write_i2c(uint8_t address, uint8_t data[], uint64_t length);

#endif /* __I2C_H__ */