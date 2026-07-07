#ifndef PDM_HS_MIC_SPH0641U4H_H
#define PDM_HS_MIC_SPH0641U4H_H

#include "esp_err.h"
#include "driver/gpio.h"

/* Pin assignments for the renewed gene PCB (sensor slot pins).
 * Phase 6 spec section 3.2 -- MIC daughter board uses IO1 / IO2 of
 * the slot connector for PDM CLK / DATA (shared with other sensor
 * types' GPIO roles; these pins must be configured only after
 * AT24CS01 confirms a MIC board is attached). */
#define I2S_PDM_CLK_IO           GPIO_NUM_1
#define I2S_PDM_DIN_IO           GPIO_NUM_2

/* PDM clock presets. The driver no longer hard-codes "hypersonic":
 * the caller (mic_sensor.c) chooses clock_hz from config_manager
 * (mic.clock_hz). These macros are kept as named defaults / examples
 * matching the two recommended operating modes. */
#define PDM_CLK_SLEEP_HZ         250000     /* mode entry standby */
#define PDM_CLK_VOICE_HZ         1024000    /* voice / wake word (16 kHz PCM @ OSR 64) */
#define PDM_CLK_HYPERSONIC_HZ    4800000    /* ultrasonic        (75 kHz PCM @ OSR 64) */
#define OSR_VAL                  64
#define STANDBY_DELAY_MS         500

/* Initialize the I2S PDM RX channel.
 * Returns ESP_OK on success, ESP-IDF error code otherwise. Does not
 * panic the firmware on failure (callers must check the return). */
esp_err_t pdm_mic_init(void);

/* Start the channel in operating clock mode. Performs the
 * sleep -> standby -> operating clock transition required by the
 * SPH0641LU4H-1 mode-entry sequence.
 *
 * operating_clock_hz: PDM bit-clock to use after the warm-up.
 *   1.024 MHz typical for voice (16 kHz PCM @ OSR 64),
 *   4.8 MHz for ultrasonic / hypersonic mode (75 kHz PCM). */
esp_err_t pdm_mic_start(uint32_t operating_clock_hz);

// データ読み取り（増幅処理なしの生データ）
esp_err_t pdm_mic_read(void *dest, size_t size, size_t *bytes_read, uint32_t timeout_ms);

// 単に一時停止したい場合
esp_err_t pdm_mic_stop(void);

// 停止・リソース解放のための関数
esp_err_t pdm_mic_deinit(void);

#endif