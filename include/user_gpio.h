// user_gpio.h

#ifndef _USER_GPIO_H
#define _USER_GPIO_H

#include <user_interface.h>
#include <osapi.h>
#include <gpio.h>
#include "user_task.h"

// GPIO defines
#define HIGH_PIN	12
#define HIGH_BIT	BIT12
#define HIGH_MUX	PERIPHS_IO_MUX_MTDI_U
#define HIGH_FUNC	FUNC_GPIO12
#define BUTTON1_PIN	13
#define BUTTON1_BIT	BIT13
#define BUTTON1_MUX	PERIPHS_IO_MUX_MTCK_U
#define BUTTON1_FUNC	FUNC_GPIO13	
#define BUTTON2_PIN	14
#define BUTTON2_BIT	BIT14
#define BUTTON2_MUX	PERIPHS_IO_MUX_MTMS_U
#define BUTTON2_FUNC	FUNC_GPIO14

// Interrupt Service Routine: user_gpio_isr(uint32 intr_mask, void *arg)
// Desc: ISR called when a GPIO interrupt happens
// Args:
//	uint32 intr_mask: Interrupt flag mask
//	void *arg:	Arguments
// Returns:
//	Nothing 
void user_gpio_isr(uint32 intr_mask, void *arg);

#endif	 
