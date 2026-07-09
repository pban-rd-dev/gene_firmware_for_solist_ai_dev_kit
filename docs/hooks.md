# フック API

`main/user_main.c` に以下の関数を定義すると、firmware の該当タイミングで呼ばれる。未定義のフックは何もしないデフォルト実装が動く。宣言は `components/gene_precompiled/include/precompiled_api.h`。

## user_init

```c
void user_init(void);
```

起動時に一度だけ呼ばれる。UART / NVS / config / LED / sensor_task / ml_bridge の初期化と FreeRTOS task 作成の後、`user_on_ready` より前。

`ESP_LOG` 使用可、FreeRTOS 稼働中。長時間処理は別 task を作って行うこと。

## user_on_ready

```c
void user_on_ready(void);
```

`app_main` の末尾で一度だけ呼ばれる。全 subsystem が初期化され、FreeRTOS task が稼働中。app_main は本フックから return するが、firmware は task 経由で動き続ける。

## user_on_predict_result

```c
void user_on_predict_result(const float *scores, size_t n);
```

`>ML_GET_RESULT` の OK 応答を UART に送出する直前に呼ばれる。`scores` は bfloat16 から変換された `n` 個の float 配列。ポインタは呼び出し中のみ有効。

UART 応答の抑制や書き換えはできない (passive)。

## 実装例

```c
#include "esp_log.h"
#include "precompiled_api.h"

static const char *TAG = "user";

void user_init(void)
{
    ESP_LOGI(TAG, "user override active");
}

void user_on_predict_result(const float *scores, size_t n)
{
    if (n > 0 && scores[0] > 0.9f) {
        ESP_LOGI(TAG, "high confidence: %.3f", scores[0]);
    }
}
```
