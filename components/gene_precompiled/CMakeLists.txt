idf_component_register(
    INCLUDE_DIRS include
)

set(_prebuilt_lib "${CMAKE_CURRENT_LIST_DIR}/lib/libmain.a")

if(NOT EXISTS "${_prebuilt_lib}")
    message(FATAL_ERROR "Prebuilt library not found: ${_prebuilt_lib}")
endif()

# REQUIRES must cover every ESP-IDF component libmain.a links against,
# because the archive carries the whole firmware:
#   freertos                          PDM mic driver
#   fatfs / sdmmc / esp_driver_sdspi  SD card driver
#   bt                                NimBLE host (BLE control transport)
#   esp_hw_support                    ditto
# The PDM mic and SD card drivers are inside libmain.a, so no component
# supplies them. The third-party sensor drivers stay as components: they
# are ST/Bosch submodules built from source.
#
# Keep this list in sync with main/CMakeLists.txt REQUIRES in the
# gene_firmware_for_solist_ai repo. The refresh workflow does not touch
# this file, so a new dependency there has to be mirrored here by hand.
add_prebuilt_library(gene_precompiled_lib "${_prebuilt_lib}"
    REQUIRES driver esp_timer nvs_flash freertos esp_hw_support
             fatfs sdmmc esp_driver_sdspi
             bt
             vl53l5cx_uld bmi08x_sensor_api
)

target_link_libraries(${COMPONENT_LIB} INTERFACE
    gene_precompiled_lib
    "-Wl,--undefined=user_init"
    "-Wl,--undefined=user_on_ready"
    "-Wl,--undefined=user_on_predict_result"
)
