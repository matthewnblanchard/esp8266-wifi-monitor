#include <user_interface.h>
#include <osapi.h>
#include <mem.h>
#include "user_sniffer.h"

struct wlan_header {
	uint16 frame_ctrl;
	uint16 dur;
	uint8 addr_1[6];
	uint8 addr_2[6];
	uint8 addr_3[6];
	uint16 seq_ctrl;
	uint8 addr_4[6];
};

#define FRAME_CTRL_PROTO_VER(X) (((X) & 0xC000) >> 14)
#define FRAME_CTRL_TYPE(X) (((X) & 0x3000) >> 12)
#define FRAME_CTRL_SUBTYPE(X) (((X) & 0x0F00) >> 8)
#define FRAME_CTRL_TO_DS(X) (((X) & 0x0080) >> 7)
#define FRAME_CTRL_FROM_DS(X) (((X) & 0x0040) >> 6)
#define FRAME_CTRL_MORE(X) (((X) & 0x0020) >> 5)
#define FRAME_CTRL_RETRY(X) (((X) & 0x0010) >> 4)
#define FRAME_CTRL_PWR(X) (((X) & 0x0008) >> 3)
#define FRAME_CTRL_MORE_DATA(X) (((X) & 0x0004) >> 2)
#define FRAME_CTRL_WEP(X) (((X) & 0x0002) >> 1)
#define FRAME_CTRL_ORDER(X) (((X) & 0x0001) >> 0)

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
	struct wlan_header *header = (struct wlan_header *)os_zalloc(sizeof(struct wlan_header));

	// The packet header is 30 bytes. If the length is less than this, there must have been an error
	if (len < 30) {
		os_printf("Malformed packet\r\n");
		return;
	}

	// Transfer first 30 bytes to the header structure
	os_memcpy(header, buf, 30);

	
};
