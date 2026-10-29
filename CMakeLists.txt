cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Build-wide settings that libmain.a was compiled with. Components built
# from source here (the VL53L5CX driver in particular) must use the same
# values, or the structs they share with libmain.a are laid out
# differently. This file is replaced together with libmain.a on every
# library refresh; do not edit it by hand.
set(_precompiled_build_config
    "${CMAKE_CURRENT_LIST_DIR}/components/gene_precompiled/build_config.cmake")
if(NOT EXISTS "${_precompiled_build_config}")
    message(FATAL_ERROR "Not found: ${_precompiled_build_config}")
endif()
include("${_precompiled_build_config}")

project(gene_firmware_for_solist_ai_dev_kit)
