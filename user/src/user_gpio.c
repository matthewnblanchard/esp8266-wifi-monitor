#include "user_gpio.h"

void user_gpio_isr(uint32 intr_mask, void *arg)
{
	static uint32 last_time = 0;
	gpio_intr_ack(intr_mask);

	uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);	// Check which GPIO(s) have an interrupt queued

	// Check if a ZCD interrupt occured
	if (intr_mask & (BUTTON1_BIT)) {	
		uint32 cur_time = system_get_time();	// Use the system time register for software debouncing
		if ((cur_time - last_time) > 750) {
			last_time = cur_time;
			TASK_RETURN(SIG_RUN, PAR_RUN_BUTTON1);
		};
	};

//	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
        return;
};
