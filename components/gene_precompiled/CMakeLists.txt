idf_component_register(
    INCLUDE_DIRS include
)

set(_prebuilt_lib "${CMAKE_CURRENT_LIST_DIR}/lib/libmain.a")

if(NOT EXISTS "${_prebuilt_lib}")
    message(FATAL_ERROR "Prebuilt library not found: ${_prebuilt_lib}")
endif()

add_prebuilt_library(gene_precompiled_lib "${_prebuilt_lib}"
    REQUIRES driver esp_timer nvs_flash
             vl53l5cx_uld bmi08x_sensor_api
             hs_pdm_mic_sph0641lu4h sd_card_manager
)

target_link_libraries(${COMPONENT_LIB} INTERFACE
    gene_precompiled_lib
    "-Wl,--undefined=user_init"
    "-Wl,--undefined=user_on_ready"
    "-Wl,--undefined=user_on_predict_result"
)
