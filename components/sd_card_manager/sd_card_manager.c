/* gene v2 PCB の SD カードスロットは SDSPI 配線。SDMMC 1-bit に
 * 戻さないこと (送 op_cond timeout で動かなくなる)。設計理由・
 * 切り戻し禁止チェックリストは docs/sd_card_sdspi_design.md を参照。 */
#include "sd_card_manager.h"

#include "driver/gpio.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#include <stdio.h>
#include <string.h>

static sdmmc_card_t *s_card = NULL;
static bool s_bus_initialized = false;
static const char *TAG = "sd_card";
static const char mount_point[] = "/sdcard";

/* SPI3_HOST is dedicated to the SD card slot. SPI2_HOST is owned by
 * ml_bridge (Solist-AI master). */
#define SDSPI_BUS_HOST  SPI3_HOST

esp_err_t sd_card_init(void)
{
    if (s_card) return ESP_OK;   /* idempotent */

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        /* Phase 6 spec 10.2.1: do NOT format on mount failure -- the
         * previous "true" would silently wipe an existing SD card on
         * a transient mount error. */
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024,
    };

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SDSPI_BUS_HOST;
    /* SDSPI driver still negotiates the SD init handshake at ~400 kHz
     * internally; this cap is the post-init transfer ceiling. 20 MHz
     * is the ESP-IDF documented SDSPI maximum. */
    host.max_freq_khz = 20000;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = SD_PIN_MOSI,
        .miso_io_num = SD_PIN_MISO,
        .sclk_io_num = SD_PIN_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    esp_err_t ret = spi_bus_initialize(SDSPI_BUS_HOST, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "spi_bus_initialize failed: %s", esp_err_to_name(ret));
        return ret;
    }
    s_bus_initialized = (ret == ESP_OK);

    /* SDSPI requires MISO to idle HIGH when the card releases the line.
     * ESP32-S3 IOMUX does not auto-enable pullup on SPI MISO, and the
     * gene PCB does not have an external pullup on this pin -- enable
     * the internal one explicitly to avoid INVALID_RESPONSE on CMD8. */
    gpio_set_pull_mode(SD_PIN_MISO, GPIO_PULLUP_ONLY);

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_PIN_CS;
    slot_config.host_id = SDSPI_BUS_HOST;

    ret = esp_vfs_fat_sdspi_mount(mount_point, &host,
                                  &slot_config, &mount_config,
                                  &s_card);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "mount failed: %s", esp_err_to_name(ret));
        s_card = NULL;
        if (s_bus_initialized) {
            spi_bus_free(SDSPI_BUS_HOST);
            s_bus_initialized = false;
        }
        return ret;
    }

    sdmmc_card_print_info(stdout, s_card);
    ESP_LOGI(TAG, "SD card mounted at %s (SDSPI)", mount_point);
    return ESP_OK;
}

void sd_card_deinit(void)
{
    if (s_card) {
        esp_vfs_fat_sdcard_unmount(mount_point, s_card);
        s_card = NULL;
    }
    if (s_bus_initialized) {
        spi_bus_free(SDSPI_BUS_HOST);
        s_bus_initialized = false;
    }
}

bool sd_card_is_mounted(void)
{
    return s_card != NULL;
}
