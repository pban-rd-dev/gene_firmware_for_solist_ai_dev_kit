#pragma once

/*
 * ST VL53L5CX UltraLite Driver platform layer header.
 *
 * vl53l5cx_api.h includes "platform.h". The driver calls the I/O
 * functions by their unprefixed names (RdByte, WrByte, RdMulti,
 * WrMulti, Reset_Sensor, SwapBuffer, WaitMs), so this file declares
 * exactly those names.
 *
 * The implementation lives in main/vl53l5cx_platform.c
 * (ESP32-S3, I2C v2 master).
 */

#include <stdint.h>

typedef struct {
    uint16_t address;   /* 7-bit I2C address (default 0x29 for VL53L5CX) */
} VL53L5CX_Platform;

/*
 * Multi-target output per zone. ST UltraLite driver allows 1..4.
 * 1 = closest/strongest only (smallest results buffer, fastest).
 */
#ifndef VL53L5CX_NB_TARGET_PER_ZONE
#define VL53L5CX_NB_TARGET_PER_ZONE     1U
#endif

#define VL53L5CX_COMMS_CHUNK_SIZE       1024

#ifdef __cplusplus
extern "C" {
#endif

uint8_t RdByte (VL53L5CX_Platform *p_platform, uint16_t RegisterAddress, uint8_t *p_value);
uint8_t WrByte (VL53L5CX_Platform *p_platform, uint16_t RegisterAddress, uint8_t value);
uint8_t RdMulti(VL53L5CX_Platform *p_platform, uint16_t RegisterAddress, uint8_t *p_values, uint32_t size);
uint8_t WrMulti(VL53L5CX_Platform *p_platform, uint16_t RegisterAddress, uint8_t *p_values, uint32_t size);
uint8_t Reset_Sensor(VL53L5CX_Platform *p_platform);
void    SwapBuffer(uint8_t *buffer, uint16_t size);
uint8_t WaitMs (VL53L5CX_Platform *p_platform, uint32_t TimeMs);

#ifdef __cplusplus
}
#endif
