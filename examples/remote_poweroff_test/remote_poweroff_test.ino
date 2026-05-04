/* ----------------------------------------------------------------
  m5stack-toio - remote_poweroff_test.ino

  https://github.com/kenichi884 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  --------------------------------------------------------------
  This sample sketch tests the remote power-off function.

  This sample sketch was tested on M5Stack Atom S3.
  This sample sketch uses M5Unified, so it should also work with other M5Stack controllers that have more than one button.
  M5Unified M5.Log.printf() is used for operation log. By default, operation log appears on the serial port.

  [How to use]

  Turn on Toio Core Cube beforehand.
  With nimble-arduino, you can remotely power off up to the number of Toio Core Cubes set by CONFIG_BT_NIMBLE_MAX_CONNECTIONS.

  When M5Stack starts, it automatically scans Toio Core Cube.
  If the Toio Core Cube is found, the device name and MAC address will appear in the serial port log.
  will be displayed in the serial port log. If it is not found, press the reset button on the M5Stack button
  If it is not found, press the reset button on the M5Stack button again to reboot.

  After the connection is complete, press and hold the A button on the M5Stack for 2 seconds 
  to set the LED color on each connected Toio Core Cube in sequence, send a power-off command after 3 seconds, 
  and disconnect the connection.
 --------------------------------------------------------------
 
  このサンプルスケッチはリモートパワーオフ機能のテストをします。
  このサンプルスケッチは M5Stack Atom S3で動作確認しました。
  M5Unifiedを使っているので、ほかのボタンが一つ以上あるM5Stack製 Controllerでも
  動作すると思います。
  動作ログはM5UnifiedのM5.Log.printf()を使っています。デフォルトではシリアルポートに出ます。

  [使い方]

  事前に Toio Core Cube の電源を入れてください。
  nimble-arduinoだとCONFIG_BT_NIMBLE_MAX_CONNECTIONSで設定した個数までのToio Core Cubeのリモートパワーオフができます

  M5Stack が起動すると自動的に Toio Core Cube をスキャンします。
  Toio Core Cube が見つかるとシリアルポートのログにデバイス名と MAC アドレス
  が表示されます。見つからなければ、再度、M5Stack ボタンのリセットボタンを
  押して再起動してください。

  接続が完了したあとで、M5Stack の A ボタンを２秒長押しすると、つながったToio Core Cubeに順番に、LEDの色を設定し、
  ３秒後にパワーオフするコマンドを送り、接続を切ります。

  -------------------------------------------------------------- */
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
// Toio オブジェクト生成
Toio toio;

// Decreare a pointer variable for the discoverd ToioCore objects.
// 発見された ToioCore オブジェクトのポインタ変数を定義
#define MAX_TOIOCORE_NUM 8
ToioCore* toiocores[MAX_TOIOCORE_NUM];
size_t toiocore_num = 0;


void setup() {
  M5.begin();
  M5.Log.printf("M5StackToio remote_poweroff_test\n");

  // Scan Toio Core Cubes in 3 seconds.
  // 3 秒間 Toio Core Cube をスキャン
  M5.Log.printf("Scanning your toio core...\n");
  std::vector<ToioCore*> toiocore_list = toio.scan(3);

  // Exit if could not found any cubes.
  // 見つからなければ終了
  toiocore_num = toiocore_list.size();
  if (toiocore_num == 0) {
    M5.Log.printf("No toio Core Cube was found. Turn on your Toio Core Cube, then press the reset button of your Toio Core Cube.\n");
    return;
  }

  if(toiocore_num > MAX_TOIOCORE_NUM)
    toiocore_num = MAX_TOIOCORE_NUM;

  // Assign the ToioCore objects of  Toio Core Cubes found.
  // 見つかった Toio Core Cube の ToioCore オブジェクト
  for(int i = 0; i < toiocore_num; i++)
    toiocores[i] = toiocore_list.at(i);
  M5.Log.printf("Your toio core cubes were found:     %d \n", toiocore_num);

  // Print the Toio Core cube's device name and MAC address.
  // Toio Core のデバイス名と MAC アドレスを表示
  for(int i = 0; i < toiocore_num; i++)
    M5.Log.printf("%s (%s)\n", toiocores[i]->getName().c_str(), toiocores[i]->getAddress().c_str());

  // Start BLE connection.
  // BLE 接続開始
  M5.Log.printf("Connecting...\n");
  for(int i = 0; i < toiocore_num; i++){
    if (!toiocores[i]->connect()) {
      M5.Log.printf("Failed to establish a BLE connection. %s\n", toiocores[i]->getName().c_str());
    } else {
      M5.Log.printf("Connected. %s ", toiocores[i]->getName().c_str());
      // Print the Toio Core cube's BLE protocol version.
      // Toio Core のBLEプロトコルバージョンを表示
      M5.Log.printf("%s (%s)\n", toiocores[i]->getName().c_str(), toiocores[i]->getBleProtocolVersion().c_str());
      // Set Connection event callback.
      // Connection イベントのコールバックをセット
      toiocores[i]->onConnection([](bool state) {
        M5.Log.printf("Connection Event %s\n", state ? "Connected   " : "Disconnected");
      });
      // Set Event callbacks
      // Set Configuration event callback.　
      // 設定イベントのコールバックをセット。
      toiocores[i]->onConfiguration([] (ToioCoreConfigurationResponse resp){
        M5.Log.printf(" config event %02x  ", resp.infoType);
        if(resp.infoType == ResponseRemotePowerOff) {
          M5.Log.printf("ResponseRemotePowerOff %02x %02x \n", resp.config.reserved, resp.config.response);
        } else {
          M5.Log.printf("set configuration %02x %02x \n", resp.config.reserved, resp.config.response);
        }
      });
      // Set indicator color.
      // LED色設定
      uint j = i + 1;
      toiocores[i]->turnOnLed((j & 0x01) ? 255:0, (j & 0x02) ? 255:0, (j & 0x04) ? 255:0 );
    }
  }
  // Print free heap memory size.
  // 空きヒープメモリサイズの確認
  //M5.Log.printf("esp_get_free_heap_size(): %6d\n", esp_get_free_heap_size() );
}

uint8_t mute_mode = TurnOffMute;
void loop() {
  M5.update();

  // If you want to handle event callbacks, you shoud call loop() method  of Toio Object here.
  // イベントを扱う場合は、必ずここで Toio オブジェクトの
  // loop() メソッドを呼び出すこと
  toio.loop();

  // M5Stack のボタン A が2秒長押しされたときの処理
  // When button A of M5Stack is pressed for 2 seconds
  // リモートパワーオフを行う
  // Perform a remote power off.
  if (M5.BtnA.wasReleaseFor(2000)) { 
    for(int i = 0; i < toiocore_num; i++)
      if (toiocores[i]->isConnected()) {
        M5.Log.println("try Power Off after 3 seconds.");
        toiocores[i]->setRemotePowerOff(3);
        toiocores[i]->disconnect();
      }
  }

  yield();

}