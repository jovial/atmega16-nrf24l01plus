#define F_CPU 12000000L

#include <inttypes.h>
#include "radio/packet.h"
#include "radio/radio.h"
#include <util/delay.h>
#include <avr/io.h>
#include "misc/led.h"

volatile uint8_t rxflag = 0;
uint8_t station_addr[5] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xE4 };
uint8_t my_addr[5] = { 0x98, 0x76, 0x54, 0x32, 0x10 };
 
radiopacket_t packet;
 


void setup()
{
	//pinMode(13, OUTPUT);
	
	//CE_HIGH();
	//_delay_ms(1000);
	//CE_LOW();
	//Radio_Init();
	init_leds();
	
	//set_led(3,TOGGLE);
	Radio_Init();
 	//set_led(3,TOGGLE);

	// configure the receive settings for radio pipe 0
	Radio_Configure_Rx(RADIO_PIPE_0, my_addr, ENABLE);
	// configure radio transciever settings.
	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);
	
	//transmit();
 

	
}

void transmit() {
	// load up the packet contents	
	packet.type = MESSAGE;
	memcpy(packet.payload.message.address, my_addr, RADIO_ADDRESS_LENGTH);
	packet.payload.message.messageid = 55;
	snprintf((char*)packet.payload.message.messagecontent, sizeof(packet.payload.message.messagecontent), 		"Test message.");
 
	Radio_Set_Tx_Addr(station_addr);
	Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);
	//Radio_Transmit(&packet, RADIO_RETURN_ON_TX);
}
 
void loop()
{
	transmit();
	
	if (rxflag)
	{
		if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS)
		{
			// if there are no more packets on the radio, clear the receive flag;
			// otherwise, we want to handle the next packet on the next loop iteration.
			rxflag = 0;
		}
		if (packet.type == ACK)
		{
			set_led(2,TOGGLE);
			_delay_ms(1000);
			set_led(2,TOGGLE);
		} else {
			set_led(1,TOGGLE);
			_delay_ms(1000);
			set_led(1,TOGGLE);
		}
	}

			set_led(0,TOGGLE);
		_delay_ms(5000);
}
 
void radio_rxhandler(uint8_t pipe_number)
{
	rxflag = 1;
}

// Arduino's default main function (included here for clarity)
int main()
{
	setup();
	for (;;) loop();
	//init_leds();
	//Radio_Init();
	/*while(1) {
		transmit();		
		set_led(0,TOGGLE);
		_delay_ms(5000);

	}*/
	//return 0;
}
