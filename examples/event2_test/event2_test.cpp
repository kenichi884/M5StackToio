/* ----------------------------------------------------------------
  m5stack-toio - event2_test.ino

  Copyright (c) 2020 Futomi Hatano. All right reserved.
  https://github.com/futomi
  Toio ID read support   https://github.com/mhama
  Protocol v2.3.0 support  https://github.com/kenichi84 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  このサンプルスケッチはデフォルトではオフの機能(IDの通知間隔の変更、磁気センサーの使用、
  姿勢角度の通知、モーター速度の通知)をテストします。
  このサンプルスケッチは M5Stack Atom S3、Atom S3 lite、M5Capsuleで動作確認しました。
  M5Unifiedを使っているので、ほかのボタンが一つ以上あるM5Stack製 Controllerでも
  動作すると思います。
  動作ログはシリアルポートにprintln()しています。ESP32-S3以外の場合は
  #define SERIAL_LOGの定義をUSBSerialからSerialに変更してください。

  [使い方]

  事前に Toio Core Cube の電源を入れてください。

  M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
  Toio Core Cube が見つかるとシリアルポートのログにデバイス名と MAC アドレス
  が表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを
  押して再起動してください。

  接続が完了すると、デフォルトではオフの機能(IDの通知間隔の変更、磁気センサーの使用、
  姿勢角度の通知、モーター速度の通知)をオンにし、それぞれのイベントを待ち受け、受信し
  たイベント情報を シリアルポートのログに表示します。

  Toio Core Cube 接続された後、M5Stack の A ボタン押すと、Toio Core Cubeが
  加速度指定モーター制御で移動します。(速度の変化が通知されます。)
  Aボタンを2秒以上押すと Toio Core Cube との BLE 接続を遮断または再接続します。

  arduino ESP32ではBLE notificationの登録が4つまでに限られるようです。
  そのため、Toio Core Cubeのすべてのnotify(イベント)を同時にうけることができません。
  必要なnotify(のコールバック)のみ(4つまで)セットするようにしてください。
  (モーション、磁気、姿勢角度は同じcharacteristicを使うので１つにカウントします。)
  -------------------------------------------------------------- */
#include <M5Unified.h>
#include <Toio.h>

// for ESP32-S3
#define SERIAL_LOG USBSerial
// #define SERIAL_LOG Serial

// Toio オブジェクト生成
Toio toio;

// 発見された ToioCore オブジェクトのポインタ変数を定義
ToioCore* toiocore;

