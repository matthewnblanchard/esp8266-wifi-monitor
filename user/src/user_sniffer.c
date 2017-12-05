#include "user_sniffer.h"

void ICACHE_FLASH_ATTR wifi_sniffer_init(os_event_t *e)
{				
	// Configure the ESP8266 to act as a station
	if (!wifi_set_opmode(STATION_MODE)) {
		os_printf("ERROR: Failed to enter station mode\r\n");
		return;
	}

	// Enable promiscuous mode. The ESP8266 will receive all packets
	wifi_promiscuous_enable(true);

	// Set the callback function for when a packet is received
	wifi_set_promiscuous_rx_cb(packet_recv_cb);	

	return;
};

void ICACHE_FLASH_ATTR packet_recv_cb(uint8 *buf, uint16 len)
{
	uint16 i = 0;
	os_printf("received packet: len=%d\r\n", len);	
	for (i = 0; i++; i < len) {
		os_printf("%d ", buf[i]);
		if ((i % 8) == 0) {
			os_printf("\r\n");
		}
	}
	os_printf("\r\n\r\n");	

	return;
};
