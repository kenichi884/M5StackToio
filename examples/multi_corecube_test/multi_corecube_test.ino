/* ----------------------------------------------------------------
  m5stack-toio - multi_corecube_test.ino

  https://github.com/kenichi884 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  --------------------------------------------------------------
  This sample sketch was tested on an M5Stack Atom S3.
  This sample sketch uses M5Unified, so it should also work with other M5Stack controllers that have more than one button.
  M5Unified M5.Log.printf() is used for operation log. By default, operation log appears on the serial port.

  This is a sample sketch of connecting multiple Toio Core Cubes, controlling them, and receiving events.
  You can connect and run at least two and up to six toio core cube.
  The default configuration of NimBLE only allows up to three connections, so you need to add the following line to nimconfig.h
  #define CONFIG_BT_NIMBLE_MAX_CONNECTIONS to 6.
  (ESP32 seems to be able to connect up to 9 devices, but I have only 6 Toio Core Cube on hand, so I have not tested it)

  [How to use]

  Turn on the Toio Core Cube in advance and place it on the mat.
  (Toio Core Cube (stand-alone) works with the simple play mat that comes with it.
  If you use other mats, please change the values of MAT_CENTER_X and MAT_CENTER_Y.)
　When M5Stack starts up, it automatically scans for Toio Core Cube.
  If the Toio Core Cube is found, the device name and MAC address
  will appear in the M5Stack serial port log. If it is not found, press the reset button on the M5Stack again to restart it.
  If it is not found, press the M5Stack reset button again to reboot.

  Once the connection with the Toio Core Cube is established, the lamp (LED) of the Toio Core Cube will be always on.
  After all Toio Core Cubes are connected, press the A button on the M5Stack.
  The Toio Core Cube will move to the position corresponding to the number of Toio Core Cubes (3 for a triangle, 4 for a square, 6 for a hexagon).
  (The first time, Toio Core Cubes may collide depending on the position they are placed, so please change the position by hand.)
  Each time you press the A button on the M5Stack, the Toio Core Cube will change position.
  
  When the Toio Core Cube button is pressed, the Toio Core Cube that has been pressed will sound a sound effect, and the Toio Core Cube that has not been pressed
  rotates for 300ms.

  --------------------------------------------------------------
  このサンプルスケッチは M5Stack Atom S3で動作確認しました。
  M5Unifiedを使っているので、ほかのボタンが一つ以上あるM5Stack製 Controllerでも
  動作すると思います。
  動作ログはM5UnifiedのM5.Log.printf()を使っています。デフォルトではシリアルポートに出ます。

  複数のToio Core Cubeを接続し、制御し、イベントを受け取るサンプルスケッチです。
  2つ以上6つまでtoio core cubeを接続して動かすことができます。
  NimBLEのデフォルト設定では3つまでしか接続できないので、nimconfig.hの
  #define CONFIG_BT_NIMBLE_MAX_CONNECTIONS の値を6に変更してください。
  (ESP32では9個まで接続できるようですが手持ちのToio Core Cubeは6つしかないので試せていません)

  [使い方]

  事前に Toio Core Cube の電源を入れてマットの上に置いてください。
  (toio コア キューブ（単体）付属の簡易プレイマットで動作します。
  ほかのマットを使う場合はMAT_CENTER_X, MAT_CENTER_Yの値を変更してください)
　M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
  Toio Core Cube が見つかると M5Stack シリアルポートのログにデバイス名と MAC アドレス
  が表示されます。見つからなければ、再度、M5Stack のリセットボタンを
  押して再起動してください。

  Toio Core Cubeとの接続が確立すると、Toio Core Cubeのランプ(LED)が常時点灯になります。
  すべてのToio Core Cubeと接続が終わった後、、M5Stack の A ボタン押すとToio Core Cubeの数に
  合わせた位置(3つなら三角形の頂点の位置、4つなら四角形、6つなら六角形)に移動します。
  (初回は置いた位置によってはToio Core Cubeが衝突するので手で場所を変えてあげてください)
  M5Stack の A ボタン押すごとにToio Core Cubeは位置を変えます。
  
  Toio Core Cubeのボタンを押すと、押したToio Core Cubeで効果音が鳴り、押していないToio Core Cube
  が300ms回転します。

  -------------------------------------------------------------- */
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
// Toio オブジェクト生成
Toio toio;

// Set this value or higher.  in nimconfig.h  "#define CONFIG_BT_NIMBLE_MAX_CONNECTIONS" 
#define MAX_TOIOCORECUBE_NUM 6

ToioCoreTargetPos targetPos[MAX_TOIOCORECUBE_NUM];

// Decreare a pointer variable for the discoverd ToioCore object.
// 発見された ToioCore オブジェクトのポインタ変数を定義
ToioCore* toiocore[MAX_TOIOCORECUBE_NUM];
size_t toiocore_num = 0;

// Simple play mat included with the toio Core Cube (single package)
// toio コア キューブ（単体）付属の簡易プレイマット
const int MAT_CENTER_X = 98 + (402 - 98) / 2;
const int MAT_CENTER_Y = 142 + (358 - 142) / 2;
// Play mat included with the toio COLLECTION (circle and two lines side)
// トイオ・コレクション付属のプレイマット(円周と二本線側)
//const int MAT_CENTER_X = 45 + (455 - 45) / 2;
//const int MAT_CENTER_Y = 45 + (455 - 45) / 2;
const int RADIUS = 60;
#define PI 3.141592653589793

