#include "radio/packet.h"
#include "radio/radio.h"
 
char output[128]
uint8_t station_addr[5] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xE4 };
 
// setup function is called once at the program's start
void setup()
{
	// start the serial output module at 100000 bps
	//Serial.begin(100000);
 
	// initialize the radio, including the SPI module
	Radio_Init();
 
	// configure the receive settings for radio pipe 0
	Radio_Configure_Rx(RADIO_PIPE_0, station_addr, ENABLE);
 
	// configure radio transciever settings.
	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);
 
	// print a message to UART to indicate that the program has started up
	//snprintf(output, sizeof(output), "STATION START\n\r");
	//Serial.print(output);
}
 
// loop function is called over and over while the system runs.
void loop()
{
	// if a packet hasn't been received, do nothing (see radio_rxhandler below).
	if (!rxflag) continue;
 
	// if a packet has been received, get it.
	if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS)
	{
		// if there are no more packets on the radio, clear the receive flag;
		// otherwise, we want to handle the next packet on the next loop iteration.
		rxflag = 0;
	}
 
	// This station is only expecting to receive MESSAGE packets (see packet.h).
	if (packet.type != MESSAGE)
	{
		//snprintf(output, sizeof(output), "Error: wrong packet type (type %d).\n\r", packet.type);
		//Serial.print(output);
		continue;
	}
 
	// Set the transmit address to the one specified in the received message packet.
	Radio_Set_Tx_Addr(packet.payload.message.address);
 
	// Print out the message, along with the message ID and sender address.
	//snprintf(output, sizeof(output), "Message ID %d from 0x%.2X%.2X%.2X%.2X%.2X: '%s'\n\r",
	//		packet.payload.message.messageid,
	//		packet.payload.message.address[0],
	//		packet.payload.message.address[1],
	//		packet.payload.message.address[2],
	//		packet.payload.message.address[3],
	//		packet.payload.message.address[4],
	//		packet.payload.message.messagecontent);
	//Serial.print(output);
 
	// Reply to the sender by sending an ACK packet, reusing the packet data structure.
	packet.type = ACK;
 
	if (Radio_Transmit(&packet, RADIO_WAIT_FOR_TX) == RADIO_TX_MAX_RT)
	{
		// If the max retries was reached, the packet was not acknowledged.
		// This usually occurs if the receiver was not configured correctly or
		// if the sender didn't copy its address into the radio packet properly.
		//snprintf(output, sizeof(output), "Could not reply to sender.\n\r");
		//Serial.print(output);
	}
	else
	{
		// the transmission was completed successfully
		//snprintf(output, sizeof(output), "Replied to sender.\n\r");
		//Serial.print(output);
	}
 
	// the LED should flash for a little over 50 ms.
	_delay(50);
	//digitalWrite(13, LOW); turn off led
}
 
// Arduino's default main function (included here for clarity)
int main()
{
	init();
	setup();
	for (;;) loop();
	return 0;
}
