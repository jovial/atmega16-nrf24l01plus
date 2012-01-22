#define F_CPU 12000000L

#include <inttypes.h>
#include "radio/packet.h"
#include "radio/radio.h"
#include <util/delay.h>
#include <avr/io.h>
#include "usart/ad_usart.h"
#include "misc/led.h"

char output[128];
volatile uint8_t rxflag = 0;
radiopacket_t packet;


uint8_t station_addr[5] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xE4 };
 
// setup function is called once at the program's start
void setup()
{
	// start the serial output module at 100000 bps
	//Serial.begin(100000);	
	init_usart(BRATE_576);

	// initialize the radio, including the SPI module
	Radio_Init();
	init_leds();
	

	//snprintf(output, sizeof(output), "Status(setup): %x \n\r",status);
	//put_str(output,0);
 
	// configure the receive settings for radio pipe 0
	Radio_Configure_Rx(RADIO_PIPE_0, station_addr, ENABLE);
 
	// configure radio transciever settings.
	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);
 
	// print a message to UART to indicate that the program has started up
	snprintf(output, sizeof(output), "STATION START\n\r");
	put_str(output,0);
}
 
// loop function is called over and over while the system runs.
void loop()
{
	// if a packet hasn't been received, do nothing (see radio_rxhandler below).
	if (!rxflag) return;
 
	// if a packet has been received, get it.
	if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS)
	{
		// if there are no more packets on the radio, clear the receive flag;
		// otherwise, we want to handle the next packet on the next loop iteration.
		rxflag = 0;
		//set_led(1,TOGGLE);
		_delay_ms(175);
		//set_led(1,TOGGLE);
	}
 
	// This station is only expecting to receive MESSAGE packets (see packet.h).
	if (packet.type != MESSAGE)
	//if (0)
	{
		snprintf(output, sizeof(output), "Error: wrong packet type (type %d).\n\r", packet.type);
		put_str(output,1);
		return;
	}
 
	// Set the transmit address to the one specified in the received message packet.
	Radio_Set_Tx_Addr(packet.payload.message.address);
 
	// Print out the message, along with the message ID and sender address.
	snprintf(output, sizeof(output), "Message ID %d from 0x%.2X%.2X%.2X%.2X%.2X: '%s'\n\r",
			packet.payload.message.messageid,
			packet.payload.message.address[0],
			packet.payload.message.address[1],
			packet.payload.message.address[2],
			packet.payload.message.address[3],
			packet.payload.message.address[4],
			packet.payload.message.messagecontent);
	put_str(output,0);

	//snprintf(output, sizeof(output), "Packet size: %d.\n\r", sizeof(packet));
	//put_str(output,0);
 
	// Reply to the sender by sending an ACK packet, reusing the packet data structure.
	packet.type = ACK;
	//packet.type = 3;

	//snprintf(output, sizeof(output), "ACK: %d , MESSAGE: %d \n\r",ACK,MESSAGE);
	//put_str(output,0);
	
	//snprintf(output, sizeof(output), "timestamp: %x \n\r",packet.timestamp);
	//put_str(output,0);
 
	if (Radio_Transmit(&packet, RADIO_WAIT_FOR_TX) == RADIO_TX_MAX_RT)
	{
		// If the max retries was reached, the packet was not acknowledged.
		// This usually occurs if the receiver was not configured correctly or
		// if the sender didn't copy its address into the radio packet properly.
		snprintf(output, sizeof(output), "Could not reply to sender.\n\r");
		put_str(output,0);
	}
	else
	{
		// the transmission was completed successfully
		snprintf(output, sizeof(output), "Replied to sender.\n\r");
		put_str(output,0);
			//set_led(0,TOGGLE);
			//_delay_ms(100);
			//set_led(0,TOGGLE);
	}
 
	// the LED should flash for a little over 50 ms.
	

	//digitalWrite(13, LOW); turn off led
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

	return 0;
}
