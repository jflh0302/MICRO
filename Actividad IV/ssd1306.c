#include "ssd1306.h"
#include "i2c.h"
#include <string.h> 

// --- Definiciones Privadas ---

// El buffer de memoria que representa la pantalla. Cada bit es un píxel.
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// --- Funciones de Comunicación (Privadas) ---

// Envía un byte de comando al SSD1306
static void ssd1306_send_command(uint8_t command) {
    uint8_t data[2] = {0x00, command}; // 0x00 indica que lo que sigue es un comando
    write_i2c(SSD1306_I2C_ADDR, data, 2);
}

void ssd1306_set_display_mode(uint8_t inverted) {
    if (inverted) {
        // Comando 0xA7: Invert Display (ON -> OFF, OFF -> ON)
        ssd1306_send_command(0xA7);
    } else {
        // Comando 0xA6: Normal Display (El estado por defecto)
        ssd1306_send_command(0xA6);
    }
}
// Envía el buffer de datos completo a la RAM de la pantalla
static void ssd1306_send_buffer(uint8_t* buffer, uint16_t size) {
    start_i2c();
    address_i2c(SSD1306_I2C_ADDR, 0); // Modo escritura
    data_i2c(0x40); // 0x40 indica que lo que sigue son datos
    for (uint16_t i = 0; i < size; i++) {
        data_i2c(buffer[i]);
    }
    stop_i2c();
}

// --- Implementación de Funciones Públicas ---

void ssd1306_init(void) {
    // Secuencia de inicialización estándar para una pantalla de 128x64
    ssd1306_send_command(0xAE); // Apagar pantalla

    // Configurar el modo de direccionamiento a Horizontal
    ssd1306_send_command(0x20); // Comando para establecer modo de memoria
    ssd1306_send_command(0x00); // Modo Horizontal

    // --- CORRECCIONES DE ORIENTACIÓN ---
    ssd1306_send_command(0xA1); // Mapeo de segmentos normal (no espejado)
    ssd1306_send_command(0xC0); // Dirección de escaneo COM normal (no de cabeza)
    // ------------------------------------

    ssd1306_send_command(0xA8); // Set MUX Ratio
    ssd1306_send_command(0x3F); // Para 64 líneas

    ssd1306_send_command(0xD3); // Set Display Offset
    ssd1306_send_command(0x00); // Sin offset

    ssd1306_send_command(0x40); // Set Display Start Line (línea 0)
    
    ssd1306_send_command(0x8D); // Activar la bomba de carga (Charge Pump)
    ssd1306_send_command(0x14); // Habilitar

    ssd1306_send_command(0xDA); // Configuración de pines COM
    ssd1306_send_command(0x12);

    ssd1306_send_command(0x81); // Set Contrast
    ssd1306_send_command(0xCF);

    ssd1306_send_command(0xD9); // Set Pre-charge Period
    ssd1306_send_command(0xF1);

    ssd1306_send_command(0xDB); // Set VCOMH Deselect Level
    ssd1306_send_command(0x40);

    ssd1306_send_command(0xA4); // Mostrar contenido de la RAM
    ssd1306_send_command(0xA6); // Display normal (no invertido)

    ssd1306_send_command(0xAF); // Encender pantalla

    // Limpiar la pantalla al inicio
    ssd1306_clear();
    ssd1306_update_screen();
}

void ssd1306_clear(void) {
    // Rellena todo el buffer con ceros (píxeles apagados)
    memset(SSD1306_Buffer, 0, sizeof(SSD1306_Buffer));
}

void ssd1306_update_screen(void) {
    // Secuencia de actualización para el Modo Horizontal
    ssd1306_send_command(0x21); // Comando para establecer rango de columnas
    ssd1306_send_command(0);    // Columna inicial 0
    ssd1306_send_command(127);  // Columna final 127

    ssd1306_send_command(0x22); // Comando para establecer rango de páginas
    ssd1306_send_command(0);    // Página inicial 0
    ssd1306_send_command(7);    // Página final 7 (para 64 píxeles de alto)

    // Enviar todo el buffer a la pantalla
    ssd1306_send_buffer(SSD1306_Buffer, sizeof(SSD1306_Buffer));
}

void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        return;
    }
    
    if (color) { // Encender píxel
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    } else { // Apagar píxel
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

char ssd1306_draw_char(char ch, uint8_t x, uint8_t y, FontDef Font) {
    uint8_t i, j;
    
    if (SSD1306_WIDTH <= (x + Font.FontWidth) || SSD1306_HEIGHT <= (y + Font.FontHeight)) {
        return 0;
    }
    
    uint32_t char_offset = (ch - 32) * (Font.FontWidth * 2);
    const uint8_t* char_data = &Font.data[char_offset];

    for (i = 0; i < Font.FontWidth; i++) {
        uint8_t byte1 = char_data[i * 2];
        uint8_t byte2 = char_data[i * 2 + 1];

        for (j = 0; j < Font.FontHeight; j++) {
            if (j < 8) {
                if ((byte1 << j) & 0x80) {
                    ssd1306_draw_pixel(x + i, y + j, 1);
                }
            } else {
                if ((byte2 << (j - 8)) & 0x80) {
                    ssd1306_draw_pixel(x + i, y + j, 1);
                }
            }
        }
    }
    
    return ch;
}

void ssd1306_draw_string(const char* str, uint8_t x, uint8_t y, FontDef Font) {
    while (*str) {
        if (ssd1306_draw_char(*str, x, y, Font) != *str) {
            return;
        }
        x += Font.FontWidth;
        str++;
    }
}

// --- NUEVA FUNCIÓN PARA DIBUJAR IMÁGENES ---
void ssd1306_draw_bitmap(const uint8_t* bitmap) {
    // Copia los datos del bitmap (que deben ser 1024 bytes)
    // directamente al buffer de la pantalla, sobreescribiendo todo.
    memcpy(SSD1306_Buffer, bitmap, sizeof(SSD1306_Buffer));
	
	
}