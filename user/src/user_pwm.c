#include <eagle_soc.h>
#include "user_pwm.h"

static inline void user_ws2812b_send_0()
{
/*	__asm__ volatile (
		"movi 	a2,	0x60000304 \n"	// Address for GPIO high writes
		"movi	a3,	0x60000308 \n"	// Address for GPIO low writes
		"movi	a4,	%0	   \n"  // Save PWM bit
		"s32i	a4, a2, 0	   \n"	// Drive PWM pin high
		"memw			   \n"	// Wait for memory operations to complete
		"movi	a5,	0x7	   \n"	// Wait 400ns via dead loop
		"high:			   \n"
		"addi   a5, a5, -1	   \n"
		"bnez   a5, 	high	   \n"
		"s32i   a4, a3, 0	   \n"  // Drive PWM pin low
		"memw			   \n"	// Wait for memory operations to complete
		"movi	a5,	0xF	   \n"	// Wait 850ns via dead loop
		"low:			   \n"
		"addi   a5, a5, -1	   \n"
		"bnez   a5, 	low	   \n"
	:: "g" (PWM_BIT)			// Write the PWM GPIO pin bit to the read/write registers
	:  "a2", "a3", "a4", "a5"		// Clobber registers 
	);
*/

	uint8 delay;

	delay = 3;
	while (delay--) {
		GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, PWM_BIT);
	};
	delay = 8;
	while (delay--) {
		GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, PWM_BIT);
	};

	return;
};

void user_pwm_drive(uint8 *data, uint16 len)
{
	
	uint16 i = 0;
	for (i = 0; i < 1000; i++){
		user_ws2812b_send_0();
	};

	return;
};
