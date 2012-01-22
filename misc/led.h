#ifndef LED_H
#define LED_H

/*
LED
*/

#define F_CPU 12000000L

#include <inttypes.h>
#include <util/delay.h>
#include <avr/io.h>
 
// led defines
#define OFF 0
#define ON 1
#define TOGGLE 3

extern unsigned char get_led(unsigned char);
extern void init_leds(void);
extern unsigned char set_led(unsigned char, unsigned char);

#endif
