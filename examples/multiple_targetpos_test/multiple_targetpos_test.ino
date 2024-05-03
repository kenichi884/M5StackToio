/* ----------------------------------------------------------------
  m5stack-toio - multiple_targetpos_test.ino

  https://github.com/kenichi884 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  --------------------------------------------------------------
  This sample sketch was tested on an M5Stack Atom S3.
  This sample sketch uses M5Unified, so it should also work with other M5Stack controllers that have more than one button.
  M5Unified M5.Log.printf() is used for operation log. By default, operation log appears on the serial port.

  This is a sample sketch of connect and control a single Toio Core Cube and test motor control with multiple destinations.

  [How to use]

  Turn on the Toio Core Cube in advance and place it on the mat.
  (Toio Core Cube (stand-alone) works with the simple play mat that comes with it.
  If you use other mats, please change the values of MAT_CENTER_X and MAT_CENTER_Y.)
　When M5Stack starts up, it automatically scans for Toio Core Cube.
  If the Toio Core Cube is found, the device name and MAC address
  will appear in the M5Stack serial port log. If it is not found, press the reset button on the M5Stack again to restart it.
  If it is not found, press the M5Stack reset button again to reboot.

  Once the connection with the Toio Core Cube is established, the lamp (LED) of the Toio Core Cube will be always on.
  After the connection with the Toio Core Cube is established, press the A button on the M5Stack to move 
  the Toio Core Cube to the center position of the mat,
  After moving to the center position of the mat, the Toio Core Cube will move outward in a spiral pattern,
  stitching its way around the specified destinations.
  
  --------------------------------------------------------------
  このサンプルスケッチは M5Stack Atom S3で動作確認しました。
  M5Unifiedを使っているので、ほかのボタンが一つ以上あるM5Stack製 Controllerでも
  動作すると思います。
  動作ログはM5UnifiedのM5.Log.printf()を使っています。デフォルトではシリアルポートに出ます。

  ひとつのToio Core Cubeを接続し、制御し、複数の目的地を指定してのモーター制御をテストします。

  [使い方]

  事前に Toio Core Cube の電源を入れてマットの上に置いてください。
  (toio コア キューブ（単体）付属の簡易プレイマットで動作します。
  ほかのマットを使う場合はMAT_CENTER_X, MAT_CENTER_Yの値を変更してください)
　M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
  Toio Core Cube が見つかると M5Stack シリアルポートのログにデバイス名と MAC アドレス
  が表示されます。見つからなければ、再度、M5Stack のリセットボタンを
  押して再起動してください。

  Toio Core Cubeとの接続が確立すると、Toio Core Cubeのランプ(LED)が常時点灯になります。
  Toio Core Cubeと接続が終わった後、、M5Stack の A ボタン押すと、マットの中心位置に移動した後、
  外側に向かってらせん状に指定した複数の目的地を縫うように移動します。

  -------------------------------------------------------------- */
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
// Toio オブジェクト生成
Toio toio;

// Maximum number of multiple destinations
// 複数の目的地の数の最大値
#define MAX_TOIOCORE_TARGET_NUM 29

ToioCoreTargetPos targetPos[MAX_TOIOCORE_TARGET_NUM];

// Decreare a pointer variable for the discoverd ToioCore object.
// 発見された ToioCore オブジェクトのポインタ変数を定義
ToioCore* toiocore;

// Simple play mat included with the toio Core Cube (single package)
// toio コア キューブ（単体）付属の簡易プレイマット
const int MAT_CENTER_X = 98 + (402 - 98) / 2;
const int MAT_CENTER_Y = 142 + (358 - 142) / 2;
// Play mat included with the toio COLLECTION (circle and two lines side)
// トイオ・コレクション付属のプレイマット(円周と二本線側)
//const int MAT_CENTER_X = 45 + (455 - 45) / 2;
//const int MAT_CENTER_Y = 45 + (455 - 45) / 2;

//#define PI 3.141592653589793

