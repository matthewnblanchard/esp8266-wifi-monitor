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

// Application Function: user_lcd_update(void)
// Desc: Updates the LCD with the display matrix
// Args:
//	Nothing
// Returns:
//	Nothing 
void ICACHE_FLASH_ATTR user_lcd_update(void);

// Application Function: user_lcd_clear(void)
// Desc: Clears the display matrix
// Args:
//	Nothing
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR user_lcd_clear(void);

// Application Function: user_lcd_text(uint8 *text, uint16 len, uint16 line, uint16 offset);
// Desc: Writes the given text to the input line at the input offset
// Args:
//	uint8 *text: Text string to be printed
//	uint16 len: Length of text string to print
//	uint16 line: Line on which to print
//	uint16 offset: Pixel offset to print the text
void ICACHE_FLASH_ATTR user_lcd_text(uint8 *text, uint16 len, uint16 line, uint16 offset);

// Application Function: user_lcd_sniffer_xaxis(void);
// Desc: Prints the x axis of the sniffer graph to the LCD
// Args:
//	None
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR user_lcd_sniffer_xaxis(void);

// Application Function: user_lcd_sniffer_yaxis(void);
// Desc: Prints the y axis of the sniffer graph to the LCD
// Args:
//	None
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR user_lcd_sniffer_yaxis(void);

// Application Function: user_lcd_hline(uint16 height, uint16 start_pos, uint16 end_pos);
// Desc: Prints a horizontal line at the input height
// Args:
//	uint16 height: The desired height, starting from the TOP
//		(e.g. height 0 is the top row of pixels)
//	uint16 start_pos: Starting horizontal position
//	uint16 end_pos: Ending horizontal position 
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR user_lcd_hline(uint16 height, uint16 start_pos, uint16 end_pos);

// Application Function: user_lcd_vline(uint16 col, uint16 start_h, uint16 end_h);
// Desc: Prints a vertial line at the input column
// Args:
//	uint16 col: The desired column, stsrting from the left
//	uint16 start_h: Starting height
//	uint16 end_h:	Ending height
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR user_lcd_vline(uint16 col, uint16 start_h, uint16 end_h);

// Application Function: user_lcd_sniffer_title(void);
// Desc: Prints the title of the sniffer grapj
// Args:
//	None
// Returns:
//	Nothing
void ICACHE_FLASH_ATTR user_lcd_sniffer_title(void);

// Application Function: user_lcd_sniffer_channel_bar(uint16 channel, float percent);
// Desc: Prints a percent bar for the given channel, based on the input percent
// Args:
//	uint16 channel:	WiFi channel at which to place the bar
//	float percent:	Percent of bar to fill
//	uint16 pps: Total packets per second for the channel
void ICACHE_FLASH_ATTR user_lcd_sniffer_channel_bar(uint16 channel, float percent, uint16 pps);


#endif
