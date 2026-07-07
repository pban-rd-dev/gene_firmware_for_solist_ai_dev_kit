# gene_firmware_for_solist_ai_dev_kit

## ビルド & 書き込み

```bash
. $IDF_PATH/export.sh
git submodule update --init --recursive
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor -b 921600
```

## ユーザコード

`main/user_main.c` を編集して振る舞いをカスタマイズする。
オーバーライド可能な関数は `components/gene_precompiled/include/precompiled_api.h` を参照。