// Calculate coordinates of polygon vertices
// 頂点の座標を計算
void calcTargetPos()
{
  int radius = 30;
  float angleDelta = 2.0 * PI / 8;
  int j = 0;
  for(int i = 0 ; i < MAX_TOIOCORE_TARGET_NUM ; i++){
    targetPos[i].angleAndRotation = 0;
    targetPos[i].angleDegree = (i * angleDelta) * 180 / PI + 90;
    targetPos[i].posX = MAT_CENTER_X + radius * cos(i * angleDelta);
    targetPos[i].posY = MAT_CENTER_Y + radius * sin(i * angleDelta);;
    M5.Log.printf("%d:TargetPos %d, %d, (%d %d)\n", i, targetPos[i].posX, targetPos[i].posY, 
      targetPos[i].angleDegree, targetPos[i].angleAndRotation );
    j = j + 1;
    if(j >= 8){
      radius = radius + 20;
      j = 0;
    }
  }
  
}


void setup() {
  M5.begin();
  M5.Log.printf("M5StackToio multi_targetpos_test\n");

  // Scan Toio Core Cubes in 3 seconds.
  // 3 秒間 Toio Core Cube をスキャン
  M5.Log.printf("Scanning your toio core...\n");
  std::vector<ToioCore*> toiocore_list = toio.scan(3);

  // Exit if could not found any cubes.
  // 見つからなければ終了
  if (toiocore_list.size() == 0) {
    M5.Log.printf("No toio Core Cube was found. Turn on your Toio Core Cube, then press the reset button of your Toio Core Cube.\n");
    return;
  }

  // Assign the ToioCore object of the first Toio Core Cube found.
  // 最初に見つかった Toio Core Cube の ToioCore オブジェクト
  toiocore = toiocore_list.at(0);
  M5.Log.printf("Your toio core was found:      \n");

 // Print the Toio Core cube's device name and MAC address.
  // Toio Core cube のデバイス名と MAC アドレスを表示
  M5.Log.printf("%s (%s)\n", toiocore->getName().c_str(), toiocore->getAddress().c_str());


  // Start BLE connection.
  // BLE 接続開始
  M5.Log.printf("Connecting...\n");

  if (!toiocore->connect()) {
    M5.Log.printf("Failed to establish a BLE connection.\n");
    return;
  }
  M5.Log.printf("Connected.\n");
  toiocore->turnOnLed(0, 0, 255);
  // Print the Toio Core cube's BLE protocol version.
  // Toio Core cubeのBLEプロトコルバージョンを表示
  M5.Log.printf("protocol ver. %s\n", toiocore->getBleProtocolVersion().c_str());

  // Set Connection event callback.
  // Connection イベントのコールバックをセット
  toiocore->onConnection([](bool state) {
    M5.Log.printf("Connection Event %s\n", state ? "Connected   " : "Disconnected");
  });

    // Set Motor event Calback.
  // Motor イベントのコールバックをセット
  toiocore->onMotor([](ToioCoreMotorResponse motor_res) {
    M5.Log.printf("Motor Event Type=%u", motor_res.controlType);
    if(motor_res.controlType == 0xe0)
      M5.Log.printf( ", left speed=%u, right speed=%u\n", motor_res.controlID, motor_res.response);
    else{
      M5.Log.printf( ", controlID=%u, response=%u\n", motor_res.controlID, motor_res.response);
      if(motor_res.response == 0x00 ) // Reached to the target position. 目標地点に到達
      {
        if(motor_res.controlID == 0){ 
          toiocore->turnOnLed(0, 255, 0);
        } else if (motor_res.controlID == 1){
          toiocore->turnOnLed(255, 0, 0);
        }
      }
    }
  }); 

  calcTargetPos();
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
      M5.Log.printf("ry connect\n");
      toiocore->connect(); // If not connected, connect. 接続していないなら接続
    }
  // When button A of M5Stack was pressed.
  // M5Stack のボタン A が押されたときの処理
  } else if(M5.BtnA.wasReleased()) { 
    M5.Log.printf("press/release A btn\n");
    // Move Toio Core Cubes to next positions.
    // 中心位置に移動した後、複数の億票位置を縫うように移動
    toiocore->controlMotorWithTarget(0, 5, 0, 80, 0, MAT_CENTER_X, MAT_CENTER_Y, 0);
    delay(10);
    toiocore->controlMotorWithMultipleTargets(1, 10, 0, 50, 0, 0, MAX_TOIOCORE_TARGET_NUM, targetPos);
    // delay(10);
    
  }

  yield();

}