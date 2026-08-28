# VL53L5CX UltraLite Driver component wrapper.
#
# The ST official driver is brought in as a git submodule at upstream/
# (see ../../.gitmodules). This wrapper exposes the ULD as an ESP-IDF
# component while keeping the submodule contents untouched.
#
# Layout:
#   upstream/modules/vl53l5cx_api.{c,h}        -- ST core driver
#   upstream/modules/vl53l5cx_buffers.h         -- firmware blob
#   upstream/LICENSE.md                         -- BSD-3-Clause
#   inc/platform.h                              -- platform layer header (this repo)

set(_uld_dir "${CMAKE_CURRENT_LIST_DIR}")
set(_uld_modules "${_uld_dir}/upstream/modules")
set(_uld_api_c "${_uld_modules}/vl53l5cx_api.c")

if(NOT EXISTS "${_uld_api_c}")
    message(FATAL_ERROR
        "\n"
        "VL53L5CX UltraLite Driver source not found.\n"
        "  expected: ${_uld_api_c}\n"
        "\n"
        "The ST driver is a git submodule. From the project root run:\n"
        "    git submodule update --init --recursive\n")
endif()

idf_component_register(
    SRCS "${_uld_api_c}"
    INCLUDE_DIRS
        "${_uld_modules}"   # vl53l5cx_api.h, vl53l5cx_buffers.h
        "inc"               # platform.h shim
    REQUIRES driver
)
