// user_i2c.c

#include <user_i2c.h>

static uint8 ICACHE_FLASH_ATTR user_i2c_recv_ack(void);
static void ICACHE_FLASH_ATTR user_i2c_send_ack(uint8 ack_level);

static void ICACHE_FLASH_ATTR user_i2c_sda_set(bool state)
{
        // To drive the line low, pull the pin low (low output)
        // To "drive" the line high, let it float at Hi-Z (input)
        if (state == 0) {
                gpio_output_set(0, SDA_BIT, SDA_BIT, 0);
        } else {
                gpio_output_set(SDA_BIT, 0, SDA_BIT, 0);
        }

        return;
};

static void ICACHE_FLASH_ATTR user_i2c_scl_set(bool state)
{
        // To drive the line low, pull the pin low (low output)
        // To "drive" the line high, let it float at Hi-Z (input)
        if (state == 0) {
                gpio_output_set(0, SCL_BIT, SCL_BIT, 0);
        } else {
                gpio_output_set(SCL_BIT, 0, SCL_BIT, 0);
        }

        return;
};

static bool ICACHE_FLASH_ATTR user_i2c_scl_read(void)
{
        // Read and return SCL line
        return GPIO_INPUT_GET(GPIO_ID_PIN(SCL_PIN));
};

static bool ICACHE_FLASH_ATTR user_i2c_sda_read(void)
{
        // Read and return SDA line
        return GPIO_INPUT_GET(GPIO_ID_PIN(SDA_PIN));
};

static ICACHE_FLASH_ATTR uint8 user_i2c_read_bit(void)
{
        uint8 bit = 0;

        // To read a bit, SDA is first driven high. After a delay,
        // SCL is brought high. Data is read from the slave, then
        // the SCL line is brought low       
        os_delay_us(I2C_DELAY);
        user_i2c_scl_set(0);
        os_delay_us(I2C_DELAY);
        user_i2c_scl_set(1);
        os_delay_us(I2C_DELAY);


        bit = user_i2c_sda_read();
        os_delay_us(I2C_DELAY);

        return bit;
};

static void ICACHE_FLASH_ATTR user_i2c_write_bit(uint8 bit)
{
        // To write a bit, pull SCL low, write the desired bit to SDA,
        // then float SCL high delay and pull it back low
        user_i2c_sda_set(bit);
        os_delay_us(I2C_DELAY);
        user_i2c_scl_set(1);
        os_delay_us(I2C_DELAY);        
        user_i2c_scl_set(0);
        os_delay_us(I2C_DELAY);        

        return;
};

void ICACHE_FLASH_ATTR user_i2c_start_bit(void)
{
        // An I2C start bit begins with both SDA & SCL high. It then
        // pulls SDA low while keeping SCL high
        user_i2c_sda_set(1);
        user_i2c_scl_set(1);
        os_delay_us(I2C_DELAY);
        user_i2c_sda_set(0);
        os_delay_us(I2C_DELAY);
        user_i2c_scl_set(0);

        return;
};

void ICACHE_FLASH_ATTR user_i2c_stop_bit(void)
{
        // An I2C stop bit starts with SDA low and SCL HIGH. It then
        // pulls SDA high while keeping SCL high
        os_delay_us(I2C_DELAY);
        user_i2c_scl_set(1);
        os_delay_us(I2C_DELAY);
        user_i2c_sda_set(1);
        os_delay_us(I2C_DELAY);

        return;
};

uint8 ICACHE_FLASH_ATTR user_i2c_write_byte(uint8 byte)
{
        uint8 i = 0;    // Loop index
       
        os_delay_us(I2C_DELAY);

        // Write each bit from most significant to least
        for (i = 0; i < 8; i++) {
                user_i2c_write_bit((byte << i) & 0x80);
        }

        // Read/return ACK/NACK
        return user_i2c_recv_ack();
};

uint8 ICACHE_FLASH_ATTR user_i2c_read_byte(uint8 ack)
{
        int8 i = 0;     // Loop index
        uint8 byte = 0; // Read byte
        
        user_i2c_sda_set(1);

        // Read each bit from most significant to least
        for (i = 7; i >=0; i--) {
                byte |= user_i2c_read_bit() << i;
        }

        user_i2c_scl_set(0);
        os_delay_us(I2C_DELAY);

        // Send an ACK or NACK
        user_i2c_send_ack(ack);

        return byte;
};

static void ICACHE_FLASH_ATTR user_i2c_send_ack(uint8 ack_level)
{
        // To send an ACK, bring SDA low and pulse the clock.
        // To send a NACK, bring SDA high and pulse the clock

        user_i2c_scl_set(0);
        os_delay_us(I2C_DELAY);

        user_i2c_sda_set(ack_level);

        user_i2c_scl_set(0);
        os_delay_us(I2C_DELAY);
        user_i2c_scl_set(1);
        os_delay_us(I2C_DELAY);
        user_i2c_scl_set(0);
        os_delay_us(I2C_DELAY);

        user_i2c_sda_set(1);
        os_delay_us(I2C_DELAY);

        return;
};

static uint8 ICACHE_FLASH_ATTR user_i2c_recv_ack(void)
{
        // To check for an ACK, bring SDA high, pulse the clock,
        // then check to see if the slave has brought SDA low

        uint8 result = 0;

        user_i2c_sda_set(1);
        os_delay_us(I2C_DELAY);
        //user_i2c_scl_set(0);
        //os_delay_us(I2C_DELAY);
        user_i2c_scl_set(1);
        os_delay_us(I2C_DELAY);

        result = user_i2c_sda_read();

        //os_delay_us(I2C_DELAY);
        user_i2c_scl_set(0);
        os_delay_us(I2C_DELAY);
        //user_i2c_sda_set(0);
        //os_delay_us(I2C_DELAY);

        return result;
}
