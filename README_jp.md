M5StackToio
===============
[English](README.md)

本ライブラリはオリジナルの https://github.com/futomi/M5StackToio で、それにToio IDの読み取り機能を追加したバージョンの https://github.com/mhama/M5StackToio をベースにBLEプロトコルバージョンv2.3.0以降対応を追加したものです。

M5StackToio はソニー・インタラクティブエンタテインメント社の「[toio コア キューブ](https://toio.io/)」を操作するための [M5Stack](https://m5stack.com/) 用 Arduino ライブラリです。

---------------------------------------
## 目次

* [1. 開発環境セットアップ](#Setup-IDE)
* [2. M5StackToio のインストール](#Install-M5StackToio)
  * [手動でライブラリファイルをインストールする場合](Install-M5StackToio_manually)
* [3. 使い方](#Usage)
* [4. `Toio` オブジェクト](#Toio-object)
  * [`scan()` メソッド (toio コア キューブ発見)](#Toio-scan-method)
  * [`loop()` メソッド (イベント処理)](#Toio-loop-method)
* [5. `ToioCore` オブジェクト](#ToioCore-object)
  * [`getAddress()` メソッド (アドレス取得)](#ToioCore-getAddress-method)
  * [`getName()` メソッド (デバイス名取得)](#ToioCore-getName-method)
  * [`connect()` メソッド (BLE 接続)](#ToioCore-connect-method)
  * [`disconnect()` メソッド (BLE 切断)](#ToioCore-disconnect-method)
  * [`isConnected()` メソッド (接続状態取得)](#ToioCore-isConnected-method)
  * [`onConnection()` メソッド (接続状態イベントのコールバックをセット)](#ToioCore-onConnection-method)
  * [`getBleProtocolVersion()` メソッド (BLE プロトコルバージョン取得)](#ToioCore-getBleProtocolVersion-method)
  * [`playSoundEffect()` メソッド (効果音再生)](#ToioCore-playSoundEffect-method)
  * [`playSoundRaw()` メソッド (サウンド再生開始)](#ToioCore-playSoundRaw-method)
  * [`stopSound()` メソッド (サウンド再生停止)](#ToioCore-stopSound-method)
  * [`turnOnLed()` メソッド (LED 点灯)](#ToioCore-turnOnLed-method)
  * [`turnOffLed()` メソッド (LED 消灯)](#ToioCore-turnOffLed-method)
  * [`getBatteryLevel()` メソッド (バッテリーレベルを取得)](#ToioCore-getBatteryLevel-method)
  * [`onBattery()` メソッド (バッテリーイベントのコールバックをセット)](#ToioCore-onBattery-method)
  * [`getButtonState()` メソッド (ボタンの状態を取得)](#ToioCore-getButtonState-method)
  * [`onButton()` メソッド (ボタンイベントのコールバックをセット)](#ToioCore-onButton-method)
  * [`getMotion()` メソッド (モーションセンサーの状態を取得)](#ToioCore-getMotion-method)
  * [`onMotion()` メソッド (モーションセンサーのコールバックをセット)](#ToioCore-onMotion-method)
  * [`setFlatThreshold()` メソッド(水平検出のしきい値設定)](#ToioCore-setFlatThreshold-method)
  * [`setClashThreshold()` メソッド(衝突検出のしきい値設定)](#ToioCore-setClashThreshold-method)
  * [`setDtapThreshold()` メソッド(ダブルタップ検出の時間間隔の設定)](#ToioCore-setDtapThreshold-method)
  * [`setIDnotificationSettings()` メソッド(読み取りセンサーの ID 通知頻度設定)](#ToioCore-setIDnotificationSettings-method)
  * [`setIDmissedNotificationSettings()` メソッド(読み取りセンサーの ID missed 通知感度設定)](#ToioCore-setIDmissedNotificationSettings-method)
  * [`setMagneticSensorSettings()` メソッド(磁気センサーの設定)](#ToioCore-setMagneticSensorSettings-method)
  * [`setMotorSpeedInformationAcquistionSettings()` メソッド(モーターの速度情報の取得の設定)](#ToioCore-setMotorSpeedInformationAxquistionSettings-method)
  * [`setPostureAngleDetectionSettings()` メソッド(姿勢角検出の設定)](#ToioCore-setPostureAngleDetectionSettings-method)
  * [`setSerializedInformationSettings()` メソッド(シリアライズ情報の通知設定)](#ToioCore-setSerializedInformationSettings-method)
  * [`onConfiguration()` メソッド(設定の応答コールバックをセット)](#ToioCore-onConfiguration-method)
  * [`setConnectionInterval()` メソッド(コネクションインターバル値の変更)](#ToioCore-setConnectionInterval-method)
  * [`getRequestedConnectionInterval()` メソッド(コネクションインターバル要求値の取得)](#ToioCore-getRequestedConnectionInterval-method)
  * [`getAcctualConnectionInterval()` メソッド(現在のコネクションインターバル値の取得)](#ToioCore-getAcctualConnectionInterval-method)
  * [`getConfigurationResponse()` メソッド(設定変更の応答を取得)](#ToioCore-getConfigurationResponse-method)
  * [`controlMotor()` メソッド (モーター制御)](#ToioCore-controlMotor-method)
  * [`drive()` メソッド (運転)](#ToioCore-drive-method)
  * [`controlMotorWithTarget()` メソッド (目標指定付きモーター制御 (目標１つ))](#ToioCore-controlMotorWithTarget-method)
  * [`controlMotorWithMultipleTargets()` メソッド (目標指定付きモーター制御 (目標複数))](#ToioCore-controlMotorWithMultipleTargets-method)
  * [` controlMotorWithAcceleration()` メソッド (加速度指定モーター制御)](#ToioCore-controlMotorWithAcceleration-method)
  * [`onMotor()` メソッド (モーター制御のコールバックをセット)](#ToioCore-onMotor-method)
  * [`getIDReaderData()` メソッド (ID読み取りセンサーの状態、マット上の位置情報を取得)](#ToioCore-getIDReaderData-method)
  * [`onIDReaderData()` メソッド (ID読み取りセンサーのコールバックをセット)](#ToioCore-onIDReaderData-method)

* [6. サンプルスケッチ](#Sample-Sketches)
* [リリースノート](#Release-Note)
* [リファレンス](#References)
* [ライセンス](#License)

---------------------------------------
## <a id="Setup-IDE">1. 開発環境セットアップ</a>

本ライブラリーを利用するにあたり M5Stack 開発環境が必要です。M5Stack の公式サイト (英語) に記載された手順に従って開発環境をセットアップしてください。

* [M5Stack Docs - Arduino IDE Development](https://docs.m5stack.com/#/en/arduino/arduino_development)

USB ドライバーのインストール、Arduino-IDE のインストール、Boards Manager の設定、ボードのインストール、ボードの選択、M5Stack Library のインストールが必要となります。

---------------------------------------
## <a id="Install-M5StackToio">2. M5StackToio のインストール</a>

v1.0.3から https://github.com/arduino/library-registry に登録しましたので、ライブラリファイルを所定の位置に手動で配置しなくてもArduino IDEからはライブラリマネージャでM5StackToioを検索してインストールすることができます。
PlatformIOではplatformio.iniのenv:セクションのlib_depsオプションのところにkenichi884/M5StackToioを足してください

ヒープメモリ消費を抑えるため、v1.0.1からBLEスタックをデフォルトのもの(Bluedroid)から[NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)に変更しています。
(37KBほど違いがあります。WiFiと同時に使う場合はNimBLEでないとヒープが足りず使えません。また、nimbleconfig.hの設定でさらにメモリを空けることもできます。)

[NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)ライブラリをインストールしてください。
Arduino IDEではライブラリマネージャでNimBLE-Arduinoを検索してインストールしてください。
PlatformIOではplatformio.iniのenv:セクションのlib_depsオプションのところに以下の２行を足してください。
```
kenichi884/M5StackToio
h2zero/NimBLE-Arduino@^1.4.1
```

NimBLEのデフォルト設定では接続できる最大数が3に設定されています。
4つ以上のtoio core cubeを接続する場合はnimconfig.hの#define CONFIG_BT_NIMBLE_MAX_CONNECTIONSの値を変更してください。
```
/** @brief Un-comment to change the number of simultaneous connections (esp controller max is 9) */
#define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 3
```

元のBLEスタックを使いたい場合はToioCore.hの #define USE_NIMBLE 1 を無効にしてください。
```
#define USE_NIMBLE 1
を以下のように変更
// #define USE_NIMBLE 1
```

## <a id="Install-M5StackToio_manually">2.1 手動でライブラリファイルをインストールする場合</a>
本ページの右上の `Code` ボタンをクリックし `Download ZIP` を選択し、本ライブラリの zip ファイルをダウンロードしてください。

Arduino IDE を起動し、メニューバーの `スケッチ` -> `ライブラリをインクルード` -> `.ZIP形式のライブラリをインストール...` を選択してください。先ほどダウンロードした本ライブラリの zip ファイルを選択すると、本ライブラリのインストールが完了します。

もし git コマンドが利用できるのであれば、次のようにインストールすることも可能です。以下は Windows10 の PowerShell を使った場合の例です。

```
cd ~
cd Documents\Arduino\libraries
git clone https://github.com/kenichi884/M5StackToio.git
```

PlatformIOの場合は、プロジェクトフォルダのlibの下にToioフォルダを作ってその中にToio.h、Toio.cpp、ToioCore.h、ToioCore.cppを置いてください。


---------------------------------------
## <a id="Usage">3. 使い方</a>

以下のサンプルコードは、toio コア キューブをスキャンし、最初に発見した toio コア キューブに接続し、3 秒後に切断します。

```c++
#include <M5Unified.h>
#include <Toio.h>

// Toio オブジェクト生成
Toio toio;

void setup() {
  // M5Stack の初期化
  M5.begin();
  M5.Power.begin();
  M5.Lcd.clear();

  M5.Log.println("- toio コア キューブをスキャンします。");
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  size_t n = toiocore_list.size();
  if (n == 0) {
    M5.Log.println("- toio コア キューブが見つかりませんでした。");
    return;
  }
  M5.Log.printf("- %d 個の toio コア キューブが見つかりました。\n", n);

  M5.Log.println("- toio コア キューブに BLE 接続します。");
  ToioCore* toiocore = toiocore_list.at(0);
  bool connected = toiocore->connect();
  if (!connected) {
    M5.Log.println("- BLE 接続に失敗しました。");
    return;
  }
  M5.Log.println("- BLE 接続に成功しました。");
  M5.Log.println("- 3 秒後に切断します。");
  delay(3000);
  toiocore->disconnect();
  M5.Log.println("- BLE コネクションを切断しました。");
}

void loop() {

}
```

上記サンプルコードを実行する前に、toio コア キューブの電源を入れてください。実行に成功すれば、Arduino IDE のシリアルモニタに次の結果が表示されます。

```
- toio コア キューブをスキャンします。
- 1 個の toio コア キューブが見つかりました。
- toio コア キューブに BLE 接続します。
- BLE 接続に成功しました。
- 3 秒後に切断します。
- BLE コネクションを切断しました。
```

上記のサンプルコードを以下に解説します。

### ライブラリのインクルード

以下の通り、`.ino` ファイルの先頭で、`M5Stack.h` に加え、本ライブラリのヘッダファイル `Toio.h` をインクルードしてください。

```c++
#include <M5Unified.h>
#include <Toio.h>
```

次に、グローバル変数として、`Toio` オブジェクトを生成します。以下は変数 `toio` に `Toio` オブジェクトを格納しています。

```c++
Toio toio;
```

### toio コア キューブの発見

以降は `.ino` ファイルの `setup()` 関数の中で実行します。

toio コア キューブを発見するには、`Toio` オブジェクトの `scan()` メソッドを実行します。`scan()` メソッドにはスキャン秒数を指定します。指定しなければ 3 秒間スキャンを行います。

```c++
std::vector<ToioCore*> toiocore_list = toio.scan(3);
```

`scan()` メソッドは、指定秒数だけスキャンを実行してから、発見された toio コア キューブのリストを返します。リストの型は `std::vector` です。個々の toio コア キューブを表すオブジェクトの型は `ToioCore` オブジェクトのポインタです。この `ToioCore` オブジェクトを使って、該当の toio コア キューブを操作します。

次の処理に移る前に、発見された toio コア キューブの数をチェックします。

```c++
size_t n = toiocore_list.size();
if(n == 0) {
  M5.Log.println("toio コア キューブが見つかりませんでした。");
  return;
}
ToioCore* toiocore = toiocore_list.at(0);
```

上記コードでは、最初に見つかった toio コア キューブを表す `ToioCore` オブジェクトのポインタを、変数 `toiocore` に格納しています。

### toio コア キューブに BLE 接続

発見した toio コア キューブに BLE 接続するには、該当の toio コア キューブを表す `ToioCore` オブジェクトのポインタから `connect()` メソッドを呼び出します。

```c++
bool connected = toiocore->connect();
if(!connected) {
  M5.Log.println("BLE 接続に失敗しました。");
  return;
}
```

`connect()` メソッドは BLE 接続が完了すると `true` を返します。この戻り値をチェックしてください。

toio コア キューブに BLE 接続できたら、以降で解説する `toioCore` オブジェクトのメソッドをポインタ経由で呼び出して toio コア キューブを操作します。

### toio コア キューブを切断

BLE 接続した toio コア キューブを切断するには、`ToioCore` オブジェクトのポインタから `disconnect()` メソッドを呼び出します。

```c++
toiocore->disconnect();
```

---------------------------------------
## <a id="Toio-object">4. `Toio` オブジェクト</a>

`Toio` オブジェクトは次のように生成します。以下のコードでは、変数 `toio` に `Toio` オブジェクトが格納されます。

```c++
Toio toio;
```

以降、変数 `toio` に `Toio` オブジェクトが格納されているとして解説します。


### <a id="Toio-scan-method">✔ `scan()` メソッド (toio コア キューブ発見)</a>

`scan()` メソッドは指定秒数だけ toio コア キューブをスキャンします。

#### プロトタイプ宣言

```c++
std::vector<ToioCore*> scan(uint8_t duration);
```

#### 引数

No. |  変数名     | 型        | 必須   | 説明
:---|:-----------|:----------|:-------|:-------------
1   | `duration` | `uint8_t` | &nbsp; | スキャン秒数 (デフォルト値: 3)

#### コードサンプル

```c++
std::vector<ToioCore*> toiocore_list = toio.scan(3);
```

### <a id="Toio-loop-method">✔ `loop()` メソッド (イベント処理)</a>

`loop()` メソッドはイベント処理を実行します。後述のイベントハンドラ設定関数を使う場合は、`.ino` ファイルの `loop()` メソッド内で必ず呼び出してください。

#### プロトタイプ宣言

```c++
void loop();
```

#### 引数

なし

#### コードサンプル

```c++
void loop() {
  M5.update();
  toio.loop();
  ...
}
```

---------------------------------------
## <a id="ToioCore-object">5. `ToioCore` オブジェクト</a>

前述の通り、発見した toio コア キューブを操作するには、`toioCore` オブジェクトのメソッドをポインタ経由で呼び出します。以下、`toioCore` オブジェクトに実装されたメソッドを説明します。以降、変数 `toiocore` が `toioCore` オブジェクトのポインタとします。

### <a id="ToioCore-getAddress-method">✔ `getAddress()` メソッド (アドレス取得)</a>

toio コア キューブの MAC アドレスを返します。

#### プロトタイプ宣言

```c++
std::string getAddress();
```

#### 引数

なし

#### コードサンプル

```c++
std::string mac = toiocore->getAddress();
M5.Log.println(mac.c_str()); // 例 "d1:52:fa:d2:c6:a1"
```

### <a id="ToioCore-getName-method">✔ `getName()` メソッド (デバイス名取得)</a>

toio コア キューブのデバイス名を返します。デバイス名は、toio コア キューブがアドバタイジングパケットにセットした `localName` の値です。通常は `"toio Core Cube-XXX"` です。XXX にはキューブごとに固有の ID 文字列が入ります。

#### プロトタイプ宣言

```c++
std::string getName();
```

#### 引数

なし

#### コードサンプル

```c++
std::string name = toiocore->getName();
M5.Log.println(name.c_str()); // 例 "toio Core Cube-a9R"
```

### <a id="ToioCore-connect-method">✔ `connect()` メソッド (BLE 接続)</a>

toio コア キューブに BLE 接続を行います。

#### プロトタイプ宣言

```c++
bool connect();
```

#### 引数

なし

#### コードサンプル

```c++
bool connected = toiocore->connect();
if (!connected) {
  M5.Log.println("- BLE 接続に失敗しました。");
  return;
}
```

### <a id="ToioCore-disconnect-method">✔ `disconnect()` メソッド (BLE 切断)</a>

toio コア キューブとの BLE コネクションを切断します。

#### プロトタイプ宣言

```c++
void disconnect();
```

#### 引数

なし

#### コードサンプル

```c++
toiocore->disconnect();
```

### <a id="ToioCore-isConnected-method">✔ `isConnected()` メソッド (接続状態取得)</a>

toio コア キューブとの BLE 接続状態を返します。接続中であれば `true` を、切断中であれば `false` を返します。

#### プロトタイプ宣言

```c++
bool isConnected();
```

#### 引数

なし

#### コードサンプル

```c++
if(toiocore->isConnected()) {
  M5.Log.println("接続中");
} else {
  M5.Log.println("切断中");
}
```

### <a id="ToioCore-onConnection-method">✔ `onConnection()` メソッド (接続状態イベントのコールバックをセット)</a>

toio コア キューブとの BLE 接続状態イベントのコールバックをセットします。BLE 接続状態に変化があれば、引数に指定したコールバック関数を呼び出します。コールバック関数には接続状態を表す bool 値が引き渡されます。

#### プロトタイプ宣言

```c++
typedef std::function<void(bool connected)> OnConnectionCallback;
void onConnection(OnConnectionCallback cb);
```

#### 引数

No. | 変数名   | 型                     | 必須   | 説明
:---|:--------|:-----------------------|:-------|:-------------
1   | `cb`    | `OnConnectionCallback` | ✔     | コールバック関数

#### コードサンプル

以下のサンプルスケッチは、10 秒おきに BLE 接続と切断を繰り返します。コールバック関数により、接続状態のイベントが発生すると、その状態を出力します。

コールバックを使う場合は、`.ino` ファイルの `loop()` 関数内で `Toio` オブジェクトの [`loop()`](#Toio-loop-method) メソッドを呼び出してください。コールバックは、`.ino` ファイルの `loop()` 関数が実行が開始されてから発生したイベントしかハンドリングできませんので注意してください。そのため、以下のサンプルスケッチでは、`loop()` 関数内で BLE 接続と切断を行っています。

```c++
#include <M5Unified.h>
#include <Toio.h>

// Toio オブジェクト生成
Toio toio;

// ToioCore オブジェクトのポインタ
ToioCore* toiocore = nullptr;

// BLE 接続・切断のタイムスタンプ
unsigned long conn_time = millis();

void setup() {
  // M5Stack の初期化
  M5.begin();
  M5.Power.begin();

  // toio コア キューブのスキャン開始
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() > 0) {
    toiocore = toiocore_list.at(0);
  }

  // BLE 接続状態イベントのコールバックをセット
  toiocore->onConnection([](bool state) {
    M5.Log.println(state ? "接続" : "切断");
  });
}

void loop() {
  // コールバックを使う場合には必ず Toio オブジェクトの loop() を呼び出す
  toio.loop();

  if (toiocore) {
    // 10 秒おきに BLE 接続・切断を繰り返す
    unsigned long now = millis();
    if (now - conn_time >= 10000) {
      if (toiocore->isConnected()) {
        toiocore->disconnect();
      } else {
        toiocore->connect();
      }
      conn_time = now;
    }
  }
}
```

### <a id="ToioCore-getBleProtocolVersion-method">✔ `getBleProtocolVersion()` メソッド (BLE プロトコルバージョン取得)</a>

toio コア キューブの BLE プロトコルバージョンを取得します。

#### プロトタイプ宣言

```c++
std::string getBleProtocolVersion();
```

#### 引数

なし

#### コードサンプル

```c++
std::string ble_ver = toiocore->getBleProtocolVersion();
M5.Log.println(ble_ver.c_str()); // 例 "2.4.0"
```

### <a id="ToioCore-playSoundEffect-method">✔ `playSoundEffect()` メソッド (効果音再生)</a>

toio コア キューブにプリセットされた効果音を再生します。

#### プロトタイプ宣言

```c++
void playSoundEffect(uint8_t sound_id, uint8_t volume = 0xff);
```

#### 引数

No. | 変数名      | 型        | 必須   | 説明
:---|:-----------|:----------|:-------|:-------------
1   | `sound_id` | `uint8_t` | ✔     | 効果音の ID (0 ～ 10)
2   | `volume`   | `uint8_t` | &nbsp; | 音量 (0 ～ 255)。指定が無ければ 255。

効果音の ID の意味は以下の通りです。

効果音 ID | 効果音名称
:---------|:---------------
`0`       | Enter
`1`       | Selected
`2`       | Cancel
`3`       | Cursor
`4`       | Mat in
`5`       | Mat out
`6`       | Get 1
`7`       | Get 2
`8`       | Get 3
`9`       | Effect 1
`10`      | Effect 2

#### コードサンプル

```c++
toiocore->playSoundEffect(3);
```

### <a id="ToioCore-playSoundRaw-method">✔ `playSoundRaw()` メソッド (サウンド再生開始)</a>

toio コア キューブの通信仕様で定義された生データを、サウンドの Characteristic に書き込みます。主に MIDI データを再生するために使います。詳細は [toio コア キューブのサウンドの仕様](https://toio.github.io/toio-spec/docs/ble_sound)を参照してください。

#### プロトタイプ宣言

```c++
void playSoundRaw(uint8_t* data, size_t length);
```

#### 引数

No. | 変数名    | 型         | 必須   | 説明
:---|:---------|:-----------|:-------|:-------------
1   | `data`   | `uint8_t*` | ✔     | 書き込むデータのバイト列
2   | `length` | `size_t`   | ✔     | 書き込むバイト数

#### コードサンプル

以下のサンプルコードは、[チャルメラ](https://www.myojofoods.co.jp/charumera/play/index.html)を最大音量で再生します。 

```c++
uint8_t charumera_data[39] = {
  3,             // 制御の種類 (MIDI なら `3`)
  1,             // 繰り返し回数
  12,            // Operation の数
  14,  69, 255,  // 140ms, A5
  14,  71, 255,  // 140ms, B5
  56,  73, 255,  // 560ms, C#6
  14,  71, 255,  // 140ms, B5
  14,  69, 255,  // 140ms, A5
  114, 128, 255, // 無音
  14,  69, 255,  // 140ms, A5
  14,  71, 255,  // 140ms, B5
  14,  73, 255,  // 560ms, C#6
  14,  71, 255,  // 140ms, B5
  14,  69, 255,  // 140ms, A5
  56,  71, 255   // 560ms, B5
};
toiocore->playSoundRaw(charumera_data, 39);
```

### <a id="ToioCore-stopSound-method">✔ `stopSound()` メソッド (サウンド再生停止)</a>

サウンド再生を停止します。

#### プロトタイプ宣言

```c++
void stopSound();
```

#### 引数

なし

#### コードサンプル

```c++
toiocore->stopSound();
```

### <a id="ToioCore-turnOnLed-method">✔ `turnOnLed()` メソッド (LED 点灯)</a>

toio コア キューブの LED を点灯します。

#### プロトタイプ宣言

```c++
void turnOnLed(uint8_t r, uint8_t g, uint8_t b);
```

#### 引数

No. | 変数名 | 型        | 必須   | 説明
:---|:------|:----------|:-------|:-------------
1   | `r`   | `uint8_t` | ✔     | 赤コンポーネント (`0` ～ `255`)
2   | `g`   | `uint8_t` | ✔     | 緑コンポーネント (`0` ～ `255`)
2   | `b`   | `uint8_t` | ✔     | 青コンポーネント (`0` ～ `255`)

#### コードサンプル

```c++
toiocore->turnOnLed(0xff, 0xff, 0x00); // 黄色で点灯
```

※ 黒 (`0x00, 0x00, 0x00`) を指定すると消灯になります。

### <a id="ToioCore-turnOffLed-method">✔ `turnOffLed()` メソッド (LED 消灯)</a>

toio コア キューブの LED を消灯します。

#### プロトタイプ宣言

```c++
void turnOffLed();
```

#### 引数

なし

#### コードサンプル

```c++
toiocore->turnOffLed();
```

### <a id="ToioCore-getBatteryLevel-method">✔ `getBatteryLevel()` メソッド (バッテリーレベルを取得)</a>

toio コア キューブのバッテリーレベル (%) を取得します。

#### プロトタイプ宣言

```c++
uint8_t getBatteryLevel();
```

#### 引数

なし

#### コードサンプル

```c++
uint8_t batt_level = toiocore->getBatteryLevel();
M5.Log.printf("%d パーセント\n", batt_level);
```

### <a id="ToioCore-onBattery-method">✔ `onBattery()` メソッド (バッテリーイベントのコールバックをセット)</a>

toio コア キューブのバッテリーイベントのコールバックをセットします。5 秒間隔で引数に指定したコールバック関数を呼び出します。コールバック関数にはバッテリーレベル (パーセント) が引き渡されます。

#### プロトタイプ宣言

```c++
typedef std::function<void(uint8_t level)> OnBatteryCallback;
void onBattery(OnBatteryCallback cb);
```

#### 引数

No. | 変数名   | 型                  | 必須   | 説明
:---|:--------|:--------------------|:-------|:-------------
1   | `cb`    | `OnBatteryCallback` | ✔     | コールバック関数

#### コードサンプル

以下のサンプルスケッチは、バッテリーイベントを受信する都度、バッテリーレベルを出力します。

コールバックを使う場合は、`.ino` ファイルの `loop()` 関数内で `Toio` オブジェクトの [`loop()`](#Toio-loop-method) メソッドを呼び出してください。コールバックは、`.ino` ファイルの `loop()` 関数が実行が開始されてから発生したイベントしかハンドリングできませんので注意してください。

```c++
#include <M5Unified.h>
#include <Toio.h>

// Toio オブジェクト生成
Toio toio;

void setup() {
  // M5Stack の初期化
  M5.begin();
  M5.Power.begin();

  // toio コア キューブのスキャン開始
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // BLE 接続
  toiocore->connect();

  // バッテリーイベントのコールバックをセット
  toiocore->onBattery([](uint8_t level) {
    M5.Log.printf("%d パーセント\n", level);
  });
}

void loop() {
  // コールバックを使う場合には必ず Toio オブジェクトの loop() を呼び出す
  toio.loop();
}
```

### <a id="ToioCore-getButtonState-method">✔ `getButtonState()` メソッド (ボタンの状態を取得)</a>

toio コア キューブ裏面のボタン (LED と同じ) の押下状態を取得します。押した状態なら `true` を、そうでなければ `false` を返します。

#### プロトタイプ宣言

```c++
bool getButtonState();
```

#### 引数

なし

#### コードサンプル

```c++
if(toiocore->getButtonState()) {
  M5.Log.println("ボタンが押されています。");
} else {
  M5.Log.println("ボタンは押されていません。");
}
```

### <a id="ToioCore-onButton-method">✔ `onButton()` メソッド (ボタンイベントのコールバックをセット)</a>

toio コア キューブのボタン押下イベントのコールバックをセットします。ボタン押下状態に変化があれば、引数に指定したコールバック関数を呼び出します。コールバック関数にはボタン押下状態を表す bool 値が引き渡されます。

#### プロトタイプ宣言

```c++
typedef std::function<void(bool state)> OnButtonCallback;
void onButton(OnButtonCallback cb);
```

#### 引数

No. | 変数名   | 型                 | 必須   | 説明
:---|:--------|:-------------------|:-------|:-------------
1   | `cb`    | `OnButtonCallback` | ✔     | コールバック関数

#### コードサンプル

以下のサンプルスケッチは、ボタン押下状態に変化があると、その都度、その状態を出力します。

コールバックを使う場合は、`.ino` ファイルの `loop()` 関数内で `Toio` オブジェクトの [`loop()`](#Toio-loop-method) メソッドを呼び出してください。コールバックは、`.ino` ファイルの `loop()` 関数が実行が開始されてから発生したイベントしかハンドリングできませんので注意してください。

```c++
#include <M5Unified.h>
#include <Toio.h>

// Toio オブジェクト生成
Toio toio;

void setup() {
  // M5Stack の初期化
  M5.begin();
  M5.Power.begin();

  // toio コア キューブのスキャン開始
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // BLE 接続
  toiocore->connect();

  // ボタン押下状態イベントのコールバックをセット
  toiocore->onButton([](bool state) {
    M5.Log.println(state ? "ボタン押下" : "ボタン解除");
  });
}

void loop() {
  // コールバックを使う場合には必ず Toio オブジェクトの loop() を呼び出す
  toio.loop();
}
```

### <a id="ToioCore-getMotion-method">✔ `getMotion()` メソッド (モーションセンサーの状態を取得)</a>

toio コア キューブのモーションセンサーの状態を取得します。

#### プロトタイプ宣言

```c++
struct ToioCoreMotionData {
  bool flat;
  bool clash;
  bool dtap;
  uint8_t attitude;
  uint8_t shake;
};

ToioCoreMotionData getMotion();
```

#### 引数

なし

#### 戻値

`ToioCoreMotionData` 型の構造体が返されます。各メンバーの意味は以下の通りです。

名前       | 型        | 説明
:----------|:----------|:------------------------
`flat`     | `bool`    | 水平検出 (`true`: 水平, `false`: 水平でない) 
`clash`    | `bool`    | 衝突検出 (`true`: あり, `false`: なし)
`dtap`     | `bool`    | ダブルタップ検出 (`true`: あり, `false`: なし)
`attitude` | `uint8_t` | 姿勢検出 (後述)
`shake`    | `uint8_t` | シェイク検出 (0x00 検出なし, 0x01 ～ 0x0a 振った強さ)

姿勢検出 `attitude` が取る値とその意味は以下の通りです。

値  | toio コア キューブの姿勢
:---|:-----------------------
`1` | 天面が上
`2` | 底面が上
`3` | 背面が上
`4` | 正面が上
`5` | 右側面が上
`6` | 左側面が上

姿勢検出の詳細は [toio コア キューブ技術仕様](https://toio.github.io/toio-spec/docs/ble_sensor)をご覧ください。

#### コードサンプル

```c++
ToioCoreMotionData motion = toiocore->getMotion();
M5.Log.printf("- 水平検出: %s\n", motion.flat ? "水平" : "水平でない");
M5.Log.printf("- 衝突検出: %s\n", motion.clash ? "あり" : "なし");
M5.Log.printf("- ダブルタップ検出: %s\n", motion.dtap ? "あり" : "なし");
M5.Log.printf("- 姿勢検出: %d\n",  motion.attitude);
M5.Log.printf("- シェイク検出: %d\n",  motion.shake);
```

### <a id="ToioCore-onMotion-method">✔ `onMotion()` メソッド (モーションセンサーのコールバックをセット)</a>

toio コア キューブのモーションイベントのコールバックをセットします。モーションに変化があれば、引数に指定したコールバック関数を呼び出します。コールバック関数にはモーションを表す構造体が引き渡されます。
第２引数に磁気センサーイベントのコールバックを、第３引数に姿勢角イベントのコールバックをセットすることもできます。
コールバック関数にはそれぞれ磁気センサからの情報、姿勢角情報を表す構造体が引き渡されます。
磁気センサーの情報や姿勢角情報の通知はデフォルトで無効になっています。
磁気センサーの情報を得る場合は、[setMagneticSensorSettings()メソッド](#ToioCore-setMagneticSensorSettings-method)、姿勢角情報を得るには[setPostureAngleDetectionSettings()メソッド](#ToioCore-setPostureAngleDetectionSettings-method)を呼び出して通知機能をオンにしておく必要があります。

#### プロトタイプ宣言

```c++

// モーション情報
struct ToioCoreMotionData {
  bool flat;
  bool clash;
  bool dtap;
  uint8_t attitude;
  uint8_t shake;
};

// 姿勢角情報
struct ToioCorePostureAngleEuler {
  int16_t roll;
  int16_t pitch;
  int16_t yaw;
};
struct ToioCorePostureAngleQuaternion {
  float_t w;
  float_t x;
  float_t y;
  float_t z;
};
struct ToioCorePostureAngleHighPrecisionEuler {
  float_t roll;
  float_t pitch;
  float_t yaw;
};

struct ToioCorePostureAngle{
  union {
    ToioCorePostureAngleEuler euler;
    ToioCorePostureAngleQuaternion quaternion;
    ToioCorePostureAngleHighPrecisionEuler eulerf;
  };
};

//磁気センサー情報
struct ToioCoreMagneticSensorData {
  uint8_t state;
  uint8_t strength;
  int8_t x;
  int8_t y;
  int8_t z;
};

typedef std::function<void(ToioCoreMotionData motion)> OnMotionCallback;
typedef std::function<void(ToioCoreMagneticSensorData magnetic_sensor)> OnMagneticSensorCallback;
typedef std::function<void(ToioCorePostureAngle angle)> OnPostureAngleCallback;
void onMotion(OnMotionCallback cb, OnMagneticSensorCallback mag_cb = nullptr, OnPostureAngleCallback angle_cb = nullptr);
```

#### 引数

No. | 変数名   | 型                 | 必須   | 説明
:---|:--------|:-------------------|:-------|:-------------
1   | `cb`    | `OnMotionCallback` | ✔     | モーション情報のコールバック関数
2   | `mag_cb`    | `OnMagneticSensorCallback` |      | 磁気センサー情報のコールバック関数
3   | `angle_cb`    | `OnPostureAngleCallback` |      | 姿勢角情報のコールバック関数

#### コードサンプル

以下のサンプルスケッチは、モーションに変化があると、その都度、その状態を出力します。

コールバックを使う場合は、`.ino` ファイルの `loop()` 関数内で `Toio` オブジェクトの [`loop()`](#Toio-loop-method) メソッドを呼び出してください。コールバックは、`.ino` ファイルの `loop()` 関数が実行が開始されてから発生したイベントしかハンドリングできませんので注意してください。

```c++
#include <M5Unified.h>
#include <Toio.h>

// Toio オブジェクト生成
Toio toio;

void setup() {
  // M5Stack の初期化
  M5.begin();
  M5.Power.begin();

  // toio コア キューブのスキャン開始
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // BLE 接続
  toiocore->connect();

  // モーションイベントのコールバックをセット
  toiocore->onMotion([](ToioCoreMotionData motion) {
    M5.Log.println("------------------------------------------");
    M5.Log.printf("- 水平検出: %s\n", motion.flat ? "水平" : "水平でない");
    M5.Log.printf("- 衝突検出: %s\n", motion.clash ? "あり" : "なし");
    M5.Log.printf("- ダブルタップ検出: %s\n", motion.dtap ? "あり" : "なし");
    M5.Log.printf("- 姿勢検出: %d\n",  motion.attitude);
  });
}

void loop() {
  // コールバックを使う場合には必ず Toio オブジェクトの loop() を呼び出す
  toio.loop();
}
```

以下のサンプルスケッチは、磁気センサー、姿勢角に変化があると、その都度、その状態を出力します。

```c++
#include <M5Unified.h>
#include <Toio.h>

// Toio オブジェクト生成
Toio toio;

void setup() {
  // M5Stack の初期化
  M5.begin();
  M5.Power.begin();

  // toio コア キューブのスキャン開始
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // BLE 接続
  toiocore->connect();

  // 磁力の強さを通知する設定
  toiocore->setMagneticSensorSettings(1, NotifyChangesOnly, EnableMagneticForce); 
  // 姿勢をオイラー角(int16)で通知する設定
  toiocore->setPostureAngleDetectionSettings(1, NotifyChangesOnly, AngleTypeEuller); 

  // モーションイベントは使わないのでnullptrをセット、磁気センサーのコールバックをセット、姿勢角のコールバックをセット
  toiocore->onMotion(
    nullptr,
    [](ToioCoreMagneticSensorData mag_sensor){
      M5.Log.printf("Magnetic Sensor Event state=%u, strength=%u, x=%d, y=%d, z=%d\n",
      mag_sensor.state, mag_sensor.strength, mag_sensor.x, mag_sensor.y, mag_sensor.z);
    },
    [](ToioCorePostureAngle angle){
      M5.Log.printf("Posture Angle Euler(int16) Event roll=%d, pitch=%d, yaw=%d\n",
      angle.euler.roll, angle.euler.pitch, angle.euler.yaw);
    });
}

void loop() {
  // コールバックを使う場合には必ず Toio オブジェクトの loop() を呼び出す
  toio.loop();
}
```

### <a id="ToioCore-getBleProtocolVersion-method">✔ `getBleProtocolVersion()` メソッド (BLE プロトコルバージョン取得)</a>

toio コア キューブのBLEプロトコルバージョンを取得します。
connect()している状態で実行してください。

#### プロトタイプ宣言

```c++
std::string getBleProtocolVersion();
```

#### 引数

なし

#### コードサンプル

```c++
  // Toio Core のBLEプロトコルバージョンを表示
  M5.Log.println(protoclver.c_str()); //  例 "v2.4.0"
```

### <a id="ToioCore-setFlatThreshold-method">✔ `setFlatThreshold()` メソッド (水平検出のしきい値設定)</a>

toio コア キューブの水平検出のしきい値を設定します。

#### プロトタイプ宣言

```c++
 void setFlatThreshold(uint8_t deg = 45);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `deg`     | `uint8_t`     |      | 水平検出のしきい値を度で設定(1～45)

水平検出のしきい値の意味については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E6%B0%B4%E5%B9%B3%E6%A4%9C%E5%87%BA%E3%81%AE%E3%81%97%E3%81%8D%E3%81%84%E5%80%A4%E8%A8%AD%E5%AE%9A)をご覧ください。


### <a id="ToioCore-setClashThreshold-method">✔ `setClashThreshold()` メソッド (衝突検出のしきい値設定)</a>

toio コア キューブの衝突検出のしきい値を設定します。

#### プロトタイプ宣言

```c++
 void setClashThreshold(uint8_t level = 7);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `level`     | `uint8_t`     |      | 衝突検出のしきい値設定(弱1～10強)

衝突検出検出のしきい値の意味については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E8%A1%9D%E7%AA%81%E6%A4%9C%E5%87%BA%E3%81%AE%E3%81%97%E3%81%8D%E3%81%84%E5%80%A4%E8%A8%AD%E5%AE%9A)をご覧ください。

### <a id="ToioCore-setDtapThreshold-method">✔ `setDtapThreshold()` メソッド (ダブルタップ検出のしきい値設定)</a>

toio コア キューブのダブルタップ検出のしきい値を設定します。

#### プロトタイプ宣言

```c++
 void setDtapThreshold(uint8_t level = 5);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `level`     | `uint8_t`     |      | ダブルタップ検出の時間間隔設定(短0～7長)

ダブルタップ検出検出のしきい値の意味については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E3%83%80%E3%83%96%E3%83%AB%E3%82%BF%E3%83%83%E3%83%97%E6%A4%9C%E5%87%BA%E3%81%AE%E6%99%82%E9%96%93%E9%96%93%E9%9A%94%E3%81%AE%E8%A8%AD%E5%AE%9A)をご覧ください。


### <a id="ToioCore-setIDnotificationSettings-method">✔ `setIDnotificationSettings()` メソッド (読み取りセンサーの ID 通知頻度設定)</a>

toio コア キューブの読み取りセンサーの ID 通知頻度を設定します。

#### プロトタイプ宣言

```c++
 void setIDnotificationSettings(uint8_t minimum_interval, uint8_t condition);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `minimum_interval`     | `uint8_t`     | ✔    | 通知間隔の最小値を設定(0～255、単位は10ms)
2   | `condition`     | `uint8_t`     | ✔     | 通知条件(0x00, 0x01, 0xff)

通知条件値の意味については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E8%AA%AD%E3%81%BF%E5%8F%96%E3%82%8A%E3%82%BB%E3%83%B3%E3%82%B5%E3%83%BC%E3%81%AE-id-%E9%80%9A%E7%9F%A5%E8%A8%AD%E5%AE%9A)をご覧ください。

### <a id="ToioCore-setIDmissedNotificationSettings-method">✔ `setIDmissedNotificationSettings()` メソッド (読み取りセンサーの ID missed 通知感度設定)</a>

toio コア キューブの読み取りセンサーの ID missed 通知感度の設定をします。

#### プロトタイプ宣言

```c++
 void setIDmissedNotificationSettings(uint8_t sensitivity);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `sensitivity`     | `uint8_t`     |  ✔  | キューブがマットやカードから取り除かれてから通知するまでの時間を設定(0～255、単位は10ms)

ID missed 通知感度設定については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E8%AA%AD%E3%81%BF%E5%8F%96%E3%82%8A%E3%82%BB%E3%83%B3%E3%82%B5%E3%83%BC%E3%81%AE-id-missed-%E9%80%9A%E7%9F%A5%E8%A8%AD%E5%AE%9A)をご覧ください。

### <a id="ToioCore-setMagneticSensorSettings-method">✔ `ssetMagneticSensorSettings()` メソッド (磁気センサーの設定)</a>

toio コア キューブの磁気センサーの設定をします。
磁気センサーはデフォルトでオフなので、使用する場合はこのメソッドを呼んで有効化しておく必要があります。

#### プロトタイプ宣言

```c++
 void setMagneticSensorSettings(uint8_t interval, uint8_t condition, uint8_t function = EnableMagneticForce);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `interval`     | `uint8_t`     |  ✔  | 通知間隔を設定(0～255、単位は20ms)
2   | `condition`     | `uint8_t`     |  ✔  | 常に通知 0x00、 変化があったとき通知 0x01
3   | `function`     | `uint8_t`     |    | 0x01 状態変化、0x02 磁力、0x00 センサー無効化

磁気センサー設定については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E7%A3%81%E6%B0%97%E3%82%BB%E3%83%B3%E3%82%B5%E3%83%BC%E3%81%AE%E8%A8%AD%E5%AE%9A)をご覧ください。

### <a id="ToioCore-setMotorSpeedInformationAcquistionSetting-method">✔ `setMotorSpeedInformationAcquistionSettings()` メソッド (モーターの速度情報の取得の設定)</a>

toio コア キューブのモーターの速度情報の取得を設定します。
モーターの速度情報の取得はデフォルトでオフなので、使用する場合はこのメソッドを呼んで有効化しておく必要があります。

#### プロトタイプ宣言

```c++
 void setMotorSpeedInformationAcquistionSettings(bool enable);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `enable`     | `bool`     |  ✔  | モーターの速度情報の取得の true 有効、 false無効

モーターの速度情報の取得設定については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E3%83%A2%E3%83%BC%E3%82%BF%E3%83%BC%E3%81%AE%E9%80%9F%E5%BA%A6%E6%83%85%E5%A0%B1%E3%81%AE%E5%8F%96%E5%BE%97%E3%81%AE%E8%A8%AD%E5%AE%9A)をご覧ください。

### <a id="ToioCore-setPostureAngleDetectionSettings-method">✔ `setPostureAngleDetectionSettings()` メソッド (姿勢角検出の設定)</a>

toio コア キューブの姿勢角検出の設定をします。
姿勢角検出の機能はデフォルトでオフなので、使用する場合はこのメソッドを呼んで有効化しておく必要があります。
また、姿勢角は指定した角度の種類(オイラー角、クォータニオン、高精度オイラー角)で通知されますので、通知の受信側でそれぞれの角度の種類に応じた処理が必要です。

#### プロトタイプ宣言

```c++
 void setPostureAngleDetectionSettings(uint8_t interval, uint8_t condition, uint8_t angle_type = AngleTypeEuller);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `interval`     | `uint8_t`     |  ✔  | 通知間隔を設定(0～255、単位は10ms)
2   | `condition`     | `uint8_t`     |  ✔  | 常に通知 0x00、 変化があったとき通知 0x01
3   | `angle_type`     | `uint8_t`     |    | 角度の種類 0x01 オイラー角(uint16_t)、0x02 クォータニオン(float)、0x03 高精度オイラー角(float)

姿勢角検出の設定については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E5%A7%BF%E5%8B%A2%E8%A7%92%E6%A4%9C%E5%87%BA%E3%81%AE%E8%A8%AD%E5%AE%9A-)をご覧ください。

### <a id="ToioCore-setSerializedInformationSettings-method">✔ `setSerializedInformationSettings()` メソッド (シリアライズ情報の通知設定)</a>

toio コア キューブのシリアライズ情報の通知の設定をします。
シリアライズ情報の通知はデフォルトでオフなので、使用する場合はこのメソッドを呼んで有効化しておく必要があります。

#### プロトタイプ宣言

```c++
 void setSerializedInformationSettings(uint8_t interval, uint8_t condition);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `interval`     | `uint8_t`     |  ✔  | 通知間隔を設定(0～255、単位は10ms)
2   | `condition`     | `uint8_t`     |  ✔  | 常に通知 0x00、 変化があったとき通知 0x01

シリアライズ情報の通知の設定については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E3%82%B7%E3%83%AA%E3%82%A2%E3%83%A9%E3%82%A4%E3%82%BA%E6%83%85%E5%A0%B1%E3%81%AE%E9%80%9A%E7%9F%A5%E8%A8%AD%E5%AE%9A)をご覧ください。

### <a id="ToioCore-onConfiguration-method">✔ `onConfiguration()` メソッド (設定の応答コールバックをセット)</a>

toio コア キューブの設定の応答コールバックをセットします。
設定変更の応答の通知を受信する場合はコールバック関数をセットします。
シリアライズ情報もこの設定変更の応答を使って通知されます。


#### プロトタイプ宣言

```c++
// 設定応答

// 設定変更の応答
struct ToioCoreSetConfigurationResponse {
    uint8_t reserved;
    uint8_t response;
};

// コネクションインターバル値
struct ToioCoreConnectionIntervalSettings {
    uint8_t reserved;
    uint16_t minimum;
    uint16_t maximum;
};

// 設定characteristicsの読み出し/notifyの応答
struct ToioCoreConfigurationResponse {
  uint8_t infoType;
  union {
    ToioCoreSetConfigurationResponse config; // 設定応答 infoType = 0x98 0x99 0x9b 0x9c 0x9d 0x9e 0xb0
    ToioCoreConnectionIntervalSettings interval; // コネクションインターバル値 infoType =  0xb1 or 0xb2
    uint8_t serialized[19]; // シリアライズ情報   info Type = 0xf0
  }; 
};

enum ToioCoreConfigrationType {
  ResponseBLEProtocolVersion = 0x81,
  ResponseIDnotificationSettings = 0x98,
  ResponseIDmissedNotificationSettings = 0x99,
  ResponseMagneticSensorSettings = 0x9b,
  ResponseMotoroSpeedInformationAcquisitionSettings = 0x9c,
  ResponsePostureAngleDetectionSettings = 0x9d,
  ResponseSerializedInformationSettings = 0x9e,
  ResponseChangeConnectionInterval = 0xb0,
  ResponseObtainRequestedConnectionInterval = 0xb1,
  ResponseObtainActualConnectionInterval = 0xb2,
  SerializedData = 0xf0
};
typedef std::function<void(ToioCoreConfigurationResponse configration_response)> OnConfigurationCallback;
void onConfiguration(OnConfigurationCallback cb);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `cb`     | `OnConfigurationCallback`     |  ✔  | コールバック関数

#### コードサンプル

以下のコードは設定変更時の応答通知および、シリアライズ情報の通知を受け取るサンプルコードです。

```c++
  struct SerializedBits {
    unsigned reserved:4;
    unsigned button:1;
    unsigned toioIDstatus:2;
    unsigned toioIDangleLower:1;

    unsigned toioIDangleUpper:8;

    unsigned cubeXLower:8;

    unsigned cubeXUpper:6;
    unsigned cubeYLower:2;

    unsigned cubeYMiddle:8;

    unsigned cubeYUpper:4;
    unsigned standardIDLower:4;

    unsigned standardIDMiddleLow:8;

    unsigned standardIDMiddleHigh:8;

    unsigned standardIDUpper:8;

    unsigned motorSpeedL:8;

    unsigned motorSpeedR:8;

    unsigned moveToTargetsResponse:5;
    unsigned motionAngleDetect:3;

    unsigned battery:7;
    unsigned motionHorizontalDetect:1;

    unsigned motionShakedDetect:5;
    unsigned motionCollisionDetect:1;
    unsigned motionDoubleTapDetect:1;
    unsigned reserved2:1;

    unsigned eulerAngleRoll:8;

    unsigned eulerAnglePitch:8;

    unsigned eulerAngleYaw:8;

    unsigned eulerAngleRollSign:1;
    unsigned eulerAnglePitchSign:1;
    unsigned eulerAngleYawSign:1;
    unsigned reserved3:2;
    unsigned magnetStatus:3;

    unsigned magnetForce:8;
  };

  toiocore->onConfiguration([] (ToioCoreConfigurationResponse resp){
    M5.Log.printf(" config event %02x  ", resp.infoType);
    if((resp.infoType == ResponseObtainRequestedConnectionInterval) ||
      (resp.infoType == ResponseObtainActualConnectionInterval))
        M5.Log.printf("connection interval %02x %u %u", resp.interval.reserved, resp.interval.minimum, resp.interval.maximum);
    else if(resp.infoType == SerializedData) {
      M5.Log.printf("serialized rawdata ");
      for(int i = 0 ; i < 19; i++){
        M5.Log.printf("%02x ", resp.serialized[i]);
      }
      M5.Log.printf("\n");
      SerializedBits bits;
      memcpy(&bits, resp.serialized, 19);
      uint toioIDangle = (bits.toioIDangleUpper << 1) |  bits.toioIDangleLower;
      uint cubeX = (bits.cubeXUpper << 8) | bits.cubeXLower;
      uint cubeY = (bits.cubeYUpper << 10) | (bits.cubeYMiddle << 4) | bits.cubeYLower;
      uint32_t standardID = (bits.standardIDUpper << 20) | (bits.standardIDMiddleHigh << 12) | (bits.standardIDMiddleLow << 4) | (bits.standardIDLower);
      int roll = bits.eulerAngleRoll;
      if(bits.eulerAngleRollSign) roll = -roll;
      int pitch = bits.eulerAnglePitch;
      if(bits.eulerAnglePitchSign) pitch = -pitch;
      int yaw = bits.eulerAngleYaw;
      if(bits.eulerAngleYawSign) yaw = -yaw;
      M5.Log.printf("serialized res%u, btn%u, IDstatus%u IDangle%u, X%u Y%u ID0x%08x spdL%u spdR%u MTR%u MA%u Bat%u Ho%u Sh%u Co%u DT%u R%u Rol%d Pit%d Yaw%d R%u Ms%u Mf%u\n",  bits.reserved, 
        bits.button, 
        bits.toioIDstatus, toioIDangle,
        cubeX, cubeY,
        standardID, bits.motorSpeedL, bits.motorSpeedR,
        bits.moveToTargetsResponse, bits.motionAngleDetect, bits.battery,
        bits.motionHorizontalDetect, bits.motionShakedDetect, bits.motionCollisionDetect, bits.motionDoubleTapDetect,
        bits.reserved2,
        roll, pitch, yaw,
        bits.reserved3,
        bits.magnetStatus, bits.magnetForce); 
    } else 
      M5.Log.printf("set configuration %02x %02x ", resp.config.reserved, resp.config.response);

    M5.Log.printf("\n");
  });
```

設定変更の応答については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E8%AA%AD%E3%81%BF%E5%87%BA%E3%81%97%E6%93%8D%E4%BD%9C)をご覧ください。

### <a id="ToioCore-setConnectionInterval-method">✔ `setConnectionInterval()` メソッド (コネクションインターバル値の変更)</a>

M5Stackとtoio コア キューブの間のBLEコネクションインターバルの最小時間と最大時間を設定します。
指定可能な値はBLE規格により定められています。 

#### プロトタイプ宣言

```c++
 void setConnectionInterval(uint16_t minimum, uint16_t maximum);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `minimum`     | `uint16_t`     |  ✔  | min値(0xFFFF（要求なし）または 0x0006 ~ 0x0C80、単位は1.25ms)
2   | `maximum`     | `uint16_t`     |  ✔  | max値(0xFFFF（要求なし）または 0x0006 ~ 0x0C80、単位は1.25ms)

コネクションインターバル値の変更の設定については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E3%82%B3%E3%83%8D%E3%82%AF%E3%82%B7%E3%83%A7%E3%83%B3%E3%82%A4%E3%83%B3%E3%82%BF%E3%83%BC%E3%83%90%E3%83%AB%E5%A4%89%E6%9B%B4%E8%A6%81%E6%B1%82-)をご覧ください。

### <a id="ToioCore- getRequestedConnectionInterval-method">✔ ` getRequestedConnectionInterval()` メソッド (コネクションインターバル要求値の取得)</a>

setConnectionInterval()メソッドで設定を要求したBLEコネクションインターバルの要求値を取得します。

#### プロトタイプ宣言

```c++
 void getRequestedConnectionInterval(uint16_t& minimum, uint16_t& maximum);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `minimum`     | `uint16_t`     |  ✔  | min値(単位は1.25ms)
2   | `maximum`     | `uint16_t`     |  ✔  | max値(単位は1.25ms)

コネクションインターバル要求値の取得については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E3%82%B3%E3%83%8D%E3%82%AF%E3%82%B7%E3%83%A7%E3%83%B3%E3%82%A4%E3%83%B3%E3%82%BF%E3%83%BC%E3%83%90%E3%83%AB%E8%A6%81%E6%B1%82%E5%80%A4%E3%81%AE%E5%8F%96%E5%BE%97%E3%81%AE%E5%BF%9C%E7%AD%94-)をご覧ください。

### <a id="ToioCore- getAcctualConnectionInterval-method">✔ ` getAcctualConnectionInterval()` メソッド (現在のコネクションインターバル値の取得)</a>

現在のBLEコネクションインターバル値を取得します。

#### プロトタイプ宣言

```c++
 void getAcctualConnectionInterval(uint16_t& minimum, uint16_t& maximum);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `minimum`     | `uint16_t`     |  ✔  | min値(単位は1.25ms)
2   | `maximum`     | `uint16_t`     |  ✔  | max値(単位は1.25ms)

現在のコネクションインターバル要求値の取得については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E7%8F%BE%E5%9C%A8%E3%81%AE%E3%82%B3%E3%83%8D%E3%82%AF%E3%82%B7%E3%83%A7%E3%83%B3%E3%82%A4%E3%83%B3%E3%82%BF%E3%83%BC%E3%83%90%E3%83%AB%E5%80%A4%E3%81%AE%E5%8F%96%E5%BE%97%E3%81%AE%E5%BF%9C%E7%AD%94-)をご覧ください。


### <a id="ToioCore-getConfigurationResponse-method">✔ `getConfigurationResponse()` メソッド (設定変更の応答を取得)</a>

setFlatThreshold()など、設定系のメソッド呼び出しの結果応答を取得します。

#### プロトタイプ宣言

```c++
// 設定応答
// 設定変更の応答
struct ToioCoreSetConfigurationResponse {
    uint8_t reserved;
    uint8_t response;
};

// コネクションインターバル値
struct ToioCoreConnectionIntervalSettings {
    uint8_t reserved;
    uint16_t minimum;
    uint16_t maximum;
};

// 設定characteristicsの読み出し/notifyの応答
struct ToioCoreConfigurationResponse {
  uint8_t infoType;
  union {
    ToioCoreSetConfigurationResponse config; // 設定応答 infoType = 0x98 0x99 0x9b 0x9c 0x9d 0x9e 0xb0
    ToioCoreConnectionIntervalSettings interval; // コネクションインターバル値 infoType =  0xb1 or 0xb2
    uint8_t serialized[19]; // シリアライズ情報   info Type = 0xf0
  }; 
};

ToioCoreConfigurationResponse getConfigurationResponse();
```

#### 引数

なし

設定の結果応答については、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_configuration#%E8%AA%AD%E3%81%BF%E5%87%BA%E3%81%97%E6%93%8D%E4%BD%9C)をご覧ください。


### <a id="ToioCore-controlMotor-method">✔ `controlMotor()` メソッド (モーター制御)</a>

toio コア キューブのモーターを制御します。

#### プロトタイプ宣言

```c++
void controlMotor(bool ldir, uint8_t lspeed, bool rdir, uint8_t rspeed, uint16_t duration = 0);
```

#### 引数

No. | 変数名      | 型         | 必須   | 説明
:---|:-----------|:-----------|:-------|:-------------
1   | `ldir`     | `bool`     | ✔     | 左モーターの回転方向 (`true`: 前進, `false`: 後進)
2   | `lspeed`   | `uint8_t`  | ✔     | 左モーターの速度 (`0` ～ `115`)
3   | `rdir`     | `bool`     | ✔     | 右モーターの回転方向 (`true`: 前進, `false`: 後進)
4   | `rspeed`   | `uint8_t`  | ✔     | 右モーターの速度 (`0` ～ `115`)
5   | `duration` | `uint16_t` | &nbsp; | モーターの制御時間 (ミリ秒)

`lspeed` および `rspeed` の値と実際の速度の対応は、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_motor)をご覧ください。

`duration` に `0` (デフォルト値) を指定すると、停止 (`lspeed`, `rspeed` に `0` をセット) 命令を受けるまで動き続けます。


#### コードサンプル

```c++
// 右に曲がりながら進む
toiocore->controlMotor(true, 50, true, 40);
delay(5000);

// 停止
displayProgressMessage("Stopping...");
toiocore->controlMotor(true, 0, true, 0);
delay(5000);

// 2 秒間だけ左に曲がりながら進む
toiocore->controlMotor(true, 40, true, 50, 2000);
delay(5000);
```

### <a id="ToioCore-drive-method">✔ `drive()` メソッド (運転)</a>

ゲームパッドやジョイスティックを使うことを想定したモーター制御メソッドです。モーター制御をスロットルとステアリング操作に置き換えます。

#### プロトタイプ宣言

```c++
void drive(int8_t throttle, int8_t steering);
```

#### 引数

No. | 変数名      | 型        | 必須   | 説明
:---|:-----------|:----------|:-------|:-------------
1   | `throttle` | `int8_t`  | ✔     | スロットル (`-100` ～ `100`)
2   | `steering` | `int8_t`  | ✔     | ステアリング (`-100` ～ `100`)

`throttle` に正の値を指定すると前進、負の値を指定すると後進、`0` を指定すると停止します。`steering` に正の値を指定すると右に曲がり、負の値を指定すると左に曲がります。

`steering` の絶対値が大きくなるにつれて、一方のモーターの回転数を緩めることで、ステアリング操作を実現しています。もし `throttle` に `100` を指定し、`steering` に `100` を指定すると、左モーターの速度は `100` のままに、右モーターの速度が `0` になります。つまり、右タイヤを軸に左にくるくる回ることになります。ラジコンカーのような動きを求めるなら、`steering` のリミットを `50` 程度に抑えておくのが良いでしょう。

#### コードサンプル

```c++
// ゆっくりと左に曲がる
toiocore->drive(20, -10);
delay(5000);

// 停止
toiocore->drive(0, 0);
```

もし戦車のように左右のタイヤをそれぞれ反対方向に回転させて本体の中心を軸にくるくる回る動きを実現したい場合は、前述の [`controlMotor()`](#ToioCore-controlMotor-method) メソッドを使ってください。

### <a id="ToioCore-controlMotorWithTarget-method">✔ `controlMotorWithTarget()` メソッド (目標指定付きモーター制御 (目標１つ))</a>

目標地点を一つ指定してモーターを制御し、キューブを自律的に移動させるメソッドです。

#### プロトタイプ宣言

```c++
void controlMotorWithTarget(uint8_t distinction, uint8_t timeout, uint8_t movement_type, 
  uint8_t maximum_speed, uint8_t speed_change_type,
  uint16_t target_x, uint16_t target_y, 
  uint16_t target_angle_degree, uint8_t target_angle_and_rotation_bits = 0);
```

#### 引数

No. | 変数名         | 型        | 必須   | 説明
:---|:---------------|:----------|:-------|:-------------
1   | `distinction`  | `uint8_t`  | ✔     | 制御識別値
2   | `timeout`      | `uint8_t`  | ✔     | タイムアウト時間(秒)
3   | `movement_type` | `uint8_t`  | ✔     | 移動タイプ (`0` ～ `2`)
4   | `maximum_speed` | `uint8_t`  | ✔     | モーターの最大速度指示値 (`10` ～ `2550`)
5   | `speed_change_type` | `uint8_t`  | ✔ | モーターの速度変化タイプ (`0` ～ `3`)
6   | `target_x`     | `uint16_t`  | ✔     | 目標地点の X 座標値(`0` ～ `65535`)
7   | `target_y`     | `uint16_t`  | ✔     | 目標地点の Y 座標値 (`-0` ～ `65535`)
8   | `target_angle_degree` | `uint16_t`  | ✔     | 目標地点でのキューブの角度  (`0` ～ `0x1fff`) 
9   | `target_angle_and_rotation_bits` | `uint8_t`  | &nbsp;  |角度の意味と動き方の指定 (`0` ～ `6`)

#### コードサンプル

```c++
// 座標(150, 200)の位置、角度0の方向で止まるように速度80で移動
toiocore->controlMotorWithTarget(1, 5, 0, 80, 0, 150, 200, 0);
delay(5000);
```

## <a id="ToioCore-controlMotorWithMultipleTargets-method">✔ `controlMotorWithMultipleTargets()` メソッド (目標指定付きモーター制御 (目標複数))</a>

目標地点を複数指定してモーターを制御し、キューブを自律的に移動させるメソッドです。

#### プロトタイプ宣言

```c++
struct ToioCoreTargetPos {
  uint16_t posX; // 目標地点の X 座標値
  uint16_t posY; // 目標地点の Y 座標値 
  uint16_t angleDegree:13; // 目標地点でのキューブの角度(0～0x1fff)
  uint8_t angleAndRotation:3; //角度の意味と動き方(0～6)
};

void controlMotorWithMultipleTargets(uint8_t distinction, uint8_t timeout, uint8_t movement_type, 
      uint8_t maximum_speed, uint8_t speed_change_type,  uint8_t addition_setting,
      uint8_t target_num, ToioCoreTargetPos *target_positions);
```

#### 引数

No. | 変数名         | 型        | 必須   | 説明
:---|:---------------|:----------|:-------|:-------------
1   | `distinction`  | `uint8_t`  | ✔     | 制御識別値
2   | `timeout`      | `uint8_t`  | ✔     | タイムアウト時間(秒)
3   | `movement_type` | `uint8_t`  | ✔     | 移動タイプ (`0` ～ `2`)
4   | `maximum_speed` | `uint8_t`  | ✔     | モーターの最大速度指示値 (`10` ～ `2550`)
5   | `speed_change_type` | `uint8_t`  | ✔ | モーターの速度変化タイプ (`0` ～ `3`)
6   | `addition_setting` | `uint8_t`  | ✔ | 書き込み操作の追加設定(`0` ～ `1`)
7   | `target_num`     | `uint16_t`  | ✔     | ターゲット座標の数(`1` ～ `29`)
8   | `target_positions` | `ToioCoreTargetPos *`  | ✔     | ターゲット座標の配列へのポインタ

#### コードサンプル

```c++
// ５つの目標地点を指定して移動
  ToioTargetPos pos[5];
  pos[0].x = 250;
  pos[0].y = 250;
  pos[0].angle = 0;
  pos[1].x = 250;
  pos[1].y = 300;
  pos[1].angle = 90;
  pos[2].x = 300;
  pos[2].y = 300;
  pos[2].angle = 180;
  pos[3].x = 300;
  pos[3].y = 250;
  pos[3].angle = 270;
  pos[4].x = 250;
  pos[4].y = 250;
  pos[4].angle = 0;
  toiocore->controlMotorWithMultipleTargets(0, 0, 0, 50, 0, 0, 5, pos);
  delay(5000);
```

### <a id="ToioCore-controlMotorWithAcceleration-method">✔ `controlMotorWithAcceleration()` メソッド (加速度指定モーター制御)</a>

キューブの加速度を指定してモーターを制御するメソッドです。

#### プロトタイプ宣言

```c++
void controlMotorWithAcceleration(uint8_t translational_speed, uint8_t acceleration,
  uint16_t rotational_velocity, uint8_t rotational_direction, uint8_t travel_direction,
  uint8_t priority, uint8_t duration);
```

#### 引数

No. | 変数名         | 型        | 必須   | 説明
:---|:---------------|:----------|:-------|:-------------
1   | `translational_speed`  | `uint8_t`  | ✔     | キューブの並進速度(`0` ～ `255`)
2   | `acceleration` | `uint8_t`  | ✔     | キューブの加速度(100 ミリ秒ごとの速度の増加分 `0` ～ `255`)
3   | `rotational_velocity` | `uint16_t`  | ✔     | キューブの向きの回転速度 (度/秒 `0` ～ `65535`)
4   | `rotational_direction` | `uint8_t`  | ✔     | キューブの向きの回転方向	 (`0` 正 ～ `1` 負)
5   | `travel_direction` | `uint8_t`  | ✔ | キューブの進行方向 (`0` 前進 ～ `1` 後退)
6   | `priority`     | `uint8_t`  | ✔     | 速度調整優先指定(`0` 並進優先回転調整  ～ `1` 回転優先速度調整)
7   | `duration`     | `uint8_t`  | ✔     | 制御時間  x10 ミリ秒 0は制限なし(`0` ～ `255`)

#### コードサンプル

```c++
// ゆっくり弧を描きながら 2 秒間移動します。
toiocore->controlMotorWithAcceleration(50, 15, 30, 0, 0, 0, 200);
delay(5000);
```

### <a id="ToioCore-onMotor-method">✔ `onMotor()` メソッド (モーター制御の応答コールバックをセット)</a>

toio コア キューブのモーター制御の応答イベントのコールバックをセットします。
目標指定付きモーター制御の応答、複数目標指定付きモーター制御の応答、モーターの速度情報(デフォルトでは無効)のいずれかが返ります。

#### プロトタイプ宣言

```c++
// モーター制御の応答
struct ToioCoreMotorResponse {
  uint8_t controlType;  // 制御の種類
  union {
    uint8_t controlID; // 制御識別値 
    uint8_t leftSpeed; // または 左モーターの速度
  };
  union {
   uint8_t response; // 応答内容
   uint8_t rightSpeed;  // または右モーターの速度
  };
};

typedef std::function<void(ToioCoreMotorResponse motor_response)> OnMotorCallback;
void onMotor(OnMotoraCallback cb);
```

#### 引数

No. | 変数名   | 型                 | 必須   | 説明
:---|:--------|:-------------------|:-------|:-------------
1   | `cb`    | `OnMotorCallback` | ✔     | コールバック関数

#### コードサンプル

以下のサンプルスケッチは、目標指定付きモーター制御の応答、複数目標指定付きモーター制御の応答、モーターの速度情報のいずれかを得るものです。

コールバックを使う場合は、`.ino` ファイルの `loop()` 関数内で `Toio` オブジェクトの [`loop()`](#Toio-loop-method) メソッドを呼び出してください。コールバックは、`.ino` ファイルの `loop()` 関数が実行が開始されてから発生したイベントしかハンドリングできませんので注意してください。

```c++
#include <M5Unified.h>
#include <Toio.h>

// Toio オブジェクト生成
Toio toio;

void setup() {
  // M5Stack の初期化
  M5.begin();

  // toio コア キューブのスキャン開始
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // BLE 接続
  toiocore->connect();

  // モーター制御の応答イベントのコールバックをセット
  toiocore->onMotor([](ToioCoreMotorResponse motor_res) {
    M5.Log.printf("Motor Event ");
    M5.Log.print(f"Type=%u", motor_res.controlType);
    if(motor_res.controlType == 0xe0 )
      M5.Log.printf( ", left speed=%u, right speed=%u\n", motor_res.controlID, motor_res.response);
    else
      M5.Log.printf( ", controlID=%u, response=%u\n", motor_res.controlID, motor_res.response);
  });  
}

void loop() {
  // コールバックを使う場合には必ず Toio オブジェクトの loop() を呼び出す
  toio.loop();
}
```

### <a id="ToioCore-getIDReaderData-method">✔ `getIDReaderData()` メソッド (ID読み取りセンサーの状態、マット上の位置情報を取得)</a>

toio コア キューブのID読み取りセンサーの状態を取得します。キューブがマット上にあるとき、マットからセンサーで読み取った値(ID)が取得できます。マットから読み取ることができるIDには、マット上の座標値を表すポジションIDと、キューブが乗っている領域を表すスタンダードIDがあり、どちらかの情報が返されます。

#### プロトタイプ宣言

```c++
struct ToioCoreIDData {
  ToioCoreIDType type; // 	情報の種類
  union {
    ToioCorePositionIDData position; // キューブの座標と角度
    ToioCoreStandardIDData standard; // キューブの下のStandard IDとキューブの角度
  };
};

struct ToioCorePositionIDData {
  uint16_t cubePosX; // 	キューブの中心の X 座標値
  uint16_t cubePosY; //   キューブの中心の Y 座標値
  uint16_t cubeAngleDegree; // キューブの角度
  uint16_t sensorPosX;  // 読み取りセンサーの X 座標値
  uint16_t sensorPosY;  // 読み取りセンサーの Y 座標値
  uint16_t sensorAngleDegree;  // 読み取りセンサーの角度
};

struct ToioCoreStandardIDData {
  uint32_t standardID;  // Standard ID の値
  uint16_t cubeAngleDegree; // キューブの角度
};

ToioCoreIDData getIDReaderData();
```

#### 引数

なし

#### 戻値

`ToioCoreIDData` 型の構造体が返されます。各メンバーの意味は以下の通りです。

名前       | 型        | 説明
:----------|:----------|:------------------------
`type`     | `ToioCoreIDType`    | IDタイプ (`ToioCoreIDTypeNone`: 読み取れなかった場合, `ToioCoreIDTypePosition`: ポジションIDの場合, `ToioCoreIDTypeStandard`: スタンダードIDの場合) 
`position`    | `ToioCorePositionIDData`    | ポジションID情報
`standard`     | `ToioCoreStandardIDData`    | スタンダードID情報

type が ToioCoreIDTypePosition の場合、position 値には、`ToioCorePositionIDData` 型の構造体が返されます。各メンバーの意味は以下の通りです。


名前       | 型        | 説明
:----------|:----------|:------------------------
`cubePosX`     | `uint16_t`    | キューブの中心の X 座標値
`cubePosY`     | `uint16_t`    | キューブの中心の Y 座標値
`cubeAngleDegree`     | `uint16_t`    | キューブの角度（度）
`sensorPosX`     | `uint16_t`    | 読み取りセンサーの中心の X 座標値
`sensorPosY`     | `uint16_t`    | 読み取りセンサーの中心の Y 座標値
`sensorAngleDegree`     | `uint16_t`    | 読み取りセンサーの角度（度）

type が ToioCoreIDTypeStandard の場合、standard 値には、`ToioCoreStandardIDData` 型の構造体が返されます。各メンバーの意味は以下の通りです。

名前       | 型        | 説明
:----------|:----------|:------------------------
`standardID`     | `uint32_t`    | Standard ID の値
`cubeAngleDegree`     | `uint16_t`    | キューブの角度（度）


position および standard の値の詳細仕様は、[toio コア キューブ通信仕様](https://toio.github.io/toio-spec/docs/ble_id)をご覧ください。

Standard IDの値については[Standard ID 一覧](https://toio.github.io/toio-spec/docs/hardware_standard_id)をご覧ください。

#### コードサンプル

```c++
ToioCoreIDData data = toiocore->getIDReaderData();
if (data.type == ToioCoreIDTypePosition) {
  M5.Log.printf("posX: %u posY: %u angle: %u\n",
    data.position.cubePosX, data.position.cubePosY, data.position.cubeAngleDegree);
}
else if (data.type == ToioCoreIDTypeStandard) {
  M5.Log.printf("Standard ID: %u\n", data.standard.standardID);
}
else {
  M5.Log.println("no id found.");
}
```

### <a id="ToioCore-onIDReaderData-method">✔ `onIDReaderData()` メソッド (ID読み取りセンサーのコールバックをセット)</a>

toio コア キューブのID読み取りイベントのコールバックをセットします。IDが読み取られている間一定時間間隔で、およびID読み取りが終了（失敗）したとき、引数に指定したコールバック関数を呼び出します。コールバック関数には読み取られたIDを表す構造体が引き渡されます。

#### プロトタイプ宣言

```c++
struct ToioCoreIDData {
  ToioCoreIDType type;
  ToioCorePositionIDData position;
  ToioCoreStandardIDData standard;
};

// Position IDの場合のデータ
struct ToioCorePositionIDData {
  uint16_t cubePosX;
  uint16_t cubePosY;
  uint16_t cubeAngleDegree;
  uint16_t sensorPosX;
  uint16_t sensorPosY;
  uint16_t sensorAngleDegree;
};

// Standard IDの場合のデータ
struct ToioCoreStandardIDData {
  uint32_t standardID;
  uint16_t cubeAngleDegree;
};

typedef std::function<void(ToioCoreIDData id_data)> OnIDDataCallback;
void onIDReaderData(OnIDDataCallback cb);
```

#### 引数

No. | 変数名   | 型                 | 必須   | 説明
:---|:--------|:-------------------|:-------|:-------------
1   | `cb`    | `OnIDDataCallback` | ✔     | コールバック関数

#### コードサンプル

以下のサンプルスケッチは、IDが読み取られたとき、またはID読み取りが終了（失敗）したとき、その都度、その状態を出力します。

コールバックを使う場合は、`.ino` ファイルの `loop()` 関数内で `Toio` オブジェクトの [`loop()`](#Toio-loop-method) メソッドを呼び出してください。コールバックは、`.ino` ファイルの `loop()` 関数が実行が開始されてから発生したイベントしかハンドリングできませんので注意してください。

```c++
#include <M5Unified.h>
#include <Toio.h>

// Toio オブジェクト生成
Toio toio;

void setup() {
  // M5Stack の初期化
  M5.begin();
  M5.Power.begin();

  // toio コア キューブのスキャン開始
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // BLE 接続
  toiocore->connect();

  // ID読み取りイベントのコールバックをセット
  toiocore->onIDReaderData([](ToioCoreIDData data) {
    if (data.type == ToioCoreIDTypePosition) {
      M5.Log.printf("posX: %u posY: %u angle: %u\n",
        data.position.cubePosX, data.position.cubePosY, data.position.cubeAngleDegree);
    }
    else if (data.type == ToioCoreIDTypeStandard) {
      M5.Log.printf("Standard ID: %u\n", data.standard.standardID);
    }
    else {
      M5.Log.println("no id found.");
    }
  });
}

void loop() {
  // コールバックを使う場合には必ず Toio オブジェクトの loop() を呼び出す
  toio.loop();
}
```

---------------------------------------
## <a id="Sample-Sketches">6. サンプルスケッチ</a>

本ライブラリのインストールが完了すると、Arduino IDE のメニューバーの `ファイル` -> `スケッチ例` の中から `M5StackToio` が選択できるようになります。この中には以下の 6 つのサンプルが用意されています。

このうち`basic`、`event`、`joystick_drive`はfutomiさんオリジナルのサンプルコードで [M5Stack Basic](https://www.switch-science.com/catalog/3647/) および [M5Stack Gray](https://www.switch-science.com/catalog/3648/) で動作します。

`basic_test`、`event_test`、`event2_test`、`multi_corecube_test`、`multiple_targetpos_test`、`config_event_test`は、新たに加えたサンプルコードで、M5Unifiedライブラリを使っているので、ボタンが一つ以上あるM5Stack製コントローラ製品であれば動作します。(出力結果はLCDではなくシリアルポートに出力されます)

### `basic_test`

オリジナルのスケッチ`basic`と同様に本ライブラリが提供するメソッドを一通り実行するスケッチです。

事前に toio コア キューブ の電源を入れてください。

M5Stack が起動すると自動的に toio コア キューブ をスキャンします。toio コア キューブ が見つかると シリアルポートにデバイス名と MAC アドレスが出力されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを押して再起動してください。

toio コア キューブ が発見されると、自動的に BLE 接続し、一連のメソッドを順次実行しながら シリアルポートに結果を表示します。

目標地点つき移動のテストを行う場合は、toio コア キューブ（単体）に付属する簡易プレイマットを用意するか目標地点の座標を手持ちのプレイマットに対応した座標に変更してください。
https://toio.github.io/toio-spec/docs/hardware_position_id

一連のメソッドの実行が完了すると、自動的に BLE 接続を切断して終了します。

### `event_test`

オリジナルのスケッチ`event`と同様に本ライブラリが提供するイベントをハンドリングするサンプルスケッチです。
ただし、arduino ESP32ではnotifyの登録が4つまでしか動作しないため、同時に４つまでしかイベントをハンドリングするコールバックを設定できませんので注意してください。(ソースコードのコメントも参照)
要確認: NimBLEの場合は5つ以上コールバックをセットできるようです。

事前に toio コア キューブ の電源を入れてください。

M5Stack が起動すると自動的に toio コア キューブ をスキャンします。toio コア キューブ が見つかると シリアルポートにデバイス名と MAC アドレスが表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを押して再起動してください。

toio コア キューブ が発見された後、各種イベントを待ち受け、受信したイベント情報を シリアルポートに表示します。
M5Stack の A ボタンを押すと toio コア キューブ が指定した目標地点に移動し、移動を完了するとMotorイベントを受信し、結果をシリアルポートに表示します。このテストを行う場合は、toio コア キューブ（単体）に付属する簡易プレイマットを用意するか目標地点の座標を手持ちのプレイマットに対応した座標に変更してください。
https://toio.github.io/toio-spec/docs/hardware_position_id

M5Stack の A ボタンを2秒以上押すと toio コア キューブ とのBLE 接続を接続/切断します。

### `event2_test`

デフォルトではオフの機能(IDの通知間隔の変更、磁気センサーの使用、姿勢角度の通知、モーター速度の通知)をテストするスケッチです。

事前に toio コア キューブの電源を入れてください。

M5Stack が起動すると自動的に toio コア キューブ をスキャンします。toio コア キューブが見つかるとシリアルポートのログにデバイス名と MAC アドレスが表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを押して再起動してください。

接続が完了すると、デフォルトではオフの機能(IDの通知間隔の変更、磁気センサーの使用、姿勢角度の通知、モーター速度の通知)をオンにし、それぞれのイベントを待ち受け、受信したイベント情報を シリアルポートのログに表示します。

toio コア キューブが接続された後、M5Stack の A ボタン押すと、toio コア キューブが加速度指定モーター制御で移動します。(速度の変化が通知されます。)

M5Stack の Aボタンを2秒以上押すと toio コア キューブとの BLE 接続を遮断または再接続します。

arduino ESP32のデフォルトのBLEライブラリではnotifyの登録が4つまでしか動作しないため、同時に４つまでしかイベントをハンドリングするコールバックを設定できませんので注意してください。(モーション、磁気、姿勢角度は同じcharacteristicを使うので１つにカウントします。)
要確認: NimBLEの場合は5つ以上コールバックをセットできるようです。

### `multi_corecube_test`

複数のToio Core Cubeを接続し、制御し、イベントを受け取るサンプルスケッチです。

2つ以上6つまでtoio core cubeを接続して動かすことができます。
NimBLEのデフォルト設定では3つまでしか接続できないので、nimconfig.hの #define CONFIG_BT_NIMBLE_MAX_CONNECTIONS の値を6に変更してください。(ESP32では9個まで接続できるようですが、手持ちのToio Core Cubeは6つしかないので試せていません)
arduino ESP32のデフォルトのBLEライブラリでは4つまでしか接続できないようです。

事前に Toio Core Cube の電源を入れてマットの上に置いてください。(toio コア キューブ（単体）付属の簡易プレイマットで動作します。ほかのマットを使う場合はMAT_CENTER_X, MAT_CENTER_Yの値を変更してください)

M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
Toio Core Cube が見つかると M5Stack シリアルポートのログにデバイス名と MAC アドレスが表示されます。見つからなければ、再度、M5Stack のリセットボタンを押して再起動してください。

Toio Core Cubeとの接続が確立すると、Toio Core Cubeのランプ(LED)が常時点灯になります。
すべてのToio Core Cubeと接続が終わった後、、M5Stack の A ボタン押すとToio Core Cubeの数に合わせた位置(3つなら三角形の頂点の位置、4つなら四角形、6つなら六角形)に移動します。 (初回は置いた位置によってはToio Core Cubeが衝突するので手で場所を変えてあげてください)
M5Stack の A ボタン押すごとにToio Core Cubeは位置を変えます。
  
Toio Core Cubeのボタンを押すと、押したToio Core Cubeで効果音が鳴り、押していないToio Core Cubeが300ms回転します。

### `multiple_targetpos_test`

複数の目標地点つき移動のテストを行うスケッチです。
toio コア キューブ（単体）に付属する簡易プレイマットを用意するか目標地点の座標を手持ちのプレイマットに対応した座標に変更してください。
https://toio.github.io/toio-spec/docs/hardware_position_id

Toio Core Cubeとの接続が確立した後、M5Stack の A ボタン押すと、マットの中心位置に移動した後、
外側に向かってらせん状に(#define MAX_TOIOCORE_TARGET_NUM 29で)指定した複数の目的地を縫うように移動します。
Toio Core Cubeの通信プロトコル仕様では29が最大値です。

### `config_event_test`

設定の応答とシリアライズ情報をnotifyで受信するスケッチです。
Toio Core Cubeとの接続が完了すると、設定の応答ととシリアライズ情報のnotifyの受信をテストします。
設定の応答イベントを待ち受け、受信した設定の応答およびシリアライズ情報をシリアルポートのログに表示します。

---------------------------------------
以下はfutomiさんのオリジナルのサンプルコード
M5Stackの画面に動作ログを表示するため、画面を持った指定のM5Stackコントローラ製品で動かしてください。

### `basic`

本ライブラリが提供するメソッドを一通り実行するスケッチです。

事前に toio コア キューブ の電源を入れてください。

M5Stack が起動すると自動的に toio コア キューブ をスキャンします。toio コア キューブ が見つかると M5Stack の画面にデバイス名と MAC アドレスが表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを押して再起動してください。

toio コア キューブ が発見されると、自動的に BLE 接続し、一連のメソッドを順次実行しながら M5Stack の画面に結果を表示します。

一連のメソッドの実行が完了すると、自動的に BLE 接続を切断して終了します。

### `event`

本ライブラリが提供するイベントをハンドリングするサンプルスケッチです。

事前に toio コア キューブ の電源を入れてください。

M5Stack が起動すると自動的に toio コア キューブ をスキャンします。toio コア キューブ が見つかると M5Stack の画面にデバイス名と MAC アドレスが表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを押して再起動してください。

toio コア キューブ が発見された後、M5Stack の A ボタンを押すと toio コア キューブ と BLE 接続します。接続が完了すると、各種イベントを待ち受け、受信したイベント情報を M5Stack の画面に表示します。

### `joystick_drive`

[M5Stack用ジョイスティックユニット](https://www.switch-science.com/catalog/4050/)を使って toio コア キューブを運転するサンプルスケッチです。

 事前に toio コア キューブ の電源を入れてください。

M5Stack が起動すると自動的に toio コア キューブ をスキャンします。toio コア キューブ が見つかると M5Stack の画面にデバイス名と MAC アドレスが表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを押して再起動してください。

toio コア キューブ と BLE 接続する前にジョイスティックをキャリブレーションする必要があります。M5Stack の画面上にジョイスティックの座標が表示されます。もし x と y の値が 0 でなければ、ジョイスティックの中心座標がずれています。M5Stack の C ボタンを押してキャリブレーションしてください。

toio コア キューブ が発見された後、M5Stack の A ボタンを押すと toio コア キューブ と BLE 接続します。接続が完了すると、ジョイスティックで toio コア キューブ を運転することができます。

BLE 接続中、ジョイスティックの z 軸を押すと、チャルメロが再生されます。また、M5Stack の B ボタンを押すと、toio コア キューブ の LED が白で点灯します。

[![joystick_drive のデモ](https://img.youtube.com/vi/FLccNi00Pds/0.jpg)](https://www.youtube.com/watch?v=FLccNi00Pds)

---------------------------------------
## <a id="Release-Note">リリースノート</a>

* v1.0.0 (2020-07-19)
  * 初版リリース(futomiさん版)

* v1.0.0 (2023-10-09)
  * kenichi884版　最初のgithub公開 toio core cube BLE protocol v2.3.0に対応。

* v1.0.0 (2023-12-24)
  * kenichi884版　サンプルコード、Readmeを微修正、以降の修正との区別のためv1.0.0タグつけ。

* v1.0.1 (2023-12-24)
  * kenichi884版　NimBLE使用に変更、サンプルコードののSerial.print()をM5.Log.printf()に変更

* v1.0.2 (2024-01-07)
  * kenichi884版　複数のtoio core cubeを接続した場合、get系メソッド、イベント(notify)が個別のtoio core cubeの情報を返していなかったのを修正

* v1.0.3. (2024-02-07)
  * kenichi884 version　英語版の Reaadme.md を追加. struct ToioCoreIDDataと struct ToioCoreMotorResponse の定義を変更(unionを使うようにした).　

* v1.0.4. (2024-05-03)
  * kenichi884 version　BLEプロトコルバージョン2.4.0の機能(クォータニオン、高精度オイラー角での姿勢角取得、コネクションインターバル値の設定)に対応。シリアライズ情報の通知に対応(これはBLEプロトコルバージョン2.3.0の機能だったが未対応だった)。
---------------------------------------
## <a id="References">リファレンス</a>

* [toio 公式ページ](https://toio.io/)
* [toio コア キューブ 技術仕様](https://toio.github.io/toio-spec/)
* [toio Core Cube Specifications](https://toio.github.io/toio-spec/en/)
* [M5Stack](https://m5stack.com/)
  * [ESP32 Basic Core IoT Development Kit](https://m5stack.com/collections/m5-core/products/basic-core-iot-development-kit)
  * [ESP32 GREY Development Kit with 9Axis Sensor](https://m5stack.com/collections/m5-core/products/grey-development-core)
  * [I2C Joystick Unit (MEGA328P)](https://m5stack.com/products/joystick-unit)
* [スイッチサイエンス](https://www.switch-science.com/)
  * [M5Stack Basic](https://www.switch-science.com/catalog/3647/)
  * [M5Stack Gray](https://www.switch-science.com/catalog/3648/)
  * [M5Stack用ジョイスティックユニット](https://www.switch-science.com/catalog/4050/)

---------------------------------------
## <a id="License">License</a>

The MIT License (MIT)

Copyright (c) 2020 Futomi Hatano
Toio ID read support   https://github.com/mhama
Protocol v2.3.0 or later support  https://github.com/kenichi884 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
