#include "user_gpio.h"

void user_gpio_isr(uint32 intr_mask, void *arg)
{
	static uint32 last_time1 = 0;
	static uint32 last_time2 = 0;
	gpio_intr_ack(intr_mask);

	uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);	// Check which GPIO(s) have an interrupt queued
	
	if (intr_mask & (BUTTON2_BIT)) {	
		uint32 cur_time2 = system_get_time();		// Use the system time register for software debouncing
		if ((cur_time2 - last_time2) > 100000) {	// 100ms debouncing
			last_time2 = cur_time2;
			TASK_RETURN(SIG_RUN, PAR_RUN_BUTTON2);
		};
	};
	
        return;
};
