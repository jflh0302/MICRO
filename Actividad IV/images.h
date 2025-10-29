#ifndef __IMAGES_H__
#define __IMAGES_H__

#include <stdint.h>

#include "ssd1306.h" // Incluye tu cabecera para acceder a ssd1306_draw_pixel y tipos de datos.

/**
 * @brief Estructura para definir un mapa de bits (bitmap) con sus dimensiones.
 */
typedef struct {
    const uint8_t width;   // Ancho de la imagen en p�xeles.
    const uint8_t height;  // Alto de la imagen en p�xeles.
    const uint8_t *data;   // Puntero al array de datos binarios del bitmap.
} Bitmap_t;

// -------------------------------------------------------------------
// --- Declaraci�n Externa de las Im�genes ---
// -------------------------------------------------------------------

/**
 * @brief Declaraci�n del Logo de Pumas UNAM (64x64 p�xeles).
 * Es 'extern' porque la definici�n real (el array de datos) est� en images.c.
 */
extern const Bitmap_t PUMAS_LOGO_64x64;

// -------------------------------------------------------------------
// --- Prototipo de la Funci�n de Dibujo ---
// -------------------------------------------------------------------

/**
 * @brief Dibuja un bitmap en el buffer de la pantalla en una posici�n (x, y) espec�fica.
 * @param bitmap: Puntero a la estructura Bitmap_t con los datos de la imagen.
 * @param x: Coordenada X (columna) inicial.
 * @param y: Coordenada Y (fila) inicial.
 */
void images_draw_bitmap(const Bitmap_t *bitmap, uint8_t x, uint8_t y);

#endif // IMAGES_H