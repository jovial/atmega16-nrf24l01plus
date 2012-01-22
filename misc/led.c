#include "led.h"

unsigned char led_mask = 0x0f;

unsigned char set_led(unsigned char who, unsigned char what) {
	if (who >3 ) {
		return -1;
	}	
	unsigned char led = 0X00 | (0x01 << who);
	switch(what){
		case TOGGLE:
			PORTA ^= led;						
			break;
		case OFF:
			PORTA |= led;
			break;
		case ON:
			PORTA |= led;
			break;
	}

	return 0;
}



unsigned char get_led(unsigned char who) {	
	if (who > 3) {
		return -1;
	}
	unsigned char rtn;
	unsigned char porta_leds = PORTA & led_mask;
	unsigned char test_led = (porta_leds & (0x01 << who));
	rtn = test_led ? 1 : 0;
	return rtn;
}


void init_leds(void) {
	// all outputs	
	DDRA |= 0x0f;
	// all off
	PORTA |= 0x0f;

}


/*END LED */

