/* ----------------------------------------------------------------
  m5stack-toio - config_event_test.ino

  https://github.com/kenichi884 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  --------------------------------------------------------------
  This sample sketch tests the receipt of configuration responses and serialization information via notify.

  This sample sketch was tested on M5Stack Atom S3.
  This sample sketch uses M5Unified, so it should also work with other M5Stack controllers that have more than one button.
  M5Unified M5.Log.printf() is used for operation log. By default, operation log appears on the serial port.

  [How to use]

  Turn on Toio Core Cube beforehand.

  When M5Stack starts, it automatically scans Toio Core Cube.
  If the Toio Core Cube is found, the device name and MAC address will appear in the serial port log.
  will be displayed in the serial port log. If it is not found, press the reset button on the M5Stack button
  If it is not found, press the reset button on the M5Stack button again to reboot.

  Once the connection is made, it tests the receipt of the configuration response and the serialization information notify.
  Serialization information is received using the same characteristics as the configuration response.
  It listens for configuration response events and displays the received event information in the serial port log.

  After the Toio Core Cube is connected,
  pressing the A button for more than 2 seconds disconnects or reconnects the BLE connection with the Toio Core Cube.

 --------------------------------------------------------------
 
  このサンプルスケッチは設定の応答とシリアライズ情報をnotifyで受信するテストをします。
  このサンプルスケッチは M5Stack Atom S3で動作確認しました。
  M5Unifiedを使っているので、ほかのボタンが一つ以上あるM5Stack製 Controllerでも
  動作すると思います。
  動作ログはM5UnifiedのM5.Log.printf()を使っています。デフォルトではシリアルポートに出ます。

  [使い方]

  事前に Toio Core Cube の電源を入れてください。

  M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
  Toio Core Cube が見つかるとシリアルポートのログにデバイス名と MAC アドレス
  が表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを
  押して再起動してください。

  接続が完了すると、設定の応答ととシリアライズ情報のnotifyの受信をテストします。
  シリアライズ情報は設定の応答と同じcharacteristicsを使って受信します。
  設定の応答イベントを待ち受け、受信したイベント情報を シリアルポートのログに表示します。

  Toio Core Cubeが接続された後、M5Stack の 
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

// Serialized information 19bytes bit fields
// シリアライズ情報19バイトのビットフィールド
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

void setup() {
  M5.begin();
  M5.Log.printf("M5StackToio config_event_test\n");

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

  // Print the Toio Core cube's BLE protocol version.
  // Toio Core のBLEプロトコルバージョンを表示
  M5.Log.printf("%s (%s)\n", toiocore->getName().c_str(), toiocore->getBleProtocolVersion().c_str());

  // Set Event callbacks

  // Set Connection event callback.
  // Connection イベントのコールバックをセット
  toiocore->onConnection([](bool state) {
    M5.Log.printf("Connection Event %s\n", state ? "Connected   " : "Disconnected");
  });

  // Set Configuration event callback.　Also serialized information use same configuration characteristics.
  // 設定イベントのコールバックをセット。シリアライズ情報も同じ設定chracteristicsを使う
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

  // 設定変更の応答
  ToioCoreConfigurationResponse res;

  // Identification sensor ID notification settings
  // 読み取りセンサーの ID 通知設定
  toiocore->setIDnotificationSettings(1, NotifyChangesOnly);
  res = toiocore->getConfigurationResponse();
  M5.Log.printf("IDnotificationSettings response %02x %02x %02x\n", res.infoType, res.config.reserved, res.config.response);

  // Identification sensor ID missed notification settings
  // 読み取りセンサーの ID missed 通知設定
  toiocore->setIDmissedNotificationSettings(10);
  res = toiocore->getConfigurationResponse();
  M5.Log.printf("IDmissedNotificationSettings response %02x %02x %02x\n", res.infoType, res.config.reserved, res.config.response);

  // Magnetic sensor settings
  // 磁気センサーの設定
  //toiocore->setMagneticSensorSettings(1, NotifyChangesOnly, EnableMagnetState);  // 磁石の状態を検出
  toiocore->setMagneticSensorSettings(1, NotifyChangesOnly, EnableMagneticForce); // 磁力の強さを検出
  res = toiocore->getConfigurationResponse();
  M5.Log.printf("MagneticSensorSettings response %02x %02x %02x\n", res.infoType, res.config.reserved, res.config.response);

  // Posture angle detection settings
  // 姿勢角検出の設定
  toiocore->setPostureAngleDetectionSettings(1, NotifyChangesOnly, AngleTypeEuller); // 姿勢をオイラー角(int16)で検出
  //toiocore->setPostureAngleDetectionSettings(1, NotifyChangesOnly, AngleTypeQuaternion); // 姿勢を四元数(float)で検出
  //toiocore->setPostureAngleDetectionSettings(1, NotifyChangesOnly, AngleTypeHighPrecisionEuller); // 姿勢を高精度オイラー角(float)で検出
  res = toiocore->getConfigurationResponse();
  M5.Log.printf("PostureAngleDetectionSettings response %02x %02x %02x\n", res.infoType, res.config.reserved, res.config.response);

  // Motor speed information acquisition settings
  // モーターの速度情報の取得を有効にする
  toiocore->setMotorSpeedInformationAcquistionSettings(true);
  res = toiocore->getConfigurationResponse();
  M5.Log.printf("MotorSpeedInformationAcquistionSettings response %02x %02x %02x\n", res.infoType, res.config.reserved, res.config.response);

  // コネクションインターバルの最小値、最大値をセット
  toiocore->setConnectionInterval(16, 160);
  //toiocore->setConnectionInterval(8, 80);
  res = toiocore->getConfigurationResponse();
  M5.Log.printf("setConnectionInterval response %02x %02x %02x\n", res.infoType, res.config.reserved, res.config.response);

  uint16_t connection_interval_minimum = 0;
  uint16_t connection_interval_maximum = 0;
  // コネクションインターバル値の設定値を取得
  toiocore->getRequestedConnectionInterval(connection_interval_minimum, connection_interval_maximum);
  M5.Log.printf("RequestedConnectionInterval minimum %u, maximum %u\n", connection_interval_minimum, connection_interval_maximum);
  res = toiocore->getConfigurationResponse();
  M5.Log.printf("RequestedConnectionInterval response %02x %02x %u %u\n", res.infoType, res.interval.reserved, res.interval.minimum, res.interval.maximum);
   // コネクションインターバル値の実際値を取得
  toiocore->getAcctualConnectionInterval(connection_interval_minimum, connection_interval_maximum);
  M5.Log.printf("AcctualConnectionInterval minimum %u, maximum %u\n", connection_interval_minimum, connection_interval_maximum);
  res = toiocore->getConfigurationResponse();
  M5.Log.printf("AcctualConnectionInterval response %02x %02x %u %u\n", res.infoType, res.interval.reserved, res.interval.minimum, res.interval.maximum);

#if 0
  // Serialized information settings
  // シリアライズ情報を取得する設定
  toiocore->setSerializedInformationSettings(1, NotifyChangesOnly);
  res = toiocore->getConfigurationResponse();
  M5.Log.printf("SerializedInformationSettings response %02x %02x %02x\n", res.infoType, res.config.reserved, res.config.response);
#endif
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
    M5.Log.printf("pressed\n");
  }

  yield();

}