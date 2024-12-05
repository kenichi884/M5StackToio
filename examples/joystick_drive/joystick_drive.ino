/* ----------------------------------------------------------------
  m5stack-toio - event.ino

  Copyright (c) 2020 Futomi Hatano. All right reserved.
  https://github.com/futomi

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  このサンプルスケッチは M5Stack Basic Core または GREY で動作します。
  また M5Stack 用ジョイスティックユニットが必要です。
  https://www.switch-science.com/catalog/4050/
  https://docs.m5stack.com/#/en/unit/joystick

  [使い方]

  事前に Toio Core Cube の電源を入れてください。

  M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
  Toio Core Cube が見つかると M5Stack の画面にデバイス名と MAC アドレス
  が表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを
  押して再起動してください。

  Toio Core Cube と BLE 接続する前にジョイスティックをキャリブレーション
  する必要があります。M5Stack の画面上にジョイスティックの座標が表示されま
  す。もし x と y の値が 0 でなければ、ジョイスティックの中心座標がずれて
  います。M5Stack の C ボタンを押してキャリブレーションしてください。

  Toio Core Cube が発見された後、M5Stack の A ボタンを押すと Toio Core
  Cube と BLE 接続します。接続が完了すると、ジョイスティックで Toio Core
  Cute を運転することができます。

  BLE 接続中、ジョイスティックの z 軸を押すと、チャルメロが再生されます。
  また、M5Stack の B ボタンを押すと、Toio Core Cube の LED が白で点灯し
  ます。
  -------------------------------------------------------------- */
#include <M5Stack.h>
#include <Wire.h>
#include <Toio.h>

// ライト ON/OFF 状態
static bool light_on = false;

// ジョイスティックの z 軸 ボタン状態
static uint8_t joystick_button_state = 0;

// ジョイスティックの x/y 軸のキャリブレーション
static uint8_t carib_x = 130;
static uint8_t carib_y = 130;

// MIDI データ (チャルメラ)
static const uint8_t CHARUMERA_LEN = 39;
static uint8_t CHARUMERA_DATA[CHARUMERA_LEN] = {
  3,              // Type of Sound control (MIDI)
  1,              // Repeat count
  12,             // Number of operations
  14, 69, 255,    // 140ms, A5
  14, 71, 255,    // 140ms, B5
  56, 73, 255,    // 560ms, C#6
  14, 71, 255,    // 140ms, B5
  14, 69, 255,    // 140ms, A5
  114, 128, 255,  // no sound
  14, 69, 255,    // 140ms, A5
  14, 71, 255,    // 140ms, B5
  14, 73, 255,    // 560ms, C#6
  14, 71, 255,    // 140ms, B5
  14, 69, 255,    // 140ms, A5
  56, 71, 255     // 560ms, B5
};

// Joystickのモード
static const int addresses[] = {
  0x5e,  // Faces Joystick
  0x63,  // Joystick2 Unit
  0x52   // Joystick Unit
};

enum joystick_num {
  faces = 0,
  joystick2,
  joystick
};
int joystick_mode = -1;

void displayCaptionButtonA(String caption) {
  M5.Lcd.setCursor(30, 215, 2);
  M5.Lcd.print("[" + caption + "]");
}

void displayCaptionButtonB(String caption) {
  M5.Lcd.setCursor(125, 215, 2);
  M5.Lcd.print("[" + caption + "]");
}

void displayCaptionButtonC(String caption) {
  M5.Lcd.setCursor(220, 215, 2);
  M5.Lcd.print("[" + caption + "]");
}

void drawJoystick(float jx, float jy) {
  M5.Lcd.setCursor(70, 130, 2);
  M5.Lcd.printf("X : %f  ", jx);
  M5.Lcd.setCursor(70, 150, 2);
  M5.Lcd.printf("Y : %f  ", jy);
  M5.Lcd.drawCircle(190, 150, 50, 0xffffff);
  M5.Lcd.fillCircle(190, 150, 49, 0x000000);
  M5.Lcd.drawLine(140, 150, 240, 150, 0xffffff);
  M5.Lcd.drawLine(190, 100, 190, 200, 0xffffff);
  uint8_t x = (37 * jx / 100.0) + 190;
  uint8_t y = -(37 * jy / 100.0) + 150;
  M5.Lcd.fillCircle(x, y, 5, 0xffffff);
}

// Toio オブジェクト生成
Toio toio;

// 発見された ToioCore オブジェクトのポインタ変数を定義
ToioCore* toiocore = nullptr;

void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.clear();
  dacWrite(25, 0);

  // タイトル表示
  M5.Lcd.setCursor(85, 5, 4);
  M5.Lcd.print("M5StackToio");

  // ジョイスティック利用を開始
  Wire.begin();

  // I2Cアドレスにデバイスが存在するか確認する
  byte error;
  for (int i = 0; i < sizeof(addresses); i++) {
    Wire.beginTransmission(addresses[i]);
    error = Wire.endTransmission();
    if (error == 0) {
      joystick_mode = i;
      break;
    }
  }

  // 3 秒間 Toio Core Cube をスキャン
  M5.Lcd.setCursor(0, 50, 2);
  M5.Lcd.print("Scanning your toio core...");
  std::vector<ToioCore*> toiocore_list = toio.scan(3);

  // 見つからなければ終了
  size_t n = toiocore_list.size();
  if (n == 0) {
    M5.Lcd.setCursor(0, 50, 2);
    M5.Lcd.print("No toio Core Cube was found. Turn on your Toio Core Cube, then press the reset button of your Toio Core Cube.");
    return;
  }

  // 最初に見つかった Toio Core Cube の ToioCore オブジェクト
  toiocore = toiocore_list.at(0);

  // Toio　Core のデバイス名と MAC アドレスを表示
  M5.Lcd.setCursor(0, 50, 2);
  M5.Lcd.print(String(toiocore->getName().c_str()) + " (" + String(toiocore->getAddress().c_str()) + ")");

  // M5Stack のボタン A/B/C のキャプションを表示
  displayCaptionButtonA(" Connect  ");
  displayCaptionButtonB("  Light   ");
  displayCaptionButtonC("  Calib   ");

  // Connection イベントのコールバックをセット
  toiocore->onConnection([](bool state) {
    displayCaptionButtonA(state ? "Disconnect" : " Connect  ");
  });
}

