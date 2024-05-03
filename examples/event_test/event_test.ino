/* ----------------------------------------------------------------
  m5stack-toio - event_test.ino

  Copyright (c) 2020 Futomi Hatano. All right reserved.
  https://github.com/futomi
  Toio ID read support   https://github.com/mhama
  Protocol v2.3.0 or later support  https://github.com/kenichi884 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  --------------------------------------------------------------
  This sample sketch was tested on M5Stack Atom S3, Atom S3 lite, and M5Capsule.
  M5Unified, so it should also work with other M5Stack controllers with more than one button.
  This sample sketch uses M5Unified, so it should also work with other M5Stack controllers that have more than one button.
  M5Unified M5.Log.printf() is used for operation log. By default, operation log appears on the serial port.
  
  It seems that arduino ESP32 can only register up to 4 BLE notifications.
  Therefore, it is not possible to receive all the notifications (events) of Toio Core Cube at the same time.
  Please set only (up to 4) necessary notifications (callbacks).

  [How to use]

  Turn on Toio Core Cube in advance.

  When M5Stack starts up, it automatically scans Toio Core Cube.
  If Toio Core Cube is found, the device name and MAC address will be displayed in the serial port log.
  will be displayed in the serial port log. If it is not found, press the reset button on the M5Stack button
  If the Toio Core Cube is not found, press the reset button on the M5Stack button again to reboot.

  Once the connection is established, the M5Stack listens for various events, and sends the received event information to the serial port log.
  The received event information is displayed in the serial port log.

  After the Toio Core Cube is connected, press the A button on the M5Stack to move the Toio Core Cube to the specified position on the
  The Toio Core Cube will move to the specified position on the mat.
  Pressing the A button for more than 2 seconds disconnects or reconnects the BLE connection to the Toio Core Cube.
  --------------------------------------------------------------

  このサンプルスケッチは M5Stack Atom S3、Atom S3 lite、M5Capsuleで動作確認しました。
  M5Unifiedを使っているので、ほかのボタンが一つ以上あるM5Stack製 Controllerでも
  動作すると思います。
  動作ログはM5UnifiedのM5.Log.printf()を使っています。デフォルトではシリアルポートに出ます。

  arduino ESP32ではBLE notificationの登録が4つまでに限られるようです。
  そのため、Toio Core Cubeのすべてのnotify(イベント)を同時にうけることができません。
  必要なnotify(のコールバック)のみ(4つまで)セットするようにしてください。

  [使い方]

  事前に Toio Core Cube の電源を入れてください。

  M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
  Toio Core Cube が見つかるとシリアルポートのログにデバイス名と MAC アドレス
  が表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを
  押して再起動してください。

  接続が完了すると、各種イベントを待ち受け、受信し
  たイベント情報を シリアルポートのログに表示します。

  Toio Core Cube 接続された後、M5Stack の A ボタン押すと、Toio Core Cubeが
  マットの指定位置に移動します。
  Aボタンを2秒以上押すと Toio Core Cube との BLE 接続を遮断または再接続します。
  -------------------------------------------------------------- */
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
// Toio オブジェクト生成
Toio toio;

// Decreare a pointer variable for the discoverd ToioCore object.
// 発見された ToioCore オブジェクトのポインタ変数を定義
ToioCore* toiocore;

void setup() {
  M5.begin();

  M5.Log.printf("M5StackToio event_test\n");

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
  toiocore = toiocore_list.at(0);
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
  
  // Set Connection event callback.
  // Connection イベントのコールバックをセット
  toiocore->onConnection([](bool state) {
    M5.Log.printf("Connection Event %s\n", state ? "Connected   " : "Disconnected");
  });

  // BLE notifications registration seems to be limited up to 4, on arduino ESP32.
  // https://github.com/espressif/arduino-esp32/issues/8015
  // The following event callback sets can only be set to a total of four.
  // arduino ESP32では4つまでしかBLE notificationをregisterForNotify()できない
  // https://github.com/espressif/arduino-esp32/issues/8015
  // 以下のイベントコールバックのセットも合計4つまでしかセットできない。

  // With NimBLE, It seems to be able to set 5 notifications.
  // NimBLEの場合は5つイベントコールバックをセットできるようだ。
#define USE_BUTTON_EVENT 1
#define USE_BATTERY_EVENT 0
#define USE_MOTION_EVENT 1
#define USE_MOTOR_EVENT 1
#define USE_ID_READER_EVENT 1


#if USE_BUTTON_EVENT
  // Set Button evnet callback.
  // Button イベントのコールバックをセット
  toiocore->onButton([](bool state) {
    M5.Log.printf("Button Event %s\n", state ? "Pressed " : "Released");
  });
#endif

#if USE_BATTERY_EVENT
  // Set Battery event callback.
  // Battery イベントのコールバックをセット
  toiocore->onBattery([](uint8_t level) {
    M5.Log.printf("Battery Event %u %  \n", level);
  });
#endif 

#if USE_MOTION_EVENT
  // Set Motion event callback.
  // Motion イベントのコールバックをセット
  toiocore->onMotion([](ToioCoreMotionData motion) {
    M5.Log.printf("Motion Event flat=%u, clash=%u, dtap=%u, attitude=%u, shake=%u\n",
      motion.flat, motion.clash, motion.dtap, motion.attitude, motion.shake);
  }); 
#endif 

#if USE_MOTOR_EVENT
  // Set Motor event Calback.
  // Motor イベントのコールバックをセット
  toiocore->onMotor([](ToioCoreMotorResponse motor_res) {
    M5.Log.printf("Motor Event Type=%u", motor_res.controlType);
    if(motor_res.controlType == 0xe0)
      M5.Log.printf( ", left speed=%u, right speed=%u\n", motor_res.controlID, motor_res.response);
    else
      M5.Log.printf( ", controlID=%u, response=%u\n", motor_res.controlID, motor_res.response);
  });   
#endif

#if USE_ID_READER_EVENT
  // Set ID reader callback.
  // ID Reader のコールバックをセット
  toiocore->onIDReaderData([](ToioCoreIDData id_data){
    /*
    M5.Log.printf("IDReader Event ");
    if (id_data.type == ToioCoreIDTypePosition) {
      M5.Log.printf("posX: %u posY: %u angle: %u\n",
        id_data.position.cubePosX, id_data.position.cubePosY, id_data.position.cubeAngleDegree);
    }
    else if (id_data.type == ToioCoreIDTypeStandard) {
      M5.Log.printf("Standard ID: %u\n", id_data.standard.standardID);
    }
    else {
      M5.Log.printf("no id found.\n");
    }
    */
  });
 #endif

  // Print free heap memory size.
  //空きヒープメモリサイズの確認
  // M5.Log.printf("esp_get_free_heap_size(): %6d\n", esp_get_free_heap_size() );

}

void loop() {
  M5.update();
  
  // If you want to handle event callbacks, you shoud call loop() method  of Toio Object here.
  // イベントを扱う場合は、必ずここで Toio オブジェクトの
  // loop() メソッドを呼び出すこと
  toio.loop();
  
  // M5Stack のボタン A が2秒長押しされたときの処理
  // When button A of M5Stack is pressed for 2 seconds
  if (M5.BtnA.wasReleaseFor(2000)) {
    // Check the Toio Core Cube was connected or disconnected.
    // Toio Core Cube と BLE 接続中かどうかをチェック
    if (toiocore->isConnected()) {
      M5.Log.printf("try disconnect\n");
      toiocore->disconnect(); // If connected, disconnect. 接続中なら切断
    } else {
      M5.Log.printf("try connect\n");
      toiocore->connect(); // If not connected, connect. 接続していないなら接続
    }
  // When button A of M5Stack was pressed.
  // M5Stack のボタン A が押されたときの処理
  } else if(M5.BtnA.wasReleased()) {
    M5.Log.printf("start motor\n");
    // Move with multiple targets, to test Motor event callback will work.
    // 目標指定付き移動  移動の終了イベント(Motor イベントのコールバック)がくるかどうかのテスト用
    M5.Log.printf("Move to (150, 200)\n");
    toiocore->controlMotorWithTarget(1, 5, 0, 80, 0, 150, 200, 0);
  }

  yield();

}