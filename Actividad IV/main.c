#include "stm32f10x.h"
#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"
#include "fonts.h"
#include "ext_in.h"
#include "leds.h"
#include "images.h"
int main(void) {
    // 1. Inicialización de Periféricos 
    init_ports();
    init_i2c();
    init_ext_in();
    ssd1306_init();
	
	
    // --- 4. Actualizar/Mostrar en Pantalla ---
    ssd1306_update_screen();
    
    // 2. Bucle Infinito con la Secuencia 
    while(1) {
       
    
    // --- 4. Actualizar/Mostrar en Pantalla ---
    ssd1306_update_screen();
    
			
			ssd1306_set_display_mode(0); // <-- Modo Normal
        
        ssd1306_clear();
        ssd1306_draw_string("Actividad IV", 10, 10, Font_7x10); 
        ssd1306_draw_string("ITSE", 10, 30, Font_7x10); 

        ssd1306_update_screen(); 
        delay_ms(500);          

        
        // --- 2. MODO INVERTIDO (Letras Negras sobre Fondo Blanco) ---
        ssd1306_set_display_mode(1); // <-- Modo Invertido
        
        ssd1306_clear();
        ssd1306_draw_string("JESUS", 10, 10, Font_7x10);
        ssd1306_draw_string("HERNANDEZ", 10, 30, Font_7x10); 

        ssd1306_update_screen(); 
        delay_ms(500);    
				
				ssd1306_set_display_mode(1); // <-- Modo Invertido
        
        ssd1306_clear();
        ssd1306_draw_string("1709", 10, 10, Font_7x10);
        ssd1306_draw_string("MICRO", 10, 30, Font_7x10); 

        ssd1306_update_screen(); 
        delay_ms(1000);   

ssd1306_clear();
uint8_t start_x = 32;
    uint8_t start_y = 0;
		images_draw_bitmap(&PUMAS_LOGO_64x64, start_x, start_y);
		ssd1306_update_screen(); 
        delay_ms(500);  
    }
}

       

