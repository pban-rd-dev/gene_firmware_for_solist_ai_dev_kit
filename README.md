# gene_firmware_for_solist_ai_dev_kit

株式会社ピーバンドットコムが提供する [Solist-AI™ × gene｜ノーコード組み込みAI開発キット](https://www.p-ban.com/services/gene/solist-ai.html) 用の ESP32-S3 プリコンパイル済み firmware SDK。

## Solist-AI™ × gene｜ノーコード組み込みAI開発キット とは

株式会社ピーバンドットコムが提供する、オンデバイス機械学習の評価・プロトタイピング向け開発キット。ESP32-S3 と複数のセンサーを搭載し、収集したセンサーデータに対して ML 推論をキット単体で実行できる。

詳細は製品ページを参照: <https://www.p-ban.com/services/gene/solist-ai.html>

## このリポジトリは何か

Solist-AI™ × gene｜ノーコード組み込みAI開発キット 用 firmware を **プリコンパイル済みライブラリ + hook API** の形式で提供する SDK。

firmware 本体 (センサー driver、ML runtime、UART command handler、FreeRTOS task 群) は `components/gene_precompiled/lib/libmain.a` として同梱されている。利用者は `main/user_main.c` に hook 関数を実装するだけで、firmware 本体に手を入れずに振る舞いをカスタマイズできる。

hook の一覧と仕様は [`docs/hooks.md`](docs/hooks.md) を参照。

## 主なユースケース

- 音声 / 音響イベント認識 (MEMS マイク + ML)
- モーション / ジェスチャ認識 (IMU + ML)
- 近接 / 物体検知 (ToF ranging)
- 温度湿度モニタリング
- 独自 ML モデルのオンデバイス評価

## 動作環境 (ホスト側)

- ESP-IDF **v5.4.1**
- Git (submodule 取得のため)
- ESP-IDF が動作するホスト OS (Linux / macOS / Windows)

## ターゲット

- MCU: **ESP32-S3**
- Flash: 16MB (ESP32-S3-WROOM-1-N16R8)

## ビルド & 書き込み

```bash
. $IDF_PATH/export.sh
git submodule update --init --recursive
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor -b 921600
```

書き込み用のシリアルポート (`/dev/ttyUSB0`) は環境に合わせて置き換えること。

## カスタマイズ

`main/user_main.c` に hook を実装する。未定義の hook はプリコンパイル済み firmware 側の weak default (no-op) が動く。必要なものだけを override すればよい。

詳細は [`docs/hooks.md`](docs/hooks.md) を参照。

## 今後のリリース

以下の方向で機能追加を予定している。

- ハードウェア機能へアクセスするための追加 hook
- センサーデータ取得・加工 API の拡充
- ML 推論周辺 API の拡張
- 外部システム連携 (ホスト / クラウド) の強化

プリコンパイル済みライブラリは上流 firmware の更新に追従して随時リフレッシュされる。

## ライセンス

本リポジトリの内容は Solist-AI™ × gene｜ノーコード組み込みAI開発キット の購入者による、同キット向けのカスタマイズ用途に限り使用を許可する。詳細は [`LICENSE.ja`](LICENSE.ja) (日本語 / 正本) または [`LICENSE`](LICENSE) (English) を参照。

Copyright © 株式会社ピーバンドットコム. All rights reserved.
