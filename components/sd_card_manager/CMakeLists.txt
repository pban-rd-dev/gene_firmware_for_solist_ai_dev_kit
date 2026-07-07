idf_component_register(
    SRCS "sd_card_manager.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES fatfs sdmmc driver esp_driver_sdspi
)
