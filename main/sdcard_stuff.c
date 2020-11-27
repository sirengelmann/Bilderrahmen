#include "sdcard_stuff.h"

#include "driver/spi_common.h"


static const char *SD_TAG = "SD SPI";

void sdcard_init(){
	gpio_set_direction((gpio_num_t) PIN_NUM_SDPOWER, GPIO_MODE_OUTPUT);
	gpio_set_level((gpio_num_t) PIN_NUM_SDPOWER, 0);  //enable sd card power

	//ESP_LOGI(SD_TAG, "Using SPI peripheral");

	sdmmc_host_t host = SDSPI_HOST_DEFAULT();
	sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
	slot_config.gpio_miso = PIN_NUM_MISO;
	slot_config.gpio_mosi = PIN_NUM_MOSI;
	slot_config.gpio_sck  = PIN_NUM_CLK;
	slot_config.gpio_cs   = PIN_NUM_CS;
	// This initializes the slot without card detect (CD) and write protect (WP) signals.
	// Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.

	// Options for mounting the filesystem.
	// If format_if_mount_failed is set to true, SD card will be partitioned and
	// formatted in case when mounting fails.
	esp_vfs_fat_sdmmc_mount_config_t mount_config = {
		.format_if_mount_failed = false,
		.max_files = 5,
		.allocation_unit_size = 16 * 1024
	};

	// Use settings defined above to initialize SD card and mount FAT filesystem.
	// Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
	// Please check its source code and implement error recovery when developing
	// production applications.
	sdmmc_card_t* card;
	esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			ESP_LOGE(SD_TAG, "Failed to mount filesystem. "
				"If you want the card to be formatted, set format_if_mount_failed = true.");
		} else {
			ESP_LOGE(SD_TAG, "Failed to initialize the card (%s). "
				"Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
		}
		return;
	}

	// Card has been initialized, print its properties
	//sdmmc_card_print_info(stdout, card);
}

void sdcard_deinit(){
	// All done, unmount partition and disable SDMMC or SPI peripheral
	esp_vfs_fat_sdmmc_unmount();
	gpio_set_level((gpio_num_t) PIN_NUM_SDPOWER, 1);  //disable sd card power
	ESP_LOGI(SD_TAG, "Card unmounted");
}

bool has_ending(char* ending, char* str){
	if(strlen(str) <= strlen(ending)) return false;
	int str_len = strlen(str);
	int ending_len = strlen(ending);
	char* ending_UPPER = (char*) malloc(ending_len * sizeof(char));
	strcpy(ending_UPPER, ending);
	for(int i = 0; i < ending_len; i++){
		ending_UPPER[i] = toupper(ending[i]);
	}
	char* temp_copy = (char*) malloc(str_len * sizeof(char*));
	for(int i = 0; i < ending_len; i++){
		temp_copy[i] = str[str_len - ending_len + i];
	}

	if(0 == strncmp(temp_copy, ending, ending_len) || 0 == strncmp(temp_copy, ending_UPPER, ending_len)) {
		free(temp_copy);
		free(ending_UPPER);
		return true;
	}
	free(temp_copy);
	free(ending_UPPER);
	return false;
}

filename_list_t* list_files(char* path, char* endswith){
	bool specific_ending = true;
	if(endswith == NULL || endswith[0] == '*') specific_ending = false;

	int filecount = 0;

	filename_list_t* head = NULL;

	struct dirent* dp;

	DIR* dir = opendir(path);

	if (!dir)
		return NULL;

	while ((dp = readdir(dir)) != NULL)
	{
		//printf("%s\n", dp->d_name);

		if(
			dp->d_type == DT_REG					/*is regular file*/
			&& (											/*AND*/
				(	specific_ending	/*looking for a specific file ending AND ending matches*/
					&& has_ending(endswith, dp->d_name))	/*OR*/
				|| (!specific_ending)) /*not looking for a specific file ending*/
		){
			if(!filecount){		/*first entry in linked list*/
				head = (filename_list_t*) malloc(sizeof(filename_list_t));
				head->next = NULL;
				strcpy(head->filename, dp->d_name);
				//printf("%s\n", dp->d_name);
				filecount++;
			} else {			/*not first entry in linked list*/
				filename_list_t* temp = head;
				while(temp->next != NULL) temp = temp->next;
				temp->next = (filename_list_t*) malloc(sizeof(filename_list_t));
				temp = temp->next;
				temp->next = NULL;
				strcpy(temp->filename, dp->d_name);
				//printf("%s\n", dp->d_name);
				filecount++;
			}
		}
	}

	// Close directory stream
	closedir(dir);
	return head;
}


