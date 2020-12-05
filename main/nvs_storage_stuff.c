#include "nvs_storage_stuff.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"


void nvs_flash_stuff_init_func(void){
	// Initialize NVS
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK( err );
}

void nvs_flash_stuff_writeInt(unsigned char* key, int number){
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	} else {
		// Write
		err = nvs_set_i32(my_handle, (const char*)key, number);
		printf((err != ESP_OK) ? "Failed to write!\n" : "Done writing\n");

		err = nvs_commit(my_handle);
		printf((err != ESP_OK) ? "Failed to commit!\n" : "Done committing\n");

		// Close
		nvs_close(my_handle);
	}
}

void nvs_flash_stuff_writeString(unsigned char* key, unsigned char* str){
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	} else {
		// Write
		err = nvs_set_str(my_handle, (const char*)key, (const char*) str);
		printf((err != ESP_OK) ? "Failed to write!\n" : "Done writing\n");

		err = nvs_commit(my_handle);
		printf((err != ESP_OK) ? "Failed to commit!\n" : "Done committing\n");

		// Close
		nvs_close(my_handle);
	}
}

int nvs_flash_stuff_readInt(unsigned char* key){
	nvs_handle_t my_handle;
	int val = 0;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	} else {

		// Read
		err = nvs_get_i32(my_handle, (const char*)key, &val);
		switch (err) {
			case ESP_OK:
				printf("value = %d\n", val);
				break;
			case ESP_ERR_NVS_NOT_FOUND:
				printf("key \"%s\" did not lead to a value!\n", key);
				break;
			default :
				printf("Error (%s) reading!\n", esp_err_to_name(err));
		}

		// Close
		nvs_close(my_handle);
	}
	return val;
}

unsigned char* nvs_flash_stuff_readString(unsigned char* key, unsigned int length){
	nvs_handle_t my_handle;
	unsigned char* buf = NULL;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	} else {

		// Read
		buf = (unsigned char*) malloc(length*sizeof(unsigned char));
		err = nvs_get_str(my_handle, (const char*)key, (char*)buf, (size_t*)&length);
		switch (err) {
			case ESP_OK:
				printf("value = %s\n", buf);
				break;
			case ESP_ERR_NVS_NOT_FOUND:
				printf("key \"%s\" did not lead to a value!\n", key);
				break;
			default :
				printf("Error (%s) reading!\n", esp_err_to_name(err));
		}

		// Close
		nvs_close(my_handle);
	}
	return buf;
}

void nvs_flash_stuff_deleteValueByKey(unsigned char* key){
	nvs_handle_t my_handle;
	esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
	} else {

		err = nvs_erase_key(my_handle, (const char*)key);
		switch (err) {
			case ESP_OK:
				puts("Value found");
				break;
			case ESP_ERR_NVS_NOT_FOUND:
				printf("key \"%s\" did not lead to a value!\n", key);
				break;
			default :
				printf("Error (%s) !\n", esp_err_to_name(err));
		}

		err = nvs_commit(my_handle);
		printf((err != ESP_OK) ? "Failed to commit!\n" : "Done committing\n");

		// Close
		nvs_close(my_handle);
	}
	return;
}


