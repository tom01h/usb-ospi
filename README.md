# PCとFPGAをUSB接続する

## 概要
以下の接続でPCとFPGA間の高速転送をしたいプロジェクト。  
今はRead120Mbps,Write100Mbpsくらい。  
LinuxPC ←(USB-HS)→ USB3300 ←(ULPI)→ STM32H7B0 ←(OCTO SPI)→ FPGA

FPGA内ではAXIマスターポートをつける予定だけど、まだSPIクロックで動くBRAMにアクセスできるだけ。

USB転送はPCでは[libusb](https://github.com/libusb/libusb)、STM32では[tinyusb](https://github.com/hathach/tinyusb)を使ってVendorSpecificなバルク転送をします。  
OCTO SPIはSTM32はHALを使って、FPGAは適当に作ったRTLを使います。

## 部品
- PCはLinuxPCならたぶん何でもよい
- 基板を1枚こしらえました（[スケマ](https://github.com/tom01h/usb-ospi/blob/master/PCB/QMTECH/ospi.pdf)）
    - この基板の動作確認は200Tのみ
    - [8ピンのデータ用type-Cコネクタ](https://ja.aliexpress.com/item/1005004277420642.html)
    - [6ピンの電源用type-Cコネクタ](https://ja.aliexpress.com/item/1005004384274891.html)
    - USB3300-EZK
    - 24MHz 3225 のパッシブクォーツ水晶発振器 (2と4がグランド)
    - 1206 の抵抗とかコンデンサ
    - コネクタは次の組み合わせでぎりぎり干渉しない
        - STM32をとめる[コネクタ](https://ja.aliexpress.com/item/32892386779.html)
        - FPGAをとめる[コネクタオス](https://ja.aliexpress.com/item/1005004843759392.html)
            - これの11.2を、ピンが基板からはみ出ないところまでプラスチックをずらして使用
            - メスは普通のやつ
    - 他…
- QMTECHのFPGA (どっちか1個)
    - [200T](https://ja.aliexpress.com/item/1005002960622091.html)
    - [100T](https://ja.aliexpress.com/item/4000170003461.html)
- STM32H7
    - [WeActStudioのボード](https://ja.aliexpress.com/item/1005004466900940.html)
## 使い方
### ホスト
```
cd host/
cmake .
make
sudo ./main
```
### ファームウェア
- `firm/tinyusb`の下にtinyusbをクローンする
    - このリポジトリにあるファイルで上書きする
- usb_octospi.iocからプロジェクトを作る
    - このリポジトリにあるファイルで上書きする
- [ここ](https://github.com/hathach/tinyusb/discussions/633)に従って参照パスを追加する
```
- Right click the project, go to properties. in C/C++ General -> Paths and Symbols, in the "Includes" tab add a path, make it a workspace path, point it to the tinyusb/src directory. Then, in the "Source Location" tab, add folder, and also point it to tinyusb/src
- Add a tusb_config.h to your "Core/Inc"
```
### FPGA
- [なひビバ](https://github.com/tokuden/NahiViva)で次のどちらかのプロジェクトを再生してビルドする
    - QMT100/
    - QMT200/