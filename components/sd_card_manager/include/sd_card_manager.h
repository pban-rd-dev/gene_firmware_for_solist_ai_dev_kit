#ifndef GENE_SD_CARD_MANAGER_H
#define GENE_SD_CARD_MANAGER_H

#include <stdbool.h>
#include "driver/gpio.h"
#include "esp_err.h"

/* SD slot is wired in SDSPI mode on the gene PCB (verified working with
 * the gene_sdspi test firmware). Mount happens lazily, only when
 * sd_card_init() is called from the active sensor's driver (currently
 * MIC), so the SD pins are not driven for sensor boards that do not
 * use them. Pin defines are kept local to this component (parallel to
 * the PDM mic component's I2S_PDM_*_IO defs).
 *
 * Do NOT switch this to SDMMC mode. See docs/sd_card_sdspi_design.md
 * for the full rationale and revert-prevention checklist.
 *
 * Pin mapping matches the SD card slot's native SPI-mode contacts:
 *   GPIO10 -> CS  (= card pin 1 / DAT3)
 *   GPIO11 -> MOSI (= card pin 2 / CMD / DI)
 *   GPIO12 -> CLK (= card pin 5)
 *   GPIO13 -> MISO (= card pin 7 / DAT0 / DO)
 */
#define SD_PIN_CS    GPIO_NUM_10
#define SD_PIN_MOSI  GPIO_NUM_11
#define SD_PIN_CLK   GPIO_NUM_12
#define SD_PIN_MISO  GPIO_NUM_13

/* Mount the SD card at /sdcard. Idempotent: subsequent calls are
 * no-ops and return ESP_OK if already mounted. Returns the ESP-IDF
 * error code on failure. Does NOT auto-format on mount failure
 * (Phase 6 spec 10.2.1 -- prevents data loss on existing cards). */
esp_err_t sd_card_init(void);

/* Unmount and release the card. */
void sd_card_deinit(void);

/* True if sd_card_init() succeeded since the last deinit. */
bool sd_card_is_mounted(void);

#endif
