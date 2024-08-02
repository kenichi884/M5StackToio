/* ----------------------------------------------------------------
  Toio.cpp

  Copyright (c) 2020 Futomi Hatano. All right reserved.
  Original https://github.com/futomi
  Toio ID read support   https://github.com/mhama
  Protocol v2.4.0 and NimBLE support   https://github/kenichi884
  
  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.
  -------------------------------------------------------------- */
#include "Toio.h"
// ===============================================================
// Toio クラス
// ===============================================================

// ---------------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------------
Toio::Toio() {
}

// ---------------------------------------------------------------
// toio をスキャン
// ---------------------------------------------------------------
std::vector<ToioCore*> Toio::scan(uint8_t duration = 3) {
  // スキャンの準備
  BLEDevice::init("");
  BLEScan* scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  scan->setInterval(this->_BLE_SCAN_INTERVAL);
  scan->setWindow(this->_BLE_SCAN_WINDOW);

  // スキャン開始
#if defined(CONFIG_IDF_TARGET_ESP32C6)
  BLEScanResults *scan_res = scan->start(duration, false);
  int num = scan_res->getCount();
#else
  BLEScanResults scan_res = scan->start(duration, false);
  int num = scan_res.getCount();
#endif
  std::vector<ToioCore*> found_toiocore_list;

  for (int i = 0; i < num; i++) {
#if defined(CONFIG_IDF_TARGET_ESP32C6)
    BLEAdvertisedDevice device = scan_res->getDevice(i);
#else
    BLEAdvertisedDevice device = scan_res.getDevice(i);
#endif
    // サービス UUID がセットされていないデバイスなら無視
    if (!device.haveServiceUUID()) {
      continue;
    }
    // サービス UUID が toio に一致しなければ無視
    if (device.getServiceUUID().toString() != this->_TOIO_SERVICE_UUID) {
      continue;
    }

    // ToioCore オブジェクトを生成
    ToioCore* toiocore = new ToioCore(device);
    found_toiocore_list.push_back(toiocore);

    // すでに発見済みの toio かどうかをチェック
    std::string addr = toiocore->getAddress();
    auto itr = this->_devices.find(addr);
    if ( itr == this->_devices.end() ) {
      this->_devices[addr] = toiocore;
    }
  }
  scan->clearResults();
  return found_toiocore_list;
}

// ---------------------------------------------------------------
// .ino の loop() 内で呼び出す
// ---------------------------------------------------------------
void Toio::loop() {
  for (auto itr = this->_devices.begin(); itr != this->_devices.end(); ++itr) {
    ToioCore* toiocore = itr->second;
    toiocore->_loop();
  }
}
