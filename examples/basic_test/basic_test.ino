/* ----------------------------------------------------------------
  m5stack-toio - basic_test.ino

  Copyright (c) 2020 Futomi Hatano. All right reserved.
  https://github.com/futomi
  Toio ID read support   https://github.com/mhama
  Protocol v2.3.0 support  https://github.com/kenichi884 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  このサンプルスケッチは M5Stack Atom S3、Atom S3 lite、M5Capsuleで動作確認しました。
  M5Unifiedを使っているので、ほかのボタンが一つ以上あるM5Stack製 Controllerでも
  動作すると思います。
  動作ログはシリアルポートにprintln()しています。ESP32-S3以外の場合は
  #define SERIAL_LOGの定義をUSBSerialからSerialに変更してください。

  [使い方]

  事前に Toio Core Cube の電源を入れてください。

  M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
  Toio Core Cube が見つかると M5Stack シリアルポートのログにデバイス名と MAC アドレス
  が表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを
  押して再起動してください。

  Toio Core Cube が発見されると、自動的に BLE 接続し、一連のメソッドを
  順次実行しながら M5Stack のシリアルポートのログに結果を表示します。

  一連のメソッドの実行が完了すると、自動的に BLE 接続を切断して終了します。
  -------------------------------------------------------------- */
#include <M5Unified.h>
#include <Toio.h>

// for ESP32-S3 model
#define SERIAL_LOG USBSerial
// #define SERIAL_LOG Serial

#define TEST_SOUND 1
#define TEST_BATTERY 1
#define TEST_LED 1
#define TEST_BUTTON 1
#define TEST_MOTION_SENSOR 1
#define TEST_MOTOR 1

// Toio オブジェクト生成
Toio toio;

