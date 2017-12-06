#ifndef _USER_DISPLAY_H
#define _USER_DISPLAY_H

#include "user_i2c.h"

#define SLAVE_ADDRESS 0x3C

// User Task: user_lcd_init(os_event_t *e)
// Desc: Initializes the LCD display for use
// Args:
//	os_event_t *e: Pointer to OS event data
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR user_lcd_init(os_event_t *e);

// Application Function: user_lcd_cmd(uint8 cmd)
// Desc: Sends a command to the LCD
// Args:
//	uint8 cmd: Desired 8-bit command
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR user_lcd_cmd(uint8 cmd);

// Application Function: user_lcd_data(uint8 data)
// Desc: Sends one byte of data to the lcd
// Args:
//	uint8 data: Desired 8-bit data
// Returns:
//	Nothing 
void ICACHE_FLASH_ATTR user_lcd_display(void);

#endif
