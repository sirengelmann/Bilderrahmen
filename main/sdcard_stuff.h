#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "driver/gpio.h"
#include "sdmmc_cmd.h"


#define PIN_NUM_MISO 2
#define PIN_NUM_MOSI 15
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   13

#define PIN_NUM_SDPOWER 5



void sdcard_init();

void sdcard_deinit();

#define MAX_LEN 256

typedef struct node {
    char filename[MAX_LEN];
    struct node * next;
} filename_list_t;


filename_list_t* list_files(char* path, char* endswith);

bool has_ending(char* ending, char* str);

