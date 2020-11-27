#include "lis3dh_drv.h"
#include "driver/i2c.h"
#include "hal/i2c_types.h"
#include "driver/gpio.h"

#define LIS3DH_I2C_PORT_NUMBER			I2C_NUM_1			//I2C port number
#define LIS3DH_I2C_SCL_GPIO				GPIO_NUM_22			//GPIO pin
#define LIS3DH_I2C_SDA_GPIO				GPIO_NUM_19			//GPIO pin
#define LIS3DH_I2C_FREQ_HZ				100000				//!< I2C master clock frequency
#define LIS3DH_I2C_ADDR					0x32
#define LIS3DH_I2C_WRITE_BIT			I2C_MASTER_WRITE
#define LIS3DH_I2C_READ_BIT				I2C_MASTER_READ
#define LIS3DH_I2C_ACK_CHECK_EN			0x1
#define LIS3DH_I2C_ACK_CHECK_DIS		0x0
#define LIS3DH_I2C_ACK_VAL				0x0
#define LIS3DH_I2C_NACK_VAL				0x1

i2c_config_t lis3dh_conf;

void lis3dh_drv_init(void){
	//----- CREATE THE I2C PORT -----
	int i2c_master_port = LIS3DH_I2C_PORT_NUMBER;
	lis3dh_conf.mode = I2C_MODE_MASTER;
	lis3dh_conf.sda_io_num = LIS3DH_I2C_SDA_GPIO;
	lis3dh_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	lis3dh_conf.scl_io_num = LIS3DH_I2C_SCL_GPIO;
	lis3dh_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	lis3dh_conf.master.clk_speed = LIS3DH_I2C_FREQ_HZ;		//I2C frequency is the clock speed for a complete high low clock sequence
	i2c_param_config(i2c_master_port, &lis3dh_conf);
	i2c_driver_install(i2c_master_port, lis3dh_conf.mode, 0, 0, 0);
}

unsigned char lis3dh_drv_read(unsigned char addr){
	unsigned char return_data;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, LIS3DH_I2C_ADDR | LIS3DH_I2C_WRITE_BIT, LIS3DH_I2C_ACK_CHECK_EN);
	i2c_master_write_byte(cmd, addr, LIS3DH_I2C_ACK_CHECK_EN);

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, LIS3DH_I2C_ADDR | LIS3DH_I2C_READ_BIT, LIS3DH_I2C_ACK_CHECK_EN);
	i2c_master_read_byte(cmd, &return_data, LIS3DH_I2C_NACK_VAL);
	i2c_master_stop(cmd);
	//Send queued commands
	esp_err_t i2c_ret = i2c_master_cmd_begin(LIS3DH_I2C_PORT_NUMBER, cmd, 10 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	if(i2c_ret != ESP_OK) printf("I2C read failed at address %#02x\n", addr);
	return return_data;
}

void lis3dh_drv_write(unsigned char addr, unsigned char data){
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, LIS3DH_I2C_ADDR | LIS3DH_I2C_WRITE_BIT, LIS3DH_I2C_ACK_CHECK_EN);
	i2c_master_write_byte(cmd, addr, LIS3DH_I2C_ACK_CHECK_EN);
	i2c_master_write_byte(cmd, data, LIS3DH_I2C_ACK_CHECK_EN);
	i2c_master_stop(cmd);
	//Send queued commands
	esp_err_t i2c_ret = i2c_master_cmd_begin(LIS3DH_I2C_PORT_NUMBER, cmd, 10 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	if(i2c_ret != ESP_OK) printf("I2C write failed at address %#02x\n", addr);
}