void setup() {
  M5.begin();
  M5.Power.begin();
  SERIAL_LOG.begin();
  SERIAL_LOG.println("M5StackToio");

  // 3 秒間 Toio Core Cube をスキャン
  SERIAL_LOG.println("Scanning your toio core...");
  std::vector<ToioCore*> toiocore_list = toio.scan(3);

  // 見つからなければ終了
  size_t n = toiocore_list.size();
  if (n == 0) {
    SERIAL_LOG.println("No toio Core Cube was found. Turn on your Toio Core Cube, then press the reset button of your Toio Core Cube.");
    return;
  }

  // 最初に見つかった Toio Core Cube の ToioCore オブジェクト
  ToioCore* toiocore = toiocore_list.at(0);
  SERIAL_LOG.println("Your toio core was found:      ");

  // Toio Core のデバイス名と MAC アドレスを表示
  SERIAL_LOG.println(String(toiocore->getName().c_str()) + " (" + String(toiocore->getAddress().c_str()) + ")");

  // BLE 接続開始
  SERIAL_LOG.println("Connecting...");

  if (!toiocore->connect()) {
    SERIAL_LOG.println("Failed to establish a BLE connection.");
    return;
  }
  SERIAL_LOG.println("Connected.");
  delay(3000);

  // BLE プロトコルバージョンを取得
  SERIAL_LOG.println("Getting the BLE protocol version... ");
  std::string ble_ver = toiocore->getBleProtocolVersion();
  SERIAL_LOG.println(ble_ver.c_str());
  delay(3000);

#if TEST_SOUND
  // MIDI を再生 (チャルメラ)
  SERIAL_LOG.println("Play a MIDI...");
  uint8_t charumera_data[39] = {
    3,             // Type of Sound control (MIDI)
    1,             // Repeat count
    12,            // Number of operations
    14,  69, 255,  // 140ms, A5
    14,  71, 255,  // 140ms, B5
    56,  73, 255,  // 560ms, C#6
    14,  71, 255,  // 140ms, B5
    14,  69, 255,  // 140ms, A5
    114, 128, 255, // no sound
    14,  69, 255,  // 140ms, A5
    14,  71, 255,  // 140ms, B5
    14,  73, 255,  // 560ms, C#6
    14,  71, 255,  // 140ms, B5
    14,  69, 255,  // 140ms, A5
    56,  71, 255   // 560ms, B5
  };
  toiocore->playSoundRaw(charumera_data, 39);
  delay(5000);

  // 効果音を再生 (11 パターン)
  SERIAL_LOG.println("Play sound effects: Enter");
  toiocore->playSoundEffect(0);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Selected");
  toiocore->playSoundEffect(1);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Cancel");
  toiocore->playSoundEffect(2);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Cursor");
  toiocore->playSoundEffect(3);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Mat in");
  toiocore->playSoundEffect(4);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Mat out");
  toiocore->playSoundEffect(5);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Get 1");
  toiocore->playSoundEffect(6);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Get 2");
  toiocore->playSoundEffect(7);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Get 3");
  toiocore->playSoundEffect(8);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Effect 1");
  toiocore->playSoundEffect(9);
  delay(3000);

  SERIAL_LOG.println("Play sound effects: Effect 2");
  toiocore->playSoundEffect(10);
  delay(3000);
#endif // TEST_SOUND

#if TEST_LED
  // LED を黄色で点灯
  SERIAL_LOG.println("Turn on the LED: yellow");
  toiocore->turnOnLed(0xff, 0xff, 0x00);
  delay(5000);

  // LED を消灯
  SERIAL_LOG.println("Turn off the LED");
  toiocore->turnOffLed();
  delay(5000);
#endif // TEST_LED

#if TEST_BATTERY
  // バッテリーレベルを取得
  SERIAL_LOG.println("Getting the battery level...");
  uint8_t batt_level = toiocore->getBatteryLevel();
  SERIAL_LOG.println(String(batt_level) + "%");
  delay(5000);
#endif // TEST_BATTERY

#if TEST_BUTTON
  // ボタン押下状態を取得
  SERIAL_LOG.println("Getting the button state...");
  bool button_state = toiocore->getButtonState();
  SERIAL_LOG.println(String(button_state));
  delay(5000);
#endif // TEST_BUTTON

#if TEST_MOTION_SENSOR
  // モーションセンサーの状態を取得
  SERIAL_LOG.println("Getting the state of the motion sensor... ");
  ToioCoreMotionData motion = toiocore->getMotion();
  SERIAL_LOG.println(
    "flat=" + String(motion.flat) +
    ", clash=" + String(motion.clash) +
    ", dtap=" + String(motion.dtap) +
    ", attitude=" + String(motion.attitude));
  delay(5000);
#endif // TEST_MOTION_SENSOR

#if TEST_MOTOR
  // 右に曲がりながら進む (左右のモーターを個別に制御)
  SERIAL_LOG.println("Turning to the right...");
  toiocore->controlMotor(true, 50, true, 40);
  delay(5000);

  // 停止
  SERIAL_LOG.println("Stopping...");
  toiocore->controlMotor(true, 0, true, 0);
  delay(5000);

  // 2 秒間だけ左に曲がりながら進む (左右のモーターを個別に制御)
  SERIAL_LOG.println("Turning to the left for 2 seconds");
  toiocore->controlMotor(true, 40, true, 50, 2000);
  delay(5000);

  // 目標指定付き移動
  SERIAL_LOG.println("Move to (150, 200)");
  toiocore->controlMotorWithTarget(0, 0, 0, 100, 0, 150, 200, 0);
  delay(5000);

  // 複数目標指定付き移動
  ToioCoreTargetPos pos[5];
  pos[0].posX = 250;
  pos[0].posY = 250;
  pos[0].angleDegree = 0;
  pos[0].angleAndRotation = 0;
  pos[1].posX = 250;
  pos[1].posY = 300;
  pos[1].angleDegree = 90;
  pos[1].angleAndRotation = 0;
  pos[2].posX = 300;
  pos[2].posY = 300;
  pos[2].angleDegree = 180;
  pos[2].angleAndRotation = 0;
  pos[3].posX = 300;
  pos[3].posY = 250;
  pos[3].angleDegree = 270;
  pos[3].angleAndRotation = 0;
  pos[4].posX = 250;
  pos[4].posY = 250;
  pos[4].angleDegree = 0;
  pos[4].angleAndRotation = 0;
  SERIAL_LOG.println("Move to (250, 250) (250, 300) (300, 300)");
  toiocore->controlMotorWithMultipleTargets(0, 0, 0, 50, 0, 0, 5, pos);
  delay(5000);

  // 加速度指定付き移動
  SERIAL_LOG.println("Acceleration Control");
  toiocore->controlMotorWithAcceleration(50, 15, 30, 0, 0, 0, 200);
  delay(5000);
#endif // TEST_MOTOR

  // BLE 切断
  toiocore->disconnect();
  SERIAL_LOG.println("Disconnected");
}

void loop() {

}