// Calculate coordinates of polygon vertices
// 頂点の座標を計算
void calcTargetPos()
{
  if(toiocore_num > 0) {
    float angleDelta = 2.0 * PI / toiocore_num;
    for(int i = 0 ; i < toiocore_num ; i++){
      targetPos[i].angleAndRotation = 0;
      targetPos[i].angleDegree = (i * angleDelta) * 180 / PI + 90;
      targetPos[i].posX = MAT_CENTER_X + RADIUS * cos(i * angleDelta);
      targetPos[i].posY = MAT_CENTER_Y + RADIUS * sin(i * angleDelta);;
      M5.Log.printf("preset Target %d, %d, (%d %d)\n", targetPos[i].posX, targetPos[i].posY, 
        targetPos[i].angleDegree, targetPos[i].angleAndRotation );
    }
  }
}

// Move Toio Core Cubes to next positions.
// 次の頂点位置に移動
void moveToNextTarget()
{
  if(toiocore_num < 1) return;
  ToioCoreTargetPos p = targetPos[0];
  // rotate target positions
  for(int i = 0 ; i < (toiocore_num -1) ; i++){
    targetPos[i] = targetPos[i + 1];
  }
  targetPos[toiocore_num -1] = p;
  for(int i = 0 ; i < toiocore_num ; i++){
    toiocore[i]->controlMotorWithTarget(0, 5, 0, 80, 0, 
      targetPos[i].posX, targetPos[i].posY, 
      targetPos[i].angleDegree, targetPos[i].angleAndRotation);
  }

}

void setup() {
  M5.begin();
  M5.Log.printf("M5StackToio multi_corecube_test\n");

  // Scan Toio Core Cubes in 5 seconds.
  // 5 秒間 Toio Core Cube をスキャン
  M5.Log.printf("Scanning your toio core...\n");
  std::vector<ToioCore*> toiocore_list = toio.scan(3);

  // Exit if could not found any cubes.
  // 見つからなければ終了
  toiocore_num = toiocore_list.size();
  if (toiocore_num == 0) {
    M5.Log.printf("No toio Core Cube was found. Turn on your Toio Core Cube, then press the reset button of your Toio Core Cube.\n");
    return;
  }
  M5.Log.printf("found %u toio core cubes.\n", toiocore_num);
  for(uint i = 0 ; i < toiocore_num ; i++){
    // Assign the ToioCore object of the Toio Core Cubes found.
    // 見つかった Toio Core Cube の ToioCore オブジェクト
    toiocore[i] = toiocore_list.at(i);
    // Print the Toio Core cube's device name and MAC address.
    // Toio Core のデバイス名と MAC アドレスを表示
    M5.Log.printf("%u: %s (%s)\n", i, toiocore[i]->getName().c_str(), toiocore[i]->getAddress().c_str());
    targetPos[i]= {0, 0, 0, 0};

    // Start BLE connection.
    // BLE 接続開始
    M5.Log.printf("Connecting...\n");

    if (!toiocore[i]->connect()) {
      M5.Log.printf("%u: Failed to establish a BLE connection.\n", i);
      return;
    }
    M5.Log.printf("%u: Connected.\n", i);
    // Set Connection event callback.
   // Connection イベントのコールバックをセット
   M5.Log.printf("set cannection callback %u %p\n", i, toiocore[i]);
    toiocore[i]->onConnection([i](bool state) {
      M5.Log.printf("%u: Connection Event %s\n", i, state ? "Connected   " : "Disconnected");
    });

    /// Button Event callback
    toiocore[i]->onButton([i](bool state){
      M5.Log.printf("%u: Button Event %d\n", i, state);
      // Play sound effect
      toiocore[i]->playSoundEffect(i);
      for(int j = 0 ; j < toiocore_num; j++){
        // Spin other than yourself for 300ms.
        // 自分以外を300ms回転
        if(j != i)
          toiocore[j]->controlMotor(true, 80, false, 80, 300);
      }
    });

  // Set indicator color.
  // LED色設定
  uint j = i + 1;
  toiocore[i]->turnOnLed((j & 0x01) ? 255:0, (j & 0x02) ? 255:0, (j & 0x04) ? 255:0 );

  }

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
    for(uint i = 0; i < toiocore_num ; i++){
      if (toiocore[i]->isConnected()) {
        M5.Log.printf("%u: try disconnect\n", i);
        toiocore[i]->disconnect(); // If connected, disconnect. 接続中なら切断
      } else {
        M5.Log.printf("%u: try connect\n", i);
        toiocore[i]->connect(); // If not connected, connect. 接続していないなら接続
      }
    }
  // When button A of M5Stack was pressed.
  // M5Stack のボタン A が押されたときの処理
  } else if(M5.BtnA.wasReleased()) { 
    M5.Log.printf("press/release A btn\n");
  // Move Toio Core Cubes to next positions.
  // 次の位置に移動
    moveToNextTarget();
  }

  yield();

}