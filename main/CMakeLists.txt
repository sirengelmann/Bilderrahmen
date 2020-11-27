set(srcs "imagedata000.c"
    "imagedata001.c"
    "imagedata002.c"
    "imagedata003.c"
    "img_low_bat.c"
    "epdif.c"
    "epd5in65f.c"
    "qdbmp.c"
    "sdcard_stuff.c"
    "floyd_steinberg.c"
    "pixel_converter.c"
    "lis3dh.c"
    "lis3dh_drv.c"
    "main.c"
    )

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")

