// user_main.c

#include <user_interface.h>
#include <osapi.h>
#include <gpio.h>
#include <mem.h>
#include "user_task.h"
#include "user_sniffer.h"
#include "user_i2c.h"
#include "user_display.h"
#include "user_gpio.h"

// Function prototypes
void ICACHE_FLASH_ATTR user_init(void);				// First step initialization function. Handoff from bootloader.
void ICACHE_FLASH_ATTR user_task_init(void);			// Task initialization. Hands off to the control task.
void ICACHE_FLASH_ATTR user_control_task(os_event_t *e);	// Main control task. Schedules all other user tasks.
void ICACHE_FLASH_ATTR user_gpio_init(void);			// Performs GPIO initialization

// User Task: user_init()
// Desc: Initialization. The ESP8266 hooks into this function after boot.
//      ICACHE_FLASH_ATTR flag causes the function to be saved to flash
//      rather than IRAM, in which there is limited space.
void ICACHE_FLASH_ATTR user_init(void)
{
        PRINT_DEBUG(DEBUG_LOW, "user code entered\r\n");

        // Set baud rate of UART0 to 115200 for serial debugging 
        // (default is nonstandard 74880) 
        uart_div_modify(0, UART_CLK_FREQ / 115200);

        PRINT_DEBUG(DEBUG_LOW, "UART speed set to 115200\r\n");

        // Initialize GPIO interfaces
        user_gpio_init();

        // Once the system has finished initializing,
        //      continue initializing user tasks
        system_init_done_cb(user_task_init);      
};

// Callback Function: user_task_init()
// Desc: Initializes data structures for posting tasks to the system
void ICACHE_FLASH_ATTR user_task_init(void)
{
        // Allocate space for message queues
        user_msg_queue_0 = (os_event_t *)os_malloc(sizeof(os_event_t) * MSG_QUEUE_LENGTH);
        user_msg_queue_1 = (os_event_t *)os_malloc(sizeof(os_event_t) * MSG_QUEUE_LENGTH);
        user_msg_queue_2 = (os_event_t *)os_malloc(sizeof(os_event_t) * MSG_QUEUE_LENGTH);

        // Register control task and begin control
        system_os_task(user_control_task, USER_TASK_PRIO_2, user_msg_queue_2, MSG_QUEUE_LENGTH);
	TASK_RETURN(SIG_CONTROL, PAR_CONTROL_START);

	return;
};

// User Task: user_control_task(os_event_t *e)
// Desc: Maximum priority control task which calls other tasks
//	based on the results of previous tasks
void ICACHE_FLASH_ATTR user_control_task(os_event_t *e)
{
	/* ==================== */
	/* Master Control Block */
	/* ==================== */
	switch(e->sig | e->par) {

		/* ----------------------- */
		/* General Control Signals */
		/* ----------------------- */

		// Control entry point. The interior begins by scanning for AP's using the
		// SSID/password it has saved in memory.
		case SIG_CONTROL | PAR_CONTROL_START:
			TASK_START(user_lcd_init, 0, 0);
			break;

		// Deadloop of task calls while waiting for the system to restart
		case SIG_CONTROL | PAR_CONTROL_ERR_DEADLOOP:
			TASK_RETURN(SIG_CONTROL, PAR_CONTROL_ERR_DEADLOOP);
			break;

		// Fatal error case. The control function calls itself with this sig/par
		// if it is passed an error it cannot recover from. The system is restarted
		// after a 5 second delay
		case SIG_CONTROL | PAR_CONTROL_ERR_FATAL:
			PRINT_DEBUG(DEBUG_LOW, os_printf("rebooting system in 5 seconds\r\n"));
			os_timer_setfn(&timer_reboot, system_restart, NULL);
			os_timer_arm(&timer_reboot, 5000, false);
			TASK_RETURN(SIG_CONTROL, PAR_CONTROL_ERR_DEADLOOP);
			break;

		/* ---------------------- */
		/* Initialization Signals */
		/* ---------------------- */

		// After the display is initialized, begin sweeping WiFi channels
		case SIG_INIT | PAR_INIT_DISPLAY_DONE:
			TASK_START(wifi_sniffer_init, 0, 0);
			break;

		case SIG_INIT | PAR_INIT_SNIFF_DONE:
			os_timer_setfn(&timer_sweep, user_channel_sweep, NULL);
			os_timer_arm(&timer_sweep, CHANNEL_SWEEP_TIME, true);
			break;

		/* --------------- */
		/* Running Signals */
		/* --------------- */
		
		// Button1 press event
		case SIG_RUN | PAR_RUN_BUTTON1:
			os_printf("button press\r\n");
			break;		

				
	};

	return;
};

// Application Function: user_gpio_init(void)
// Desc: Initializes the GPIO pins
void ICACHE_FLASH_ATTR user_gpio_init(void)
{
        // Disable GPIO interrupts during initialization
        ETS_GPIO_INTR_DISABLE();

        // Set pin functions to GPIO 
        PIN_FUNC_SELECT(SDA_MUX, SDA_FUNC);
        PIN_FUNC_SELECT(SCL_MUX, SCL_FUNC);
	PIN_FUNC_SELECT(HIGH_MUX, HIGH_FUNC);
	PIN_FUNC_SELECT(BUTTON1_MUX, BUTTON1_FUNC);

        // Set I2C pins to open-drain
        GPIO_REG_WRITE(
		GPIO_PIN_ADDR(GPIO_ID_PIN(SDA_PIN)), 
		GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(SDA_PIN))) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE));
        GPIO_REG_WRITE(
		GPIO_PIN_ADDR(GPIO_ID_PIN(SCL_PIN)), 
		GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(SCL_PIN))) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE));
	
        // Enable pins
        GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | SDA_BIT);
        GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | SCL_BIT);
        GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | HIGH_BIT);
        GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | BUTTON1_BIT);

	// Configure HIGH as high output
	gpio_output_set(HIGH_BIT, 0, HIGH_BIT, 0);

	// Configure Button 1 as input
	gpio_output_set(0, 0, 0, BUTTON1_BIT);

	// Register ISR 
        gpio_intr_handler_register(user_gpio_isr, 0);   			    // Register GPIO ISR
        gpio_pin_intr_state_set(GPIO_ID_PIN(BUTTON1_PIN), GPIO_PIN_INTR_NEGEDGE);   // BUTTON1 falling edge triggers

        // GPIO initialization
        gpio_init();

	// Start I2C pins high
        gpio_output_set(SDA_BIT, 0, SDA_BIT, 0);
       	gpio_output_set(SCL_BIT, 0, SCL_BIT, 0);

        // Re-enable GPIO interrupts
        ETS_GPIO_INTR_ENABLE();

        return;
};
