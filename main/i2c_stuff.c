#include "driver/i2c.h"
#include "hal/i2c_types.h"
#include "driver/gpio.h"

#define EXAMPLE_I2C_PORT_NUMBER				I2C_NUM_1		//I2C port number
#define EXAMPLE_I2C_SCL_GPIO				GPIO_NUM_22				//GPIO pin
#define EXAMPLE_I2C_SDA_GPIO				GPIO_NUM_19				//GPIO pin
#define EXAMPLE_I2C_FREQ_HZ					100000			//!< I2C master clock frequency
#define EXAMPLE_I2C_ADDR					0x32
#define EXAMPLE_I2C_WRITE_BIT				I2C_MASTER_WRITE
#define EXAMPLE_I2C_READ_BIT				I2C_MASTER_READ
#define EXAMPLE_I2C_ACK_CHECK_EN			0x1
#define EXAMPLE_I2C_ACK_CHECK_DIS			0x0
#define EXAMPLE_I2C_ACK_VAL					0x0
#define EXAMPLE_I2C_NACK_VAL				0x1


void i2c_test_func(){
    //----- CREATE THE I2C PORT -----
	int i2c_master_port = EXAMPLE_I2C_PORT_NUMBER;
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = EXAMPLE_I2C_SDA_GPIO;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = EXAMPLE_I2C_SCL_GPIO;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = EXAMPLE_I2C_FREQ_HZ;		//I2C frequency is the clock speed for a complete high low clock sequence
	i2c_param_config(i2c_master_port, &conf);
	i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);

	esp_err_t i2c_ret = ESP_OK;
	uint8_t data_whoami;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	//----- WRITE BYTES -----
	/*i2c_master_start(cmd);
	i2c_master_write_byte(cmd, EXAMPLE_I2C_ADDR | EXAMPLE_I2C_WRITE_BIT, EXAMPLE_I2C_ACK_CHECK_EN);
	i2c_master_write_byte(cmd, 0x02, EXAMPLE_I2C_ACK_CHECK_EN);
	i2c_master_write_byte(cmd, 0x7f, EXAMPLE_I2C_ACK_CHECK_EN);
	i2c_master_stop(cmd);*/
	//Send queued commands
	//i2c_ret = i2c_master_cmd_begin(EXAMPLE_I2C_PORT_NUMBER, cmd, (1000 / portTICK_RATE_MS));	//"(# / portTICK_RATE_MS)"=maximum wait time. This task will be blocked until all the commands have been sent (not thread-safe - if you want to use one I2C port in different tasks you need to take care of multi-thread issues)
	//i2c_cmd_link_delete(cmd);
	//if (i2c_ret == ESP_OK){}
		//READ BYTES
		//printf("I2C Write OK\n");

		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, EXAMPLE_I2C_ADDR | EXAMPLE_I2C_WRITE_BIT, EXAMPLE_I2C_ACK_CHECK_EN);
		i2c_master_write_byte(cmd, 0x0F, EXAMPLE_I2C_ACK_CHECK_EN);
		//i2c_master_read_byte(cmd, &data_whoami, EXAMPLE_I2C_ACK_VAL);
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, EXAMPLE_I2C_ADDR | EXAMPLE_I2C_READ_BIT, EXAMPLE_I2C_ACK_CHECK_EN);
		i2c_master_read_byte(cmd, &data_whoami, EXAMPLE_I2C_NACK_VAL);
		i2c_master_stop(cmd);
		//Send queued commands
		i2c_ret = i2c_master_cmd_begin(EXAMPLE_I2C_PORT_NUMBER, cmd, 1000 / portTICK_RATE_MS);	//"(# / portTICK_RATE_MS)"=maximum wait time. This task will be blocked until all the commands have been sent (not thread-safe - if you want to use one I2C port in different tasks you need to take care of multi-thread issues)
		i2c_cmd_link_delete(cmd);
		printf(i2c_ret == ESP_OK ? "I2C Read OK\n" : "something went wrong. oops! :-(\n");
		if(i2c_ret == ESP_OK) printf("whoami register: %d", data_whoami);


}
