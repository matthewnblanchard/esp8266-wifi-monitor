// user_sniffer.h
// Desc: Contains routines/structures pertinent to Wifi
//	packet sniffing

#ifndef _USER_SNIFFER_H
#define _USER_SNIFFER_H

#include <user_interface.h>
#include <osapi.h>

#define CHANNEL_SWEEP_TIME	500	// Time in ms to spend on each channel

extern uint16 pchannel[11];	// Packets counted at each channel
extern uint16 ptotal;		// Total packets counted
extern uint16 cur_channel;	// Current channel

// User Task: wifi_sniffer_init(os_event_t *e)
// Desc: Configures the ESP8266 to act as a Wifi Sniffer.
// Args:
//	os_event_t *e: Pointer to OS event data
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR wifi_sniffer_init(os_event_t *e);

// Callback Function: packet_recv_cb(uint8 *buf, uint16 len)
// Desc: Called when the ESP8266 receives a packet
// Args:
//	uint8 *buf: Buffer containing packet data
//	uint16 len: Length of packet data
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR packet_recv_cb(uint8 *buf, uint16 len);

// Callback Function: user_channel_sweep(void)
// Desc: Timer callback. Sweeps through each channel in timed intervals,
//	Allowing the packet_recv callback to count packets. After each channel is
//	hit, updates the LCD and resets
// Args:
//	Nothing
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR user_channel_sweep(void);

#endif
