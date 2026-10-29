# Generated together with lib/libmain.a. Do not edit by hand.
#
# Targets per zone that libmain.a was compiled with. The VL53L5CX driver
# component is compiled from source in this repo and must use the same
# value: it sets the size of VL53L5CX_Configuration and
# VL53L5CX_ResultsData, which both sides access.
idf_build_set_property(COMPILE_DEFINITIONS "VL53L5CX_NB_TARGET_PER_ZONE=1U" APPEND)
