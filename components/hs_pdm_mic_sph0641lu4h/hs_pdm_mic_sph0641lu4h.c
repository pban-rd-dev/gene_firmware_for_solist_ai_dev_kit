#include <stdio.h>
#include "hs_pdm_mic_sph0641lu4h.h"

#include "driver/i2s_pdm.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "PDM_HS_MIC";
static i2s_chan_handle_t rx_handle = NULL;

static esp_err_t reconfigure_pdm_clock(uint32_t pdm_clk_hz) {
    i2s_pdm_rx_clk_config_t clk_cfg = {
        .sample_rate_hz = pdm_clk_hz / OSR_VAL,
        .clk_src = I2S_CLK_SRC_DEFAULT,
        .dn_sample_mode = (i2s_pdm_dsr_t)64,
    };
    return i2s_channel_reconfig_pdm_rx_clock(rx_handle, &clk_cfg);
}

esp_err_t pdm_mic_init(void) {
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num = 4;
    chan_cfg.dma_frame_num = 512;

    esp_err_t ret = i2s_new_channel(&chan_cfg, NULL, &rx_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "i2s_new_channel failed: %s", esp_err_to_name(ret));
        rx_handle = NULL;
        return ret;
    }

    i2s_pdm_rx_config_t pdm_rx_cfg = {
        .clk_cfg = {
            .sample_rate_hz = PDM_CLK_SLEEP_HZ / OSR_VAL,
            .clk_src = I2S_CLK_SRC_DEFAULT,
            .dn_sample_mode = (i2s_pdm_dsr_t)64,
        },
        .slot_cfg= {
            .data_bit_width = I2S_DATA_BIT_WIDTH_16BIT,
            .slot_mode = I2S_SLOT_MODE_MONO,
            .slot_mask = I2S_PDM_SLOT_LEFT, 
        },
        .gpio_cfg = {
            .clk = I2S_PDM_CLK_IO,
            .din = I2S_PDM_DIN_IO,
        },
    };

    ret = i2s_channel_init_pdm_rx_mode(rx_handle, &pdm_rx_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize PDM mic: %s", esp_err_to_name(ret));
        return ret;
    }
    return ESP_OK;
}

esp_err_t pdm_mic_start(uint32_t operating_clock_hz) {
    if (rx_handle == NULL) return ESP_ERR_INVALID_STATE;

    ESP_LOGI(TAG, "Starting I2S channel (target clock=%lu Hz)...",
             (unsigned long)operating_clock_hz);

    /* SPH0641LU4H-1 mode-entry sequence:
       sleep clock -> enable -> wait -> disable -> operating clock -> enable. */
    i2s_channel_disable(rx_handle);
    reconfigure_pdm_clock(PDM_CLK_SLEEP_HZ);
    i2s_channel_enable(rx_handle);
    vTaskDelay(pdMS_TO_TICKS(STANDBY_DELAY_MS));

    i2s_channel_disable(rx_handle);
    reconfigure_pdm_clock(operating_clock_hz);
    esp_err_t ret = i2s_channel_enable(rx_handle);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Mic is now running at %lu Hz PDM clock",
                 (unsigned long)operating_clock_hz);
    } else {
        ESP_LOGE(TAG, "i2s_channel_enable failed: %s", esp_err_to_name(ret));
    }
    return ret;
}

esp_err_t pdm_mic_stop(void) {
    if (rx_handle == NULL) return ESP_ERR_INVALID_STATE;
    return i2s_channel_disable(rx_handle);
}

esp_err_t pdm_mic_read(void *dest, size_t size, size_t *bytes_read, uint32_t timeout_ms) {
    return i2s_channel_read(rx_handle, dest, size, bytes_read, pdMS_TO_TICKS(timeout_ms));
}

// 停止・リソース解放のための関数
esp_err_t pdm_mic_deinit(void) {
    if (rx_handle == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    // 1. チャンネルを無効化（ハードウェアのクロック停止）
    i2s_channel_disable(rx_handle);

    // 2. チャンネルのリソースを削除
    //    もしアプリ終了まで保持し続けたいなら del_channel は不要ですが、
    //    initを毎回呼ぶ設計なら必須です。
    esp_err_t ret = i2s_del_channel(rx_handle);
    if (ret == ESP_OK) {
        rx_handle = NULL;
        ESP_LOGI(TAG, "PDM Mic deinitialized and channel deleted");
    }

    return ret;
}
