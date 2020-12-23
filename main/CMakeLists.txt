set(srcs "img_low_bat.c"
    "epdif.c"
    "epd5in65f.c"
    "qdbmp.c"
    "sdcard_stuff.c"
    "floyd_steinberg.c"
    "pixel_converter.c"
    "lis3dh.c"
    "lis3dh_drv.c"
    "nvs_storage_stuff.c"
    "pictureframe_management_functions.c"
    "batt_man.c"
    "main.c"
    )

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")

