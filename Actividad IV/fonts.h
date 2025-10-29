#ifndef __FONTS_H
#define __FONTS_H

#include <stdint.h>

// Estructura que define las propiedades de una fuente
typedef struct {
    const uint8_t FontWidth;    // Ancho del caracter en píxeles
    const uint8_t FontHeight;   // Alto del caracter en píxeles
    const uint8_t *data;        // Puntero al array de datos de la fuente
} FontDef;


// --- DECLARACIÓN DE FUENTES DISPONIBLES ---
extern FontDef Font_7x10; // Fuente de 7 píxeles de ancho por 10 de alto

#endif // __FONTS_H