#include "user_display.h"
#include <gpio.h>

uint8 display_matrix[1024];

// 6 column text encoding. 
char text_encoding[26][6] = {
{0xF8, 0x14, 0x12, 0x14, 0xF8, 0x00},	// A
{0xFE, 0x92, 0x92, 0x92, 0x6C, 0x00},	// B
{0x38, 0x44, 0x82, 0x82, 0x44, 0x00},	// C
{0xFE, 0x82, 0x82, 0x44, 0x38, 0x00}, 	// D
{0xFE, 0x92, 0x92, 0x92, 0x92, 0x00},	// E
{0xFE, 0x12, 0x12, 0x02, 0x02, 0x00},	// F
{0x38, 0x44, 0x92, 0x92, 0x72, 0x00}, 	// G
{0xFE, 0x10, 0x10, 0x10, 0xFE, 0x00},	// H
{0x82, 0x82, 0xFE, 0x82, 0x82, 0x00},	// I
{0x42, 0x82, 0x7E, 0x02, 0x02, 0x00},	// J
{0xFE, 0x10, 0x28, 0x44, 0x82, 0x00},	// K
{0xFE, 0x80, 0x80, 0x80, 0x80, 0x00},	// L
{0xFC, 0x02, 0xFC, 0x02, 0xFC, 0x00},	// M
{0xFE, 0x0C, 0x10, 0x60, 0xFE, 0x00},	// N
{0x38, 0x44, 0x82, 0x44, 0x38, 0x00},	// O
{0xFE, 0x12, 0x12, 0x12, 0x0C, 0x00},	// P
{0x38, 0x44, 0xA2, 0x44, 0xB8, 0x00}, 	// Q
{0xFE, 0x12, 0x32, 0x52, 0x8C, 0x00},	// R
{0x4C, 0x92, 0x92, 0x92, 0x64, 0x00},	// S
{0x02, 0x02, 0xFE, 0x02, 0x02, 0x00},	// T
{0x7E, 0x80, 0x80, 0x80, 0x7E, 0x00}, 	// U
{0x3E, 0x40, 0x80, 0x40, 0x3E, 0x00},	// V
{0x7E, 0x80, 0x78, 0x80, 0x7E, 0x00},	// W
{0xC6, 0x28, 0x10, 0x28, 0xC6, 0x00},	// X
{0x06, 0x08, 0xF0, 0x08, 0x06, 0x00},	// Y
{0x86, 0x8A, 0x92, 0xA2, 0xC2, 0x00}	// Z
};

// 4 column digit encoding
char digit_encoding[10][4] = {
{0xFE, 0x82, 0xFE, 0x00},	// 0
{0x84, 0xFE, 0x80, 0x00},	// 1
{0xF2, 0x92, 0x9E, 0x00},	// 2
{0x92, 0x92, 0xFE, 0x00},	// 3
{0x1E, 0x10, 0xFE, 0x00},	// 4
{0x9E, 0x92, 0xF2, 0x00},	// 5
{0xFE, 0x92, 0xF2, 0x00},	// 6
{0x02, 0x02, 0xFE, 0x00},	// 7
{0xFE, 0x92, 0xFE, 0x00},	// 8
{0x1E, 0x12, 0xFE, 0x00}	// 9
};