void loop() {
  float x, y;
  uint8_t z;
  float jx, jy;

  M5.update();

  // ジョイスティックが接続されているかをチェックしジョイスティックの状態を取得
  switch (joystick_mode) {
    case faces:
      Wire.requestFrom(addresses[joystick_mode], 5);
      if (!Wire.available()) {
        return;
      }
      {
        uint8_t adX_L = Wire.read();
        uint8_t adX_H = Wire.read();
        uint8_t adY_L = Wire.read();
        uint8_t adY_H = Wire.read();
        z = Wire.read();

        y = map((float)(adX_H << 8 | adX_L), 250.0, 800.0, -100.0, 100.0) + 130.0;
        x = map((float)(adY_H << 8 | adY_L), 250.0, 800.0, -100.0, 100.0) + 130.0;
      }
      break;
    case joystick2:
      // x,yの状態を8ビットで取得
      Wire.beginTransmission(addresses[joystick_mode]);
      Wire.write(0x10);
      Wire.endTransmission(false);
      Wire.requestFrom(addresses[joystick_mode], 2);
      x = (float)Wire.read();
      y = (float)Wire.read();

      /// ボタンの状態を取得
      Wire.beginTransmission(addresses[joystick_mode]);
      Wire.write(0x20);
      Wire.endTransmission(false);
      z = Wire.read();
      break;
    case joystick:
      Wire.requestFrom(addresses[joystick_mode], 3);
      if (!Wire.available()) {
        return;
      }
      x = (float)Wire.read();
      y = (float)Wire.read();
      z = Wire.read();
      break;
    default:
      break;
  }

  // イベントを扱う場合は、必ずここで Toio オブジェクトの
  // loop() メソッドを呼び出すこと
  toio.loop();

  // M5Stack のボタン C が押されたときの処理 (Carib)
  if (M5.BtnC.wasPressed()) {
    // ジョイスティックのキャリブレーション
    carib_x = x;
    carib_y = y;
  }

  // ジョイスティックの x/y 軸の位置を特定 (キャリブレーション済み)
  switch (joystick_mode) {
    case faces:
      jy = ((float)y - (float)carib_y);
      jx = ((float)x - (float)carib_x);
      break;
    case joystick2:
      jy = ((float)y - (float)carib_y);
      jx = ((float)x - (float)carib_x);
      break;
    case joystick:
      jy = ((float)y - (float)carib_y) * 100.0 / 120.0;
      jx = ((float)x - (float)carib_x) * 100.0 / 120.0;
      break;
  }
  // ジョイスティックの状態を画面に表示
  drawJoystick(jx, jy);

  // Toio core Cube が見つかっていなければ loop () を抜ける
  if (!toiocore) {
    return;
  }

  // M5Stack のボタン A が押されたときの処理 (接続/切断)
  if (M5.BtnA.wasReleased()) {
    // Toio Core Cube と BLE 接続中かどうかをチェック
    if (toiocore->isConnected()) {
      toiocore->disconnect();  // 接続中なら切断
    } else {
      toiocore->connect();  // 切断中なら接続
    }
    return;
  }

  // Toio core Cube が未接続なら loop () を抜ける
  if (!toiocore->isConnected()) {
    return;
  }

  // M5Stack のボタン B が押されたときの処理 (Light)
  if (M5.BtnB.wasPressed()) {
    if (light_on) {
      toiocore->turnOffLed();  // 点灯中なら消灯
    } else {
      toiocore->turnOnLed(255, 255, 255);  // 消灯中なら白で点灯
    }
    light_on = !light_on;
  }

  // z 軸が押された場合、チャルメラを再生
  if (joystick_button_state == 0 && z == 1) {
    toiocore->playSoundRaw(CHARUMERA_DATA, CHARUMERA_LEN);
  }
  joystick_button_state = z;

  // ジョイスティックの動きに合わせて Toio Core Cube のモーターを制御

  // スロットル
  int8_t throttle = jy;
  if (throttle > 100) {
    throttle = 100;
  }
  if (throttle < -100) {
    throttle = -100;
  }
  // 最大速度が速すぎて操作しづらいので値を 2/3 にする
  throttle = (float)throttle * 2 / 3;

  // ステアリング
  int8_t steering = (abs(jx) < 10) ? 0 : jx;  // 10 ほど遊びを入れる
  if (steering > 100) {
    steering = 100;
  }
  if (steering < -100) {
    steering = -100;
  }
  // 左右の動きの反応が敏感すぎるので値を 2/3 にする
  steering = (float)steering * 2 / 3;

  toiocore->drive(throttle, steering);
}
