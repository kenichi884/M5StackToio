/* ----------------------------------------------------------------
  m5stack-toio - basic_test.ino

  Copyright (c) 2020 Futomi Hatano. All right reserved.
  https://github.com/futomi
  Toio ID read support   https://github.com/mhama
  Protocol v2.3.0 or later support  https://github.com/kenichi884 

  --------------------------------------------------------------
  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  This sample sketch was tested on M5Stack Atom S3, Atom S3 lite, and M5Capsule.
  M5Unified, so it should also work with other M5Stack controllers with more than one button.
  This sample sketch uses M5Unified, so it should also work with other M5Stack controllers that have more than one button.
  M5Unified M5.Log.printf() is used for operation log. By default, operation log appears on the serial port.

  [How to use]

  Turn on Toio Core Cube beforehand.

  When M5Stack starts, it automatically scans Toio Core Cube.
  If the Toio Core Cube is found, the device name and MAC address will appear in the M5Stack serial port log.
  will appear in the M5Stack serial port log. If it is not found, press the reset button on the M5Stack button
  If the Toio Core Cube is not found, press the Reset button on the M5Stack button again to reboot.

  Once the Toio Core Cube is found, it will automatically make a BLE connection and
  The Toio Core Cube will automatically connect to the BLE and execute a series of methods one after the other, displaying the results in the M5Stack's serial port log.

  When the series of methods is completed, the BLE connection is automatically disconnected and terminated.

  --------------------------------------------------------------

  このサンプルスケッチは M5Stack Atom S3、Atom S3 lite、M5Capsuleで動作確認しました。
  M5Unifiedを使っているので、ほかのボタンが一つ以上あるM5Stack製 Controllerでも
  動作すると思います。
  動作ログはM5UnifiedのM5.Log.printf()を使っています。デフォルトではシリアルポートに出ます。

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

#define TEST_SOUND 1
#define TEST_BATTERY 1
#define TEST_LED 1
#define TEST_BUTTON 1
#define TEST_MOTION_SENSOR 1
#define TEST_MOTOR 1

// Declare a toio object
// Toio オブジェクト生成
Toio toio;

void setup() {
  M5.begin();
  M5.Power.begin();

  M5.Log.printf("M5StackToio basic_test\n");

  // Scan Toio Core Cubes in 3 seconds.
  // 3 秒間 Toio Core Cube をスキャン
  M5.Log.printf("Scanning your toio core...\n");
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  
  // Exit if could not found any cubes.
  // 見つからなければ終了
  size_t n = toiocore_list.size();
  if (n == 0) {
    M5.Log.printf("No toio Core Cube was found. Turn on your Toio Core Cube, then press the reset button of your Toio Core Cube.\n");
    return;
  }

  // Assign the ToioCore object of the first Toio Core Cube found.
  // 最初に見つかった Toio Core Cube の ToioCore オブジェクト
  ToioCore* toiocore = toiocore_list.at(0);
  M5.Log.printf("Your toio core was found:      \n");

  // Print the Toio Core cube's device name and MAC address.
  // Toio Core のデバイス名と MAC アドレスを表示
  M5.Log.printf("%s (%s)\n", toiocore->getName().c_str(), toiocore->getAddress().c_str());

  // Start BLE connection.
  // BLE 接続開始
  M5.Log.printf("Connecting...\n");

  if (!toiocore->connect()) {
    M5.Log.printf("Failed to establish a BLE connection.\n");
    return;
  }
  M5.Log.printf("Connected.\n");
  delay(3000);

  // Get the BLE protocol version of the Toio core cube.
  // BLE プロトコルバージョンを取得
  std::string ble_ver = toiocore->getBleProtocolVersion();
  M5.Log.printf("Getting the BLE protocol version... %s\n", ble_ver.c_str());
  delay(3000);

#if TEST_SOUND
  // Play the MIDI note numbers (Charmera, Musical instruments used at traditional Japanese ramen stalls.)
  // MIDI を再生 (チャルメラ)
  M5.Log.printf("Play a MIDI...\n");
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

  // Play sound effects (11 patterns)
  // 効果音を再生 (11 パターン)
  M5.Log.printf("Play sound effects: Enter\n");
  toiocore->playSoundEffect(0);
  delay(3000);

  M5.Log.printf("Play sound effects: Selected\n");
  toiocore->playSoundEffect(1);
  delay(3000);

  M5.Log.printf("Play sound effects: Cancel\n");
  toiocore->playSoundEffect(2);
  delay(3000);

  M5.Log.printf("Play sound effects: Cursor\n");
  toiocore->playSoundEffect(3);
  delay(3000);

  M5.Log.printf("Play sound effects: Mat in\n");
  toiocore->playSoundEffect(4);
  delay(3000);

  M5.Log.printf("Play sound effects: Mat out\n");
  toiocore->playSoundEffect(5);
  delay(3000);

  M5.Log.printf("Play sound effects: Get 1\n");
  toiocore->playSoundEffect(6);
  delay(3000);

  M5.Log.printf("Play sound effects: Get 2\n");
  toiocore->playSoundEffect(7);
  delay(3000);

  M5.Log.printf("Play sound effects: Get 3\n");
  toiocore->playSoundEffect(8);
  delay(3000);

  M5.Log.printf("Play sound effects: Effect 1\n");
  toiocore->playSoundEffect(9);
  delay(3000);

  M5.Log.printf("Play sound effects: Effect 2\n");
  toiocore->playSoundEffect(10);
  delay(3000);
#endif // TEST_SOUND

#if TEST_LED
  // Turn the indicator on.(yellow)
  // LED を黄色で点灯
  M5.Log.printf("Turn on the LED: yellow\n");
  toiocore->turnOnLed(0xff, 0xff, 0x00);
  delay(5000);

  // Turn the indicator off.
  // LED を消灯
  M5.Log.printf("Turn off the LED\n");
  toiocore->turnOffLed();
  delay(5000);
#endif // TEST_LED

#if TEST_BATTERY
  // Get the battery level.
  // バッテリーレベルを取得
  M5.Log.printf("Getting the battery level...\n");
  uint8_t batt_level = toiocore->getBatteryLevel();
  M5.Log.printf(" %u%%\n", batt_level);
  delay(5000);
#endif // TEST_BATTERY

#if TEST_BUTTON
  // Get the button state.
  // ボタン押下状態を取得
  M5.Log.printf("Getting the button state...\n");
  bool button_state = toiocore->getButtonState();
  M5.Log.printf(" %u\n", button_state);
  delay(5000);
#endif // TEST_BUTTON

#if TEST_MOTION_SENSOR
  // Get the motion detection information.
  // モーションセンサーの状態を取得
  M5.Log.printf("Getting the state of the motion sensor... \n");
  ToioCoreMotionData motion = toiocore->getMotion();
  M5.Log.printf("flat=%u, clash=%u, dtap=%u, attitude=%u\n",
    motion.flat, motion.clash, motion.dtap, motion.attitude);
  delay(5000);
#endif // TEST_MOTION_SENSOR

#if TEST_MOTOR
  // Proceed while turning to the right. (Control left and right motors individually.)
  // 右に曲がりながら進む (左右のモーターを個別に制御)
  M5.Log.printf("Turning to the right...\n");
  toiocore->controlMotor(true, 50, true, 40);
  delay(5000);

  // Stop the motors.
  // 停止
  M5.Log.printf("Stopping...\n");
  toiocore->controlMotor(true, 0, true, 0);
  delay(5000);

  // Proceed while turning left for only 2 seconds (Control left and right motors individually.)
  // 2 秒間だけ左に曲がりながら進む (左右のモーターを個別に制御)
  M5.Log.printf("Turning to the left for 2 seconds\n");
  toiocore->controlMotor(true, 40, true, 50, 2000);
  delay(5000);

  // Move to the specified target.
  // 目標指定付き移動
  M5.Log.printf("Move to (150, 200)\n");
  toiocore->controlMotorWithTarget(0, 0, 0, 100, 0, 150, 200, 0);
  delay(5000);

  // Move to the specified multiple targets.
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
  M5.Log.printf("Move to (250, 250) (250, 300) (300, 300)\n");
  toiocore->controlMotorWithMultipleTargets(0, 0, 0, 50, 0, 0, 5, pos);
  delay(5000);

  // Move with acceleration specified.
  // 加速度指定付き移動
  M5.Log.printf("Acceleration Control\n");
  toiocore->controlMotorWithAcceleration(50, 15, 30, 0, 0, 0, 200);
  delay(5000);
#endif // TEST_MOTOR

  // Print free heap memory size.
  // 空きヒープメモリサイズの確認
  //M5.Log.printf("esp_get_free_heap_size(): %6d\n", esp_get_free_heap_size() );

  // Disconnect the BLE connection.
  // BLE 切断
  toiocore->disconnect();
  M5.Log.printf("Disconnected\n");
}

void loop() {

}