// user_i2c.h
// Authors: Christian Auspland & Matthew Blanchard
// Desc: Contains software i2c functions & definitions. The ESP8266 lacks i2c
//      hardware, "bit-banging: is necessary

#ifndef USER_I2C_H
#define USER_I2C_H

#include <user_interface.h>
#include <osapi.h>
#include <gpio.h>
#include <eagle_soc.h>
#include <ets_sys.h>

// I2C pin definitions - THESE SHOULD NOT BE CHANGED
#define SDA_PIN 5
#define SDA_BIT BIT5
#define SDA_MUX PERIPHS_IO_MUX_GPIO5_U
#define SDA_FUNC FUNC_GPIO5
#define SCL_PIN 4
#define SCL_BIT BIT4
#define SCL_MUX PERIPHS_IO_MUX_GPIO4_U
#define SCL_FUNC FUNC_GPIO4

// Delay time in microseconds
#define I2C_DELAY 2

// Application Function: user_i2c_sda_set(bool state);
// Desc: Sets the SDA line to state (high or low)
// Args:
//      bool state: The desired state for SDA
// static void ICACHE_FLASH_ATTR user_i2c_sda_set(bool state);

// Application Function: user_i2c_scl_set(bool state);
// Desc: Sets the SCL line to state (high or low)
// Args:
//      bool state: The desired state for SCL
// static void ICACHE_FLASH_ATTR user_i2c_scl_set(bool state);

// Application Function: user_i2c_start_bit(void);
// Desc: Sends an I2C start bit
void ICACHE_FLASH_ATTR user_i2c_start_bit(void);

// Application Function: user_i2c_stop_bit(void);
// Desc: Sends an I2C stop bit
void ICACHE_FLASH_ATTR user_i2c_stop_bit(void);

// Application Function: user_i2c_scl_read(void);
// Desc: Reads the SCL line
// Returns:
//      SCL line state
// static bool ICACHE_FLASH_ATTR user_i2c_scl_read(void);

// Application Function: user_i2c_sda_read(void);
// Desc: Reads the SDA line
// Returns:
//      SDA line state
// static bool ICACHE_FLASH_ATTR user_i2c_sda_read(void);

// Application Function: user_i2c_read_bit(void);
// Desc:
//      Reads a single bit from the slave device
// Returns:
//      The bit which was read
// static ICACHE_FLASH_ATTR uint8 user_i2c_read_bit(void);

// Application Function: user_i2c_write_bit(uint8 bit);
// Desc:
//      Writes a single bit to the slave device
// Args:
//      uint8 bit: Bit to be written
// static void ICACHE_FLASH_ATTR user_i2c_write_bit(uint8 bit);

// Application Function: user_i2c_write_byte(uint8 byte);
// Desc:
//      Writes a single byte to the slave device,
//      and checks for an ACK/NACK
// Args:
//      uint8 byte: Byte to be written
// Returns:
//      True, if a NACK was received
//      False, if an ACK was received
uint8 ICACHE_FLASH_ATTR user_i2c_write_byte(uint8 byte);

// Application Function: user_i2c_read_byte(void)
// Desc:
//      Reads a single byte from the slave device,
//      and sends an ACK/NACK
// Args:
//      uint8 ack: 0 to send an ACK, 1 to send a NACK
// Returns:
//      The byte which was read
uint8 ICACHE_FLASH_ATTR user_i2c_read_byte(uint8 ack);

// Application Function: user_i2c_send_ack(uint8 ack_level)
// Desc:
//      Sends a ACK/NACK to the slave device. An argument
//      of '0' indicates an ACK, and '1' indicates an ACK
// Args:
//      uint8 ack_level: Ack level (ACK/NACK)
//void ICACHE_FLASH_ATTR user_i2c_send_ack(uint8 ack_level);

// Application Function: user_i2c_recv_ack(void)
// Desc:
//      Checks for an ACK/NACK from the slave device
// Returns:
//      0, if an ACK was received
//      1, if a NACK was received
//uint8 ICACHE_FLASH_ATTR user_i2c_recv_ack(void);

#endif

