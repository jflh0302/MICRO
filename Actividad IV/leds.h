#ifndef __LEDS_H__
#define __LEDS_H__
#include <stdint.h> 

void led_on_off(void);
void delay_ms(volatile uint32_t ms);

#endif /* __LEDS_H__ */