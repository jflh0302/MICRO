#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include "fonts.h"

// --- Definiciones de configuraci�n ---
#define SSD1306_I2C_ADDR        0x78 // Direcci�n I2C del display (0x3C << 1)
#define SSD1306_WIDTH           128  // Ancho en p�xeles
#define SSD1306_HEIGHT          64   // Alto en p�xeles

// --- Funciones p�blicas ---

// Inicializa el controlador SSD1306
void ssd1306_init(void);

// Limpia el buffer de la pantalla
void ssd1306_clear(void);

// Dibuja un p�xel en el buffer en la posici�n (x, y)
void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color);

// Dibuja un caracter en la posici�n (x, y) usando una fuente espec�fica
char ssd1306_draw_char(char ch, uint8_t x, uint8_t y, FontDef Font);

// Dibuja una cadena de texto
void ssd1306_draw_string(const char* str, uint8_t x, uint8_t y, FontDef Font);

// Env�a el buffer de la pantalla a la memoria del display para mostrarlo
void ssd1306_update_screen(void);

// Dibuja un bitmap de pantalla completa (128x64)
void ssd1306_draw_bitmap(const uint8_t* bitmap);

void ssd1306_set_display_mode(uint8_t inverted);

#endif // SSD1306_H