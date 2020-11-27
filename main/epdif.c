/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "epdif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//#include "driver/spi_common.h"
//#include "driver/spi_master.h"

//#include <spi.h>


void DigitalWrite(gpio_num_t pin, uint32_t value) {
    /*digitalWrite(pin, value);*/
	gpio_set_level(pin, value);
}

int DigitalRead(gpio_num_t pin) {
    return gpio_get_level(pin);
}

void DelayMs(unsigned int delaytime) {
    /*delay(delaytime);
    */
	vTaskDelay(delaytime / portTICK_PERIOD_MS);
}

void SpiTransfer(unsigned char data_to_send) {

	uint32_t data[4];
	uint32_t garbage[4];

	data[0] = data_to_send;

	trans_desc.tx_buffer = data;
	trans_desc.rx_buffer = garbage;

	gpio_set_level(CS_PIN, 0);

	ESP_ERROR_CHECK(spi_device_transmit(handle, &trans_desc));

	gpio_set_level(CS_PIN, 1);
}

int IfInit(void) {
	gpio_set_direction(CS_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(RST_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(DC_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction(BUSY_PIN, GPIO_MODE_INPUT);

	//SPI stuff
	bus_config.sclk_io_num = CLK_PIN; // CLK
	bus_config.mosi_io_num = MOSI_PIN; // MOSI
	bus_config.miso_io_num = -1; // MISO not used
	bus_config.quadwp_io_num = -1; // Not used
	bus_config.quadhd_io_num = -1; // Not used

	ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &bus_config, 0));
	dev_config.address_bits = 0;
	dev_config.command_bits = 0;
	dev_config.dummy_bits = 0;
	dev_config.mode = 0;
	dev_config.duty_cycle_pos = 0;
	dev_config.cs_ena_posttrans = 0;
	dev_config.cs_ena_pretrans = 0;
	dev_config.clock_speed_hz = /*2000000;*/  SPI_MASTER_FREQ_13M;
	dev_config.spics_io_num = -1;  //Chip Select is done in software
	dev_config.flags = 0;
	dev_config.queue_size = 1;
	dev_config.pre_cb = NULL;
	dev_config.post_cb = NULL;

	ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &dev_config, &handle));

	//moved out of SpiTransfer
	trans_desc.addr = 0;
	trans_desc.cmd = 0;
	trans_desc.flags = 0;
	trans_desc.rxlength = 0;
	trans_desc.length = 8;

    return 0;
}

void IfSuspend(){
	//puts("inside IfSuspend");
	ESP_ERROR_CHECK(spi_bus_remove_device(handle));
	ESP_ERROR_CHECK(spi_bus_free(VSPI_HOST));
}


