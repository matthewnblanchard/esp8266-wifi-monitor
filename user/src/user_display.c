#include "user_display.h"
#include <gpio.h>

uint8 display_matrix[1024];

void ICACHE_FLASH_ATTR user_lcd_init(os_event_t *e)
{
	os_memset(display_matrix, 0xFF, 1024);	// Initialize all pixels to 'on'

	// Disable interrupts, this section is timing critical
	ETS_GPIO_INTR_DISABLE();

	// A long string of commands for setup. These seem to be agreed upon
	// by the community, but some of them seem nonsensical. Will need review
	user_lcd_cmd(0xAE);	// Turn the display off
	user_lcd_cmd(0xA8);	// Set the MUX ratio to max (0x3F)
	user_lcd_cmd(0x3f);
	user_lcd_cmd(0xd3);	// Set display line offset to 0
	user_lcd_cmd(0x00);
	user_lcd_cmd(0x40);	// Set display RAM start line to 0
	user_lcd_cmd(0xA0);	// Set Segment mapping to normal
	user_lcd_cmd(0xA1);	// Set segment mapping to reverse
	user_lcd_cmd(0xC0);	// Set COM output scan direction to forward
	user_lcd_cmd(0xC8);	// '' ''' but reverse
	user_lcd_cmd(0xDA);	// Set COM pin configuration to alternative
	user_lcd_cmd(0x12);
	user_lcd_cmd(0xFF);	// No idea
	user_lcd_cmd(0xA4);	// Display RAM content
	user_lcd_cmd(0xA6);	// Normal display ('1' = pixel on)
	user_lcd_cmd(0xD5);
	user_lcd_cmd(0x80);
	user_lcd_cmd(0x8D);
	user_lcd_cmd(0x14);
	user_lcd_cmd(0xAF);
	user_lcd_cmd(0x20);
	user_lcd_cmd(0x00);

	// Start by displaying all pixels	
	user_lcd_display();

	// Re-enable interrupts
	ETS_GPIO_INTR_ENABLE();
};

void ICACHE_FLASH_ATTR user_lcd_cmd(uint8 cmd)
{
	// Start condition
	user_i2c_start_bit();

	// Indicate we are writing data by writing the slave's address followed by a '0'
	user_i2c_write_byte((SLAVE_ADDRESS << 1) | 0x00);	

	// Sending byte 0x00 indicates the following byte will be a command
	if (user_i2c_write_byte(0x00) != 0) {			// Check for ACK
		os_printf("ERROR: slave did not ack\r\n");
		return;	
	};

	// Write the command
	if (user_i2c_write_byte(cmd) != 0) {			// Check for ACK
		os_printf("ERROR: slave did not ack\r\n");
		return;	
	};

	// Stop condition
	user_i2c_stop_bit();

	return;	
};

void ICACHE_FLASH_ATTR user_lcd_display(void)
{
	uint16 i = 0;	// Loop index 
	

	// Indicate we are writing data by writing the slave's address followed by a '0'
	user_i2c_write_byte((SLAVE_ADDRESS << 1) | 0x00);	

	// Setup commands, needs review
	user_lcd_cmd(0x21);
	user_lcd_cmd(0x00);
	user_lcd_cmd(0xFF);
	user_lcd_cmd(0x22);
	user_lcd_cmd(0x00);
	user_lcd_cmd(0x07);
	
	// Start condition
	user_i2c_start_bit();

	// Indicate we are writing data by writing the slave's address followed by a '0'
	user_i2c_write_byte((SLAVE_ADDRESS << 1) | 0x00);
	
	// Sending byte 0x40 indicates the following byte will be data
	if (user_i2c_write_byte(0x40) != 0) {			// Check for ACK
		os_printf("ERROR: slave did not ack\r\n");
		return;	
	};

	// Write the display matrix
	for (i = 0; i < 1024; i++) {
		if (user_i2c_write_byte(display_matrix[i]) != 0) {			// Check for ACK
			os_printf("ERROR: slave did not ack\r\n");
			return;	
		};
	};

	// Stop condition
	user_i2c_stop_bit();

	return;	
};
