/* ----------------------------------------------------------------
  m5stack-toio - savable_settings_test.ino

  https://github.com/kenichi884 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.

  --------------------------------------------------------------
  This sample sketch tests the savable settings feature.

  This sample sketch was tested on M5Stack Atom S3.
  This sample sketch uses M5Unified, so it should also work with other M5Stack controllers that have more than one button.
  M5Unified M5.Log.printf() is used for operation log. By default, operation log appears on the serial port.

  [How to use]

  Turn on Toio Core Cube beforehand.

  When M5Stack starts, it automatically scans Toio Core Cube.
  If the Toio Core Cube is found, the device name and MAC address will appear in the serial port log.
  will be displayed in the serial port log. If it is not found, press the reset button on the M5Stack button
  If it is not found, press the reset button on the M5Stack button again to reboot.

  After connecting to the Toio Core Cube, a command to initialize saved settings will be sent after 2 seconds.
　If you hold down the function button on the Toio Core Cube until then, it will accept the initialization.
　If you do not want to initialize, do not press the function button.

  Afterward, press and hold the A button on the M5Stack for two seconds to mute the sound.
  Even at this point, the sound mute setting will not be accepted unless you keep the function button on the Toio Core Cube pressed.
  
  It listens for configuration response events and displays the received event information in the serial port log.

 --------------------------------------------------------------
 
  このサンプルスケッチは保存可能な設定機能のテストをします。
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

  Toio Core Cubeと接続した後、２秒後に保存加納な設定の初期化コマンドを送信します。
  それまでにToio Core Cubeの機能ボタンを押したままにしておくと初期化を受け付けます。
  初期化したくない場合は機能ボタンを押さないでください。

  その後、M5Stack の A ボタンを２秒長押しすると、サウンドのミュート設定を行ます。
  このときもToio Core Cubeの機能ボタンを押した状態にしておかないとサウンドのミュート設定を受け付けません。

  設定の応答イベントを待ち受け、受信したイベント情報を シリアルポートのログに表示します。

  -------------------------------------------------------------- */
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
// Toio オブジェクト生成
Toio toio;

// Decreare a pointer variable for the discoverd ToioCore objects.
// 発見された ToioCore オブジェクトのポインタ変数を定義
ToioCore* toiocore;


void setup() {
  M5.begin();
  M5.Log.printf("M5StackToio savable_settings_test\n");

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
    M5.Log.printf("Failed to establish a BLE connection. %s\n", toiocore->getName().c_str());
  } else {
    M5.Log.printf("Connected. %s ", toiocore->getName().c_str());
    // Print the Toio Core cube's BLE protocol version.
    // Toio Core のBLEプロトコルバージョンを表示
    M5.Log.printf("%s (%s)\n", toiocore->getName().c_str(), toiocore->getBleProtocolVersion().c_str());
    // Set Connection event callback.
    // Connection イベントのコールバックをセット
    toiocore->onConnection([](bool state) {
      M5.Log.printf("Connection Event %s\n", state ? "Connected   " : "Disconnected");
    });
    // Set Event callbacks
    // Set Configuration event callback.　
    // 設定イベントのコールバックをセット。
    toiocore->onConfiguration([] (ToioCoreConfigurationResponse resp){
      M5.Log.printf(" config event %02x  ", resp.infoType);
      if(resp.infoType == ResponseSaveableSettings) {
        M5.Log.printf("ResponseSaveableSettings %02x %02x \n", resp.config.reserved, resp.config.response);
      } else if(resp.infoType == ResponseMuteSoundSettings){
        M5.Log.printf("ResponseMuteSoundSettings %02x %02x \n", resp.config.reserved, resp.config.response);
      } else {
        M5.Log.printf("set configuration %02x %02x \n", resp.config.reserved, resp.config.response);
      }
    });

    // Resets the saveable settings to their default state.
    // Pressing and holding the function button on the Toio Core Cube will initiate initialization.
    // Once initialized, the LED will blink white once.
    // The Toio Core Cube will not be initialized unless you press the function button.
    // 保存可能な設定の初期化を行う。
    // Toio Core Cubeの機能ボタンを押したままにしておくと初期化を受け付ける。初期化されるとLEDが白色で１回点滅する。
    // Toio Core Cubeの機能ボタンを押していなければ初期化はされない。
    delay(2000);
    M5.Log.printf("To initalize savable settings, Press Toio core cubes's fuction button.\n");
    delay(2000);
    toiocore->initializeSavableSettings();
    M5.Log.printf("initialize savable command sended.\n");
  
  }

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
  // サウンドのミュート設定を変更するにはToio Core Cubeの機能ボタンを押したままにしておく必要がある。設定されるとLEDが白色で１回点滅する。
  // Toio Core Cubeの機能ボタンを押していなければ設定はされない。
  // To change the sound mute setting, you need to press and hold the function button on the Toio Core Cube.
  // The LED will blink white once when the setting is applied.
  // The settings will not be applied unless you press the function button on the Toio Core Cube.
  if (M5.BtnA.wasReleaseFor(2000)) { 
    if (toiocore->isConnected()) {
      M5.Log.println("set mute sound settings");
      //toiocore->setMuteSoundSettings(TurnOffMute); // ミュート解除 Turn off mute
      toiocore->setMuteSoundSettings(MuteAllSounds); // 全ての音を消音 Mute All Sounds
      //toiocore->setMuteSoundSettings(MuteSystemSoundsOnly); // システム音だけ消音 Mute System Sounds only

      //toiocore->disconnect();
    }
  }

  yield();

}