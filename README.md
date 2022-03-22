# Toilet Occupancy Sensor

## Server兼ディスプレイ
M5Stack Basicをサーバーとする。<br>
一秒ごとに状態を受け取り表示する<br>

状態は以下の通り
* 青色（空き）
* 赤色（使用中）
* 黄色（5秒以上信号なし）
信号変化後の時間も表示する


## Client兼センサー
M5Stamp C3 Mateをクライアントとする。<br>
一秒ごとに状態を送信する<br>

## プロトコル
UDPで送りっぱなしにする。<br>
メッセージは一文字のみのパケットで構成する。<br>
* 'o' occupied 使用中
* 'v' vacant 空き

## common/common.h
プロジェクト外への参照ができないようなので、コピーすることにした。

```
$ ./copy_common_h.sh
```