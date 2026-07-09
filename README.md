# gene_firmware_for_solist_ai_dev_kit

ESP32-S3 firmware SDK。`main/user_main.c` にフックを実装して振る舞いをカスタマイズする。

## 必要環境

- ESP-IDF v5.4.1
- Target: `esp32s3`

## ビルド & 書き込み

```bash
. $IDF_PATH/export.sh
git submodule update --init --recursive
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor -b 921600
```

## カスタマイズ

`main/user_main.c` にフックを実装する。利用可能なフックは [`docs/hooks.md`](docs/hooks.md) を参照。
