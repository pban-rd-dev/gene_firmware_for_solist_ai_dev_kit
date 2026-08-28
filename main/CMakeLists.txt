idf_component_register(
    SRCS "user_main.c"
    INCLUDE_DIRS "."
    REQUIRES
        gene_precompiled
        driver
        esp_timer
        nvs_flash
        vl53l5cx_uld
        bmi08x_sensor_api
    WHOLE_ARCHIVE
)