void ICACHE_FLASH_ATTR user_lcd_init(os_event_t *e)
{

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

	// Clear the LCD
	user_lcd_clear();

	user_lcd_sniffer_xaxis();
	user_lcd_sniffer_yaxis();	
	user_lcd_sniffer_title();

	user_lcd_sniffer_channel_bar(1, 0.1, 5);
	user_lcd_sniffer_channel_bar(2, 0.2, 20);
	user_lcd_sniffer_channel_bar(3, 0.3, 30);
	user_lcd_sniffer_channel_bar(4, 0.4, 40);
	user_lcd_sniffer_channel_bar(5, 0.5, 50);
	user_lcd_sniffer_channel_bar(6, 0.6, 60);
	user_lcd_sniffer_channel_bar(7, 0.7, 70);
	user_lcd_sniffer_channel_bar(8, 0.8, 80);
	user_lcd_sniffer_channel_bar(9, 0.9, 90);
	user_lcd_sniffer_channel_bar(10, 1.0, 100);
	user_lcd_sniffer_channel_bar(11, 0.0, 110);

	// Update the LCD
	user_lcd_update();

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

void ICACHE_FLASH_ATTR user_lcd_update(void)
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

void ICACHE_FLASH_ATTR user_lcd_clear(void)
{
	os_memset(display_matrix, 0x00, 1024);
	return;
};

void ICACHE_FLASH_ATTR user_lcd_text(uint8 *text, uint16 len, uint16 line, uint16 offset)
{	
	uint16 pos = (128 * line) + offset;	// Starting column for printing
	uint16 i = 0;				// Loop index
	uint16 j = 0;				// Loop index 2
	uint16 encode_index = 0;		// Index of a letter or digit in the encoding
	bool is_digit = false;			// Indicates if a character is a digit (use digit encoding)

	for (i = 0; i < len; i++) {

		// Captical ASCII letter
		if ((text[i] > 0x40) && (text[i] < 0x5B)) {
			encode_index = text[i] - 0x41;
			is_digit = false;
		
		// Lower case ASCII letter
		} else if ((text[i] > 0x60) && (text[i] < 0x7B)) {
			encode_index = text[i] - 0x61;
			is_digit = false;

		// ASCII digits
		} else if ((text[i] > 0x2F) && (text[i] < 0x3A)) {
			encode_index = text[i] - 0x30;
			is_digit = true;

		// Space - skip 4 columns, or to the next line if 4 columns exceeds the current line 
		} else if (text[i] == 0x20) {
			if (((pos % 128) + 6) > 127) {
				 (pos + (128 - (pos % 128)));
			} else {
				pos += 4;
			}
			continue;	
	
		// Discard other characters
		} else {
			continue;
		}
	
		// Letter printing	
		if (!is_digit) {

			// Skip to new line if there is not enough space
			pos = (((pos % 128) + 6) > 127) ? (pos + (128 - (pos % 128))) : (pos);
			
			// Terminate if the next letter will run off the matrix
			if ((pos + 6) >= (128 * 8)) break;

			for (j = 0; j < 6; j++) {
				display_matrix[pos] = text_encoding[encode_index][j];
				pos++;
			};
		}

		// Digit printing
		if (is_digit) {

			// Skip to new line if there is not enough space
			pos = (((pos % 128) + 4) > 127) ? (pos + (128 - (pos % 128))) : (pos);

			// Terminate if the next letter will run off the matrix
			if ((pos + 4) >= (128 * 8)) break;

			for (j = 0; j < 4; j++) {
				display_matrix[pos] = digit_encoding[encode_index][j];
				pos++;
			};
		}
	}

	return;
};

void ICACHE_FLASH_ATTR user_lcd_sniffer_xaxis(void)
{
	uint16 pos = 0;		// Display matrix position
	uint8 channel = 1;	// Wifi channel #
	uint8 digit = 0;	// Digit is display
	
	// Print x-axis label
	user_lcd_text("wifi channel", 12, 7, 40);

	// Print channel numbers at at spaced intervals
	pos = 17;	// Starting position

	// Single digit channels
	for (channel = 1; channel <= 9; channel++) {
		pos++;			// Space of padding
		pos += 4;		// Skip a space
		digit = channel + 0x30;	// Convert digit to ASCII
		user_lcd_text(&digit, 1, 6, pos);	// Display digit
		pos += 4;
		pos++;			// Space of padding
	};

	// Double digit channels
	for (channel = 10; channel <= 11; channel++) {
		pos++;					// Space of padding
		digit = 0x31;				// Display leading '1'
		user_lcd_text(&digit, 1, 6, pos);	
		pos += 4;				
		digit = (channel % 10) + 0x30;		// Convert 1's place to ASCII
		user_lcd_text(&digit, 1, 6, pos);
		pos += 4;
		pos++;					// Space of padding
	};

	// Draw hline at the bottom of the 6th row
	user_lcd_hline(47, 17, 127);

	return;
};

void ICACHE_FLASH_ATTR user_lcd_hline(uint16 height, uint16 start_pos, uint16 end_pos)
{
	uint16 i = 0;	// Loop index

	// If the height is greater than the lowest row, discard
	if (height > 63) {
		return;
	};

	// Check that starting and ending positions are valid
	if ((start_pos > 127) || (end_pos > 127) || (end_pos <= start_pos)) {
		return;
	};  		

	// Start at the beginning of a row
	for (i = ((128 * (height / 8)) + start_pos); i < (((128 * (height / 8)) + 128) - (127 - end_pos)); i++) {
		display_matrix[i] |= (0x01 << (height % 8));
	};
	
	return;
};

void ICACHE_FLASH_ATTR user_lcd_vline(uint16 col, uint16 start_h, uint16 end_h)
{
	uint16 i = 0;	// Loop index

	// If the column is off the display matrix, discard
	if (col > 127) {
		return;
	};

	// Check that starting and ending heights are valid
	if ((start_h > 63) || (end_h > 63) || (end_h <= start_h)) {
		return;
	};  	

	// Fill in each pixel vertically in sequence
	for (i = start_h; i <= end_h; i++) {
		display_matrix[((i / 8) * 128) + col] |= (0x01) << (i % 8);
	};	

	return;	
};

void ICACHE_FLASH_ATTR user_lcd_sniffer_yaxis(void)
{

	// Print % symbol for label
	display_matrix[(2 * 128) + 0] = 0x11;
	display_matrix[(2 * 128) + 1] = 0x08;
	display_matrix[(2 * 128) + 2] = 0x04;
	display_matrix[(2 * 128) + 3] = 0x02;
	display_matrix[(2 * 128) + 4] = 0x11;
	display_matrix[(2 * 128) + 5] = 0x00;

	// Print "TFC" for "traffic", vertically
	user_lcd_text("T", 1, 3, 0);
	user_lcd_text("F", 1, 4, 0);
	user_lcd_text("C", 1, 5, 0);

	// Print percentage marks
	user_lcd_text("100", 3, 2, 6);
	user_lcd_text("75", 2, 3, 10);
	user_lcd_text("50", 2, 4, 10);
	user_lcd_text("25", 2, 5, 10);
	
	
	// Draw vline at 17th column
	user_lcd_vline(17, 16, 47);

	// Draw vline at the 127th column
	user_lcd_vline(127, 16, 47);

	return;
};

void ICACHE_FLASH_ATTR user_lcd_sniffer_title(void)
{
	// Draw a horizontal line at the top of the plotting area
	user_lcd_hline(16, 17, 127);

	// Draw the title of the plot
	user_lcd_text("Packets per Second", 18, 0, 10);

	return;
};

void ICACHE_FLASH_ATTR user_lcd_sniffer_channel_bar(uint16 channel, float percent, uint16 pps) {
	
	uint16 start_col = 9 + (channel * 10);	// Starting column
	uint16 end_col = start_col + 7;		// Ending column
	uint16 end_h = 47;			// Ending height
	uint16 start_h = 47 - (uint16)(percent * (float)(47 - 16)); // Start height is a percent portion of the whole plot height
	uint16 i = 0;				// Loop index
	uint8 digit = 0;			// Digit for printing pps	

	for (i = start_col; i <= end_col; i++) {
		user_lcd_vline(i, start_h, end_h);
	};
	
	// The max displayable pps is 99
	uint16 num = (pps > 99) ? 99 : pps;

	// Tens place
	if ((num / 10) != 0) {
		digit = (num / 10) + 0x30;
		user_lcd_text(&digit, 1, 1, start_col);
	}

	// Ones place
	digit = (num % 10) + 0x30;
	user_lcd_text(&digit, 1, 1, start_col + 4);

	return;
	
};
