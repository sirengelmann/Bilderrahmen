#include "esp_system.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
//#include "driver/sigmadelta.h"

void read_battery_setup(void){
	adc1_config_width(ADC_WIDTH_BIT_12);
	adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_11);

}



float read_battery_voltage(void){
	int* buf = malloc(100 * sizeof(int));
	if(buf == NULL) return -1.0;
	for(int i = 0; i < 100; i++){
		buf[i] = adc1_get_raw(ADC1_CHANNEL_4);
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
	float val = 0.0;
	for(int i = 0; i < 100; i++){
		val += ((float)buf[i] * 0.01);
	}

	return (val * 2 * 3.9 / (powf(2,12) - 1.0)) * (4.1 / 4.5);  //calibration value for the adc: (4.1 / 4.5)
}

float read_battery_percent(void){
	float val = (read_battery_voltage() - 3.3) * 100.0 / (4.1 - 3.3);
	return val > 100.0 ? 100.0 : val < 0.0 ? 0.0 : val;
}
