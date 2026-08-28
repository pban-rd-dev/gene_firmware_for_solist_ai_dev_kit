# Bosch BMI08x Sensor API component wrapper.
#
# The official driver is fetched as a git submodule under upstream/
# (see ../../.gitmodules). License: BSD-3-Clause (Bosch).
#
# Layout:
#   upstream/bmi08a.c      -- accelerometer driver
#   upstream/bmi08g.c      -- gyroscope driver
#   upstream/bmi08xa.c     -- common (BMI08x) helpers
#   upstream/bmi08*.h      -- public headers

set(_dir "${CMAKE_CURRENT_LIST_DIR}")
set(_upstream "${_dir}/upstream")
set(_api_c "${_upstream}/bmi08a.c")

if(NOT EXISTS "${_api_c}")
    message(FATAL_ERROR
        "\n"
        "BMI08x Sensor API source not found.\n"
        "  expected: ${_api_c}\n"
        "\n"
        "The driver is a git submodule. From the project root run:\n"
        "    git submodule update --init --recursive\n")
endif()

idf_component_register(
    SRCS
        "${_upstream}/bmi08a.c"
        "${_upstream}/bmi08g.c"
        "${_upstream}/bmi08xa.c"
    INCLUDE_DIRS
        "${_upstream}"
    REQUIRES driver
)