void setup() {
  M5.begin();
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
  toiocore = toiocore_list.at(0);
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
  

    // Connection イベントのコールバックをセット
  toiocore->onConnection([](bool state) {
    SERIAL_LOG.print("Connection Event ");
    SERIAL_LOG.println(state ? "Connected   " : "Disconnected");
  });

  // arduino ESP32では4つまでしかBLE notificationをregisterForNotify()できない
  // https://github.com/espressif/arduino-esp32/issues/8015
  // 以下のイベントコールバックのセットも合計4つまでしかセットできない。

  // 読み取りセンサーの ID 通知設定
  toiocore->setIDnotificationSettings(1, 1);

  // 読み取りセンサーの ID missed 通知設定
  toiocore->setIDmissedNotificationSettings(10);
  
  // 読み取りセンサーのイベントのコールバックをセット
  toiocore->onIDReaderData([](ToioCoreIDData id_data){
  SERIAL_LOG.print("IDReader Event ");
    if (id_data.type == ToioCoreIDTypePosition) {
      SERIAL_LOG.println("posX: " + String(id_data.position.cubePosX)
      + " posY: " + String(id_data.position.cubePosY)
      + " angle: " + String(id_data.position.cubeAngleDegree));
    }
    else if (id_data.type == ToioCoreIDTypeStandard) {
      SERIAL_LOG.println("Standard ID: " + String(id_data.standard.standardID));
    }
    else {
      SERIAL_LOG.println("no id found.");
    }
  });

  // 磁気センサーの設定
  // toiocore->setMagneticSensorSettings(1, 1, 1);  // 磁石の除隊を検出
  toiocore->setMagneticSensorSettings(2, 1, 1); // 磁力の強さを検出


  // 姿勢角検出の設定
  toiocore->setPostureAngleDetectionSettings(1, 1); // 姿勢をオイラー格で検出


  // Motion イベントと磁気センサーと姿勢角のコールバックをセット
  toiocore->onMotion(
    [](ToioCoreMotionData motion) {
    SERIAL_LOG.print("Motion Event ");
    SERIAL_LOG.println("flat=" + String(motion.flat) + ", clash=" + String(motion.clash) + ", dtap=" + String(motion.dtap) + ", attitude=" + String(motion.attitude) + ", shake=" + String(motion.shake));
    },
    [](ToioCoreMagneticSensorData mag_sensor){
      SERIAL_LOG.print("Magnetic Sensor Event ");
      SERIAL_LOG.println("state=" + String(mag_sensor.state) + ", strength=" + String(mag_sensor.strength) + ", x=" + String(mag_sensor.x) + ", y=" + String(mag_sensor.y) + ", z=" + String(mag_sensor.z));
    },
    [](ToioCorePostureAngleEuler euler_angle){
      SERIAL_LOG.print("Posture Angle Euler Event ");
      SERIAL_LOG.println("roll=" + String(euler_angle.roll) + ", pitch=" + String(euler_angle.pitch) + ", yaw=" + String(euler_angle.yaw));
    }
  ); 
  // 不要なコールバックはnullptrをセットする  
  // 例 磁気だけ必要な場合
  /*
  toiocore->onMotion(
    nullptr,
    [](ToioCoreMagneticSensorData mag_sensor){
      SERIAL_LOG.print("Magnetic Sensor Event ");
      SERIAL_LOG.println("state=" + String(mag_sensor.state) + ", strength=" + String(mag_sensor.strength) + ", x=" + String(mag_sensor.x) + ", y=" + String(mag_sensor.y) + ", z=" + String(mag_sensor.z));
    },
    nullptr
  ); 
  */

  // モーターの速度情報の取得を有効にする
  toiocore->setMotorSpeedInformationAcquistionSettings(true);

  // Motor イベントのコールバックをセット
  toiocore->onMotor([](ToioCoreMotorResponse motor_res) {
    SERIAL_LOG.print("Motor Event ");
    SERIAL_LOG.print("Type=" + String(motor_res.controlType));
    if(motor_res.controlType == 0xe0 )
      SERIAL_LOG.println( ", left speed=" + String(motor_res.controlID) + ", right speed=" + String(motor_res.response));
    else
      SERIAL_LOG.println( ", controlID=" + String(motor_res.controlID) + ", response=" + String(motor_res.response));
  });   
}

void loop() {
  M5.update();

  // イベントを扱う場合は、必ずここで Toio オブジェクトの
  // loop() メソッドを呼び出すこと
  toio.loop();

  if (M5.BtnA.wasReleaseFor(2000)) { // M5Stack のボタン A が2秒長押しされたときの処理
    // Toio Core Cube と BLE 接続中かどうかをチェック
    if (toiocore->isConnected()) {
      SERIAL_LOG.println("try disconnect");
      toiocore->disconnect(); // 接続中なら切断
    } else {
      SERIAL_LOG.println("try connect");
      toiocore->connect(); // 切断中なら接続
    }
  } else if(M5.BtnA.wasReleased()) { // M5Stack のボタン A が押されたときの処理
    SERIAL_LOG.println("start motor");
    // 加速度指定付き移動 速度変化イベント(Motor イベントのコールバック)がくるかどうかのテスト用
    SERIAL_LOG.println("Acceleration Control");
    toiocore->controlMotorWithAcceleration(50, 15, 30, 0, 0, 0, 200);
  }

  yield();

}