/* ----------------------------------------------------------------
  m5stack-toio - event2_test.ino

  Copyright (c) 2020 Futomi Hatano. All right reserved.
  https://github.com/futomi
  Toio ID read support   https://github.com/mhama
  Protocol v2.3.0 or later support  https://github.com/kenichi884 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  --------------------------------------------------------------
  This sample sketch tests features that are off by default (changing the ID notification interval, using magnetic sensors,
  attitude angle notifications, and motor speed notifications) that are off by default.
  This sample sketch was tested on M5Stack Atom S3, Atom S3 lite, and M5Capsule.
  This sample sketch uses M5Unified, so it should also work with other M5Stack controllers that have more than one button.
  M5Unified M5.Log.printf() is used for operation log. By default, operation log appears on the serial port.

  [How to use]

  Turn on Toio Core Cube beforehand.

  When M5Stack starts, it automatically scans Toio Core Cube.
  If the Toio Core Cube is found, the device name and MAC address will appear in the serial port log.
  will be displayed in the serial port log. If it is not found, press the reset button on the M5Stack button
  If it is not found, press the reset button on the M5Stack button again to reboot.

  Once the connection is made, you will be able to use functions that are off by default (changing the ID notification interval, using the magnetic sensor, changing the attitude angle notification, and changing the motor speed),
  Attitude Angle Notification, Motor Speed Notification) are turned on, and the M5Stack listens for each event and logs the received event information in the serial port log.
  The default is to turn on the off functions (change ID notification interval, use of magnetic sensors, attitude angle notification, motor speed notification), listen for each event, and display the received event information in the serial port log.

  After the Toio Core Cube is connected, pressing the A button on the M5Stack will cause the Toio Core Cube to
  The Toio Core Cube moves under the control of a motor with a specified acceleration. (You will be notified of the change in speed.)
  Pressing the A button for more than 2 seconds disconnects or reconnects the BLE connection with the Toio Core Cube.

  The arduino ESP32 seems to be limited to registering up to four BLE notifications.
  Therefore, it is not possible to receive all the notifications (events) of Toio Core Cube at the same time.
  Please set only (up to 4) necessary notifications (callbacks).
  (Motion, magnetism, and attitude angle use the same characteristic, so they count as one.)

 --------------------------------------------------------------
 
  このサンプルスケッチはデフォルトではオフの機能(IDの通知間隔の変更、磁気センサーの使用、
  姿勢角度の通知、モーター速度の通知)をテストします。
  このサンプルスケッチは M5Stack Atom S3、Atom S3 lite、M5Capsuleで動作確認しました。
  M5Unifiedを使っているので、ほかのボタンが一つ以上あるM5Stack製 Controllerでも
  動作すると思います。
  動作ログはM5UnifiedのM5.Log.printf()を使っています。デフォルトではシリアルポートに出ます。

  [使い方]

  事前に Toio Core Cube の電源を入れてください。

  M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
  Toio Core Cube が見つかるとシリアルポートのログにデバイス名と MAC アドレス
  が表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを
  押して再起動してください。

  接続が完了すると、デフォルトではオフの機能(IDの通知間隔の変更、磁気センサーの使用、
  姿勢角度の通知、モーター速度の通知)をオンにし、それぞれのイベントを待ち受け、受信し
  たイベント情報を シリアルポートのログに表示します。

  Toio Core Cubeが接続された後、M5Stack の A ボタン押すと、Toio Core Cubeが
  加速度指定モーター制御で移動します。(速度の変化が通知されます。)
  Aボタンを2秒以上押すと Toio Core Cube との BLE 接続を遮断または再接続します。

  arduino ESP32ではBLE notificationの登録が4つまでに限られるようです。
  そのため、Toio Core Cubeのすべてのnotify(イベント)を同時にうけることができません。
  必要なnotify(のコールバック)のみ(4つまで)セットするようにしてください。
  (モーション、磁気、姿勢角度は同じcharacteristicを使うので１つにカウントします。)
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
  M5.Log.printf("M5StackToio event2_test\n");

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

  // Identification sensor ID notification settings
  // 読み取りセンサーの ID 通知設定
  toiocore->setIDnotificationSettings(1, NotifyChangesOnly);

  // Identification sensor ID missed notification settings
  // 読み取りセンサーの ID missed 通知設定
  toiocore->setIDmissedNotificationSettings(10);
  
  // Set ID notification event callback.
  // 読み取りセンサーのイベントのコールバックをセット
  toiocore->onIDReaderData([](ToioCoreIDData id_data){
    M5.Log.printf("IDReader Event ");
      if (id_data.type == ToioCoreIDTypePosition) {
        M5.Log.printf("posX: %u posY: %u angle: %u\n", 
        id_data.position.cubePosX,
        id_data.position.cubePosY,
        id_data.position.cubeAngleDegree);
      }
      else if (id_data.type == ToioCoreIDTypeStandard) {
        M5.Log.printf("Standard ID: %u angle: %u\n",
        id_data.standard.standardID, id_data.standard.cubeAngleDegree);
      }
      else {
        M5.Log.printf("no id found.\n");
      }
  });

  // Magnetic sensor settings
  // 磁気センサーの設定
  // toiocore->setMagneticSensorSettings(1, NotifyChangesOnly, EnableMagnetState);  // 磁石の状態を検出
  toiocore->setMagneticSensorSettings(1, NotifyChangesOnly, EnableMagneticForce); // 磁力の強さを検出

  // Posture angle detection settings
  // 姿勢角検出の設定
  toiocore->setPostureAngleDetectionSettings(1, NotifyChangesOnly, AngleTypeEuller); // 姿勢をオイラー角(int16)で検出
  //toiocore->setPostureAngleDetectionSettings(1, NotifyChangesOnly, AngleTypeQuaternion); // 姿勢を四元数(float)で検出
  //toiocore->setPostureAngleDetectionSettings(1, NotifyChangesOnly, AngleTypeHighPrecisionEuller); // 姿勢を高精度オイラー角(float)で検出

  // Set motion event, magnetic sensor event, posture angle event callbacks.
  // Motion イベントと磁気センサーと姿勢角のコールバックをセット
  toiocore->onMotion(
    [](ToioCoreMotionData motion) {
    M5.Log.printf("Motion Event flat=%u, clash=%u, dtap=%u, attitude=%u , shake=%u\n",
      motion.flat, motion.clash, motion.dtap, motion.attitude, motion.shake);
    },
    [](ToioCoreMagneticSensorData mag_sensor){
      M5.Log.printf("Magnetic Sensor Event state=%u, strength=%u, x=%d, y=%d, z=%d\n",
      mag_sensor.state, mag_sensor.strength, mag_sensor.x, mag_sensor.y, mag_sensor.z);
    },
    [](ToioCorePostureAngle angle){
      M5.Log.printf("Posture Angle Euler(int16) Event roll=%d, pitch=%d, yaw=%d\n",
      angle.euler.roll, angle.euler.pitch, angle.euler.yaw);
      /*
      M5.Log.printf("Posture Angle Quaternion(float32) Event w=%f, x=%f, y=%f, z=%f\n",
      angle.quaternion.w, angle.quaternion.x, angle.quaternion.y, angle.quaternion.z);
      */
      /*
      M5.Log.printf("Posture Angle High precision Euler(float32) Event roll=%f, pitch=%f, yaw=%f\n",
      angle.eulerf.roll, angle.eulerf.pitch, angle.eulerf.yaw);
      */
      
    }
  ); 

  // Set nullptr to not used callback.
  // 不要なコールバックはnullptrをセットする  
  // Example: If you need only magnetic sensor event.
  // 例 磁気だけ必要な場合
  /*
  toiocore->onMotion(
    nullptr,
    [](ToioCoreMagneticSensorData mag_sensor){
      M5.Log.printf("Magnetic Sensor Event state=%u, strength=%u, x=%d, y=%d, z=%d\n",
      mag_sensor.state, mag_sensor.strength, mag_sensor.x, mag_sensor.y, mag_sensor.z);
    },
    nullptr
  ); 
  */

  // Motor speed information acquisition settings
  // モーターの速度情報の取得を有効にする
  toiocore->setMotorSpeedInformationAcquistionSettings(true);

  // Set motor event callback.
  // Motor イベントのコールバックをセット
  toiocore->onMotor([](ToioCoreMotorResponse motor_res) {
    M5.Log.printf("Motor Event Type=%u", motor_res.controlType);
    if(motor_res.controlType == 0xe0 )
      M5.Log.printf( ", left speed=%u, right speed=%u\n", motor_res.controlID, motor_res.response);
    else
      M5.Log.printf( ", controlID=%u, response=%u\n", motor_res.controlID, motor_res.response);
  });   

  // Print free heap memory size.
  // 空きヒープメモリサイズの確認
  //M5.Log.printf("esp_get_free_heap_size(): %6d\n", esp_get_free_heap_size() );
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
    // Move with acceleration specified, to test obtaining motor speed(Motor event callback) will work.
    // 加速度指定付き移動 速度変化イベント(Motor イベントのコールバック)がくるかどうかのテスト用
    M5.Log.printf("Acceleration Control\n");
    toiocore->controlMotorWithAcceleration(50, 15, 30, 0, 0, 0, 200);
  }

  yield();

}