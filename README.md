M5StackToio
===============

M5StackToio はソニー・インタラクティブエンタテインメント社の「[toio コア キューブ](https://toio.io/)」を操作するための [M5Stack](https://m5stack.com/) 用 Arduino ライブラリです。

---------------------------------------
## 目次

* [1. 開発環境セットアップ](#Setup-IDE)
* [2. M5StackToio のインストール](#Install-M5StackToio)
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

本ページの右上の `Code` ボタンをクリックし `Download ZIP` を選択し、本ライブラリの zip ファイルをダウンロードしてください。

Arduino IDE を起動し、メニューバーの `スケッチ` -> `ライブラリをインクルード` -> `.ZIP形式のライブラリをインストール...` を選択してください。先ほどダウンロードした本ライブラリの zip ファイルを選択すると、本ライブラリのインストールが完了します。

もし git コマンドが利用できるのであれば、次のようにインストールすることも可能です。以下は Windows10 の PowerShell を使った場合の例です。

```
cd ~
cd Documents\Arduino\libraries
git clone https://github.com/kenichi884/M5StackToio.git
```

---------------------------------------
## <a id="Usage">3. 使い方</a>

以下のサンプルコードは、toio コア キューブをスキャンし、最初に発見した toio コア キューブに接続し、3 秒後に切断します。

```c++
#include <M5Stack.h>
#include <Toio.h>

// Toio オブジェクト生成
Toio toio;

void setup() {
  // M5Stack の初期化
  M5.begin();
  M5.Power.begin();
  M5.Lcd.clear();

  Serial.println("- toio コア キューブをスキャンします。");
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  size_t n = toiocore_list.size();
  if (n == 0) {
    Serial.println("- toio コア キューブが見つかりませんでした。");
    return;
  }
  Serial.printf("- %d 個の toio コア キューブが見つかりました。\n", n);

  Serial.println("- toio コア キューブに BLE 接続します。");
  ToioCore* toiocore = toiocore_list.at(0);
  bool connected = toiocore->connect();
  if (!connected) {
    Serial.println("- BLE 接続に失敗しました。");
    return;
  }
  Serial.println("- BLE 接続に成功しました。");
  Serial.println("- 3 秒後に切断します。");
  delay(3000);
  toiocore->disconnect();
  Serial.println("- BLE コネクションを切断しました。");
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
#include <M5Stack.h>
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
  Serial.println("toio コア キューブが見つかりませんでした。");
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
  Serial.println("BLE 接続に失敗しました。");
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
Serial.println(mac.c_str()); // 例 "d1:52:fa:d2:c6:a1"
```

### <a id="ToioCore-getName-method">✔ `getName()` メソッド (デバイス名取得)</a>

toio コア キューブのデバイス名を返します。デバイス名は、toio コア キューブがアドバタイジングパケットにセットした `localName` の値です。通常は `"toio Core Cube"` 固定です。

#### プロトタイプ宣言

```c++
std::string getName();
```

#### 引数

なし

#### コードサンプル

```c++
std::string name = toiocore->getName();
Serial.println(name.c_str()); // 例 "toio Core Cube"
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
  Serial.println("- BLE 接続に失敗しました。");
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
  Serial.println("接続中");
} else {
  Serial.println("切断中");
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
#include <M5Stack.h>
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
    Serial.println(state ? "接続" : "切断");
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
Serial.println(ble_ver.c_str()); // 例 "2.1.0"
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
Serial.printf("%d パーセント\n", batt_level);
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
#include <M5Stack.h>
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
    Serial.printf("%d パーセント\n", level);
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
  Serial.println("ボタンが押されています。");
} else {
  Serial.println("ボタンは押されていません。");
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
#include <M5Stack.h>
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
    Serial.println(state ? "ボタン押下" : "ボタン解除");
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
Serial.println("- 水平検出: " + String(motion.flat ? "水平" : "水平でない"));
Serial.println("- 衝突検出: " + String(motion.clash ? "あり" : "なし"));
Serial.println("- ダブルタップ検出: " + String(motion.dtap ? "あり" : "なし"));
Serial.println("- 姿勢検出: " + String(motion.attitude));
```

### <a id="ToioCore-onMotion-method">✔ `onMotion()` メソッド (モーションセンサーのコールバックをセット)</a>

toio コア キューブのモーションイベントのコールバックをセットします。モーションに変化があれば、引数に指定したコールバック関数を呼び出します。コールバック関数にはモーションを表す構造体が引き渡されます。

#### プロトタイプ宣言

```c++
struct ToioCoreMotionData {
  bool flat;
  bool clash;
  bool dtap;
  uint8_t attitude;
};

typedef std::function<void(ToioCoreMotionData motion)> OnMotionCallback;
void onMotion(OnMotionCallback cb);
```

#### 引数

No. | 変数名   | 型                 | 必須   | 説明
:---|:--------|:-------------------|:-------|:-------------
1   | `cb`    | `OnMotionCallback` | ✔     | コールバック関数

#### コードサンプル

以下のサンプルスケッチは、モーションに変化があると、その都度、その状態を出力します。

コールバックを使う場合は、`.ino` ファイルの `loop()` 関数内で `Toio` オブジェクトの [`loop()`](#Toio-loop-method) メソッドを呼び出してください。コールバックは、`.ino` ファイルの `loop()` 関数が実行が開始されてから発生したイベントしかハンドリングできませんので注意してください。

```c++
#include <M5Stack.h>
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
    Serial.println("------------------------------------------");
    Serial.println("- 水平検出: " + String(motion.flat ? "水平" : "水平でない"));
    Serial.println("- 衝突検出: " + String(motion.clash ? "あり" : "なし"));
    Serial.println("- ダブルタップ検出: " + String(motion.dtap ? "あり" : "なし"));
    Serial.println("- 姿勢検出: " + String(motion.attitude));
  });
}

void loop() {
  // コールバックを使う場合には必ず Toio オブジェクトの loop() を呼び出す
  toio.loop();
}
```

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

要確認:目標地点5個は動いた。 29個っていける？


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
5   | `travel_direction` | `uint8_t`  | ✔ | モーターの速度変化タイプ (`0` 前進 ～ `1` 後退)
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
  uint8_t controlID; // 制御識別値 または 左モーターの速度
  uint8_t response;  // 応答内容 または右モーターの速度
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
#include <M5Stack.h>
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
    Serial.print("Motor Event ");
    Serial.print("Type=" + String(motor_res.controlType));
    if(motor_res.controlType == 0xe0 )
      Serial.println( ", left speed=" + String(motor_res.controlID) + ", right speed=" + String(motor_res.response));
    else
      Serial.println( ", controlID=" + String(motor_res.controlID) + ", response=" + String(motor_res.response));
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
  Serial.println("posX: " + String(data.position.cubePosX)
  + " posY: " + String(data.position.cubePosY)
  + " angle: " + String(data.position.cubeAngleDegree));
}
else if (data.type == ToioCoreIDTypeStandard) {
  Serial.println("Standard ID: " + String(data.standard.standardID));
}
else {
  Serial.println("no id found.");
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
#include <M5Stack.h>
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
      Serial.println("posX: " + String(data.position.cubePosX)
      + " posY: " + String(data.position.cubePosY)
      + " angle: " + String(data.position.cubeAngleDegree));
    }
    else if (data.type == ToioCoreIDTypeStandard) {
      Serial.println("Standard ID: " + String(data.standard.standardID));
    }
    else {
      Serial.println("no id found.");
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

本ライブラリのインストールが完了すると、Arduino IDE のメニューバーの `ファイル` -> `スケッチ例` の中から `M5StackToio` が選択できるようになります。この中には以下の 3 つのサンプルが用意されています。いずれも [M5Stack Basic](https://www.switch-science.com/catalog/3647/) および [M5Stack Gray](https://www.switch-science.com/catalog/3648/) で動作します。

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
  * 初版リリース

---------------------------------------
## <a id="References">リファレンス</a>

* [toio 公式ページ](https://toio.io/)
* [toio コア キューブ 技術仕様](https://toio.github.io/toio-spec/)
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
