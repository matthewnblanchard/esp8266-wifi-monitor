// user_pwm.h
// Author: Matthew Blanchard
// Description: Contains functions/definitions for driving a the pwm

#include <user_interface.h>
#include <eagle_soc.h>

// Pin definitions
#define PWM_PIN 0
#define PWM_BIT BIT0
#define PWM_MUX PERIPHS_IO_MUX_GPIO0_U
#define PWM_FUNC FUNC_GPIO0

// void user_pwm_drive(uint8 *data, uint16 len)
// Description: Drives the PWM 
// Args:
//	uint8 *data: Strings of RGB data
//	uint16 len: Data length
void user_pwm_drive(uint8 *data, uint16 len);

// void user_ws2812b_send_0()
// Description: Sends a "0" to the WS2812B LED strip
//	(400ns high, 850ns low +/- 150ns)
