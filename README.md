M5StackToio
===============
[日本語 Japanese](README_jp.md)

This library is based on the original https://github.com/futomi/M5StackToio and the version https://github.com/mhama/M5StackToio that adds the Toio ID reading function, and is compatible with protocol v2.3.0. This is a combination of position-specific motor control.

M5StackToio is an Arduino library for [M5Stack](https://m5stack.com/) to operate Sony Interactive Entertainment's "[toio Core Cube](https://toio.io/)".

---------------------------------------
## Index

* [1. Setting up the development environment](#Setup-IDE)
* [2. Omsta;; M5StackToio library](#Install-M5StackToio)
* [3. Usage](#Usage)
* [4. `Toio` object](#Toio-object)
  * [`scan()` method (Discover toio core cubes)](#Toio-scan-method)
  * [`loop()` method (Process events)](#Toio-loop-method)
* [5. `ToioCore` object](#ToioCore-object)
  * [`getAddress()` method (Get address)](#ToioCore-getAddress-method)
  * [`getName()` method (gGet device name)](#ToioCore-getName-method)
  * [`connect()` method (Establish BLE connection)](#ToioCore-connect-method)
  * [`disconnect()` method (Disconnect BLE connection)](#ToioCore-disconnect-method)
  * [`isConnected()` method (Get connected or not)](#ToioCore-isConnected-method)
  * [`onConnection()` method (Set connection and disconnection callback)](#ToioCore-onConnection-method)
  * [`getBleProtocolVersion()` method (Get the BLE protocol version)](#ToioCore-getBleProtocolVersion-method)
  * [`playSoundEffect()` method (Play sound effects)](#ToioCore-playSoundEffect-method)
  * [`playSoundRaw()` method (Play the MIDI note numbers)](#ToioCore-playSoundRaw-method)
  * [`stopSound()` method (Stop playing)](#ToioCore-stopSound-method)
  * [`turnOnLed()` method (Turning the indicator on)](#ToioCore-turnOnLed-method)
  * [`turnOffLed()` method (Turning the indicator off)](#ToioCore-turnOffLed-method)
  * [`getBatteryLevel()` method (Get the battery level)](#ToioCore-getBatteryLevel-method)
  * [`onBattery()` method (Set the battery level event callback)](#ToioCore-onBattery-method)
  * [`getButtonState()` method (Get the button state)](#ToioCore-getButtonState-method)
  * [`onButton()` method (Set the button state change event)](#ToioCore-onButton-method)
  * [`getMotion()` method (Get the motion sensor status)](#ToioCore-getMotion-method)
  * [`onMotion()` method (Set the motion sensor state change event callback)](#ToioCore-onMotion-method)
  * [`controlMotor()` method (control the motors)](#ToioCore-controlMotor-method)
  * [`drive()` method (drive toio core cube like a car)](#ToioCore-drive-method)
  * [`controlMotorWithTarget()` method (Motor control with target specified (single target))](#ToioCore-controlMotorWithTarget-method)
  * [`controlMotorWithMultipleTargets()` method (Motor control with multiple targets)](#ToioCore-controlMotorWithMultipleTargets-method)
  * [` controlMotorWithAcceleration()` method (Motor control with acceleration specified)](#ToioCore-controlMotorWithAcceleration-method)
  * [`onMotor()` method (Set motor event(reached to target, motor speed infomation) callback)](#ToioCore-onMotor-method)
  * [`getIDReaderData()` method (Get the identification sensor information (position on mat, etc)](#ToioCore-getIDReaderData-method)
  * [`onIDReaderData()` メソッド (Set identification sensor iformation event callback)](#ToioCore-onIDReaderData-method)

* [6. Sample Arduino sketches](#Sample-Sketches)
* [Release notes](#Release-Note)
* [References](#References)
* [License](#License)

---------------------------------------
## <a id="Setup-IDE">1. Setup development environment</a>

M5Stack development environment is required to use this library. Please follow the instructions on the M5Stack official website to set up your development environment.

* [M5Stack Docs - Arduino IDE Development](https://docs.m5stack.com/#/en/arduino/arduino_development)

Requires installation of USB driver, Arduino-IDE installation, Boards Manager settings, board installation, board selection, and M5Stack Library installation.

---------------------------------------
## <a id="Install-M5StackToio">2. Install M5StackToio/a>

Click the `Code` button on the top right of this page and select `Download ZIP` to download the zip file of this library.

Start Arduino IDE and select `Sketch` -> `Include library` -> `Install .ZIP format library...` from the menu bar. Select the zip file of this library that you downloaded earlier to complete the installation of this library.

If you have access to the git command, you can also install it as follows: The following is an example using Windows 10 PowerShell.

```
cd ~
cd Documents\Arduino\libraries
git clone https://github.com/kenichi884/M5StackToio.git
```

For PlatformIO, create a Toio folder under lib in the project folder and place Toio.h, Toio.cpp, ToioCore.h, and ToioCore.cpp in it.

In order to reduce heap memory consumption, the BLE stack has been changed from the default one (Bluedroid) to [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) from v1.0.1.
(There is a difference of about 37KB. If you use it at the same time as WiFi, you will not be able to use it unless you use NimBLE because there is not enough heap. You can also free up more memory by setting nimbleconfig.h.)

Install [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) library

In the Arduino IDE, search for NimBLE-Arduino in the library manager and install it.
For PlatformIO, add the following line to the lib_deps option in the env: section of platformio.ini.
```
h2zero/NimBLE-Arduino@^1.4.1
```

In the default settings of NimBLE, the maximum number of connections that can be made is set to 3.
If you want to connect 4 or more toio core cubes, please change the value of #define CONFIG_BT_NIMBLE_MAX_CONNECTIONS in nimconfig.h.
```
/** @brief Un-comment to change the number of simultaneous connections (esp controller max is 9) */
#define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 3
```

If you want to use the original BLE stack, disable #define USE_NIMBLE 1 in ToioCore.h.
```
#define USE_NIMBLE 1
Change this line like below.
// #define USE_NIMBLE 1
```


---------------------------------------
## <a id="Usage">3. Usage</a>

The sample code below scans for toio core cubes, connects to the first toio core cube it finds, and disconnects after 3 seconds.

```c++
#include <M5Unified.h>
#include <Toio.h>

// Decrare Toio object
Toio toio;

void setup() {
  // Initialize M5Stack 
  M5.begin();
  M5.Power.begin();
  M5.Lcd.clear();

  M5.Log.println("- Scan toio core cubes");
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  size_t n = toiocore_list.size();
  if (n == 0) {
    M5.Log.println("- Not found any toio core cubes.");
    return;
  }
  M5.Log.printf("- %d  toio core cube(s) found.\n", n);

  M5.Log.println("- Establish BLE connection to toio core cube.");
  ToioCore* toiocore = toiocore_list.at(0);
  bool connected = toiocore->connect();
  if (!connected) {
    M5.Log.println("- BLE connection failed.");
    return;
  }
  M5.Log.println("- BLE connection was succeeded.");
  M5.Log.println("- Disconnect after 3 seconds.");
  delay(3000);
  toiocore->disconnect();
  M5.Log.println("- BLE connection was disconnected.");
}

void loop() {

}
```

Before running the sample code above, please power on the Toio Core Cube. If the execution is successful, the following result will be displayed on the Arduino IDE's serial monitor.

```
- Scan toio core cubes
- 1 toio core cube(s) found.
- Establish BLE connection to toio core cube.
- BLE connection was succeeded.
- Disconnect after 3 seconds.
- BLE connection was disconnected.
```

The above sample code is explained below.

### Include library

Please include the header file `Toio.h` of this library in addition to `M5Stack.h` at the beginning of the `.ino` file as shown below.

```c++
#include <M5Unified.h>
#include <Toio.h>
```

Next, create a `Toio` object as a global variable. The following stores a `Toio` object in the variable `toio`.

```c++
Toio toio;
```

### Discover toio core cubes

The rest is executed in the `setup()` function of the `.ino` file.

To discover the toio core cube, call the `scan()` method of the `Toio` object. The `scan()` method specifies the number of seconds to scan. If not specified, the scan will be performed for 3 seconds.

```c++
std::vector<ToioCore*> toiocore_list = toio.scan(3);
```

The `scan()` method performs a scan for the specified number of seconds and then returns a list of discovered toio core cubes. The list type is `std::vector`. The type of object representing an individual toio core cube is a pointer to a `ToioCore` object. Use this `ToioCore` object to manipulate the corresponding toio core cube.

Before moving on, check the number of toio core cubes discovered.

```c++
size_t n = toiocore_list.size();
if(n == 0) {
  M5.Log.println("Not found any toio core cubes.");
  return;
}
ToioCore* toiocore = toiocore_list.at(0);
```

The above code stores a pointer to the `ToioCore` object representing the first toio core cube found in the variable `toiocore`.

### Establish BLE connection to toio core cube.

To make a BLE connection to the discovered toio core cube, call the `connect()` method from the pointer of the `ToioCore` object representing the toio core cube.

```c++
bool connected = toiocore->connect();
if(!connected) {
  M5.Log.println("BLE connection failed.");
  return;
}
```

The `connect()` method returns `true` when the BLE connection is completed. Check this return value.

Once you have established a BLE connection to the toio core cube, operate the toio core cube by calling the methods of the `toioCore` object explained below via the pointer.

### Disconnect connection to the toio core cube

To disconnect a BLE-connected toio core cube, call the `disconnect()` method from the `ToioCore` object's pointer.

```c++
toiocore->disconnect();
```

---------------------------------------
## <a id="Toio-object">4. `Toio` object</a>

Create a `Toio` object as follows. In the code below, the variable `toio` stores a `Toio` object.

```c++
Toio toio;
```

From now on, we will explain that a `Toio` object is stored in the variable `toio`.


### <a id="Toio-scan-method">x `scan()` method (Scan toio core cubes)</a>

The `scan()` method scans the toio core cube for the specified number of seconds.

#### prototype declaration

```c++
std::vector<ToioCore*> scan(uint8_t duration);
```

#### argument

No. | Variable name | Type | Required | Description
:---|:-----------|:----------|:-------|:-------------
1   | `duration` | `uint8_t` | &nbsp; | Scan seconds (default value: 3)

#### code sample

```c++
std::vector<ToioCore*> toiocore_list = toio.scan(3);
```

### <a id="Toio-loop-method">x `loop()` method (Event processing)</a>

The `loop()` method performs event handling. When using the event handler setting function described below, be sure to call it within the `loop()` method of the `.ino` file.

#### prototype declaration

```c++
void loop();
```

#### argument

none

#### code sample

```c++
void loop() {
  M5.update();
  toio.loop();
  ...
}
```

---------------------------------------
## <a id="ToioCore-object">5. `ToioCore` object</a>

As mentioned above, to manipulate the discovered toio core cube, call the methods of the `toioCore` object via the pointer. The methods implemented in the `toioCore` object are explained below. Hereafter, the variable `toiocore` will be a pointer to the `toioCore` object.

### <a id="ToioCore-getAddress-method">x `getAddress()` method (Get Address)</a>

toio コア キューブの MAC アドレスを返します。

#### prototype declaration

```c++
std::string getAddress();
```

#### argument

none

#### code sample

```c++
std::string mac = toiocore->getAddress();
M5.Log.println(mac.c_str()); // example "d1:52:fa:d2:c6:a1"
```

### <a id="ToioCore-getName-method">x `getName()` method (Get device name)</a>

Returns the device name of the toio core cube. The device name is the `localName` value set in the advertising packet by the toio core cube. Usually it is `"toio Core Cube-XXX"`. XXX is a unique cube ID string. 

#### prototype declaration

```c++
std::string getName();
```

#### argument

none

#### code sample

```c++
std::string name = toiocore->getName();
M5.Log.println(name.c_str()); // example "toio Core Cube-a9R"
```

### <a id="ToioCore-connect-method">x `connect()` method (Make a BLE connection)</a>

Make a BLE connection to the toio core cube.

#### prototype declaration

```c++
bool connect();
```

#### argument

none

#### code sample

```c++
bool connected = toiocore->connect();
if (!connected) {
  M5.Log.println("- BLE connection failed.");
  return;
}
```

### <a id="ToioCore-disconnect-method">x `disconnect()` method (Disconnect BLE connection)</a>

Disconnect the BLE connection with the toio core cube.

#### prototype declaration

```c++
void disconnect();
```

#### argument

なし

#### code sample

```c++
toiocore->disconnect();
```

### <a id="ToioCore-isConnected-method">x `isConnected()` method (Get connection status)</a>

Returns the BLE connection status with the toio core cube. Returns `true` if connected, `false` if disconnected.

#### prototype declaration

```c++
bool isConnected();
```

#### argument

none

#### code sample

```c++
if(toiocore->isConnected()) {
  M5.Log.println("Connected");
} else {
  M5.Log.println("Disconnected");
}
```

### <a id="ToioCore-onConnection-method">x `onConnection()` method (Set callback for connection state event)</a>

Sets the callback for the BLE connection state event with the toio core cube. If there is a change in the BLE connection status, call the callback function specified as an argument. The callback function is passed a bool value representing the connection state.。

#### prototype declaration

```c++
typedef std::function<void(bool connected)> OnConnectionCallback;
void onConnection(OnConnectionCallback cb);
```

#### argument

No. | Variable name | Type | Required | Description
:---|:--------|:-----------------------|:-------|:-------------
1   | `cb`    | `OnConnectionCallback` | x     | callback function

#### code sample

The sample sketch below repeatedly connects and disconnects BLE every 10 seconds. The callback function outputs the connection state when a connection state event occurs.

To use a callback, call the [`loop()`](#Toio-loop-method) method of the `Toio` object within the `loop()` function in the `.ino` file. Note that callbacks can only handle events that occur after the `loop()` function in the `.ino` file starts executing. Therefore, in the sample sketch below, BLE connection and disconnection are performed within the `loop()` function.

```c++
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
Toio toio;

// ToioCore object pointer
ToioCore* toiocore = nullptr;

// BLE connection/disconnection timestamp
unsigned long conn_time = millis();

void setup() {
  // MInitialize 5Stack
  M5.begin();
  M5.Power.begin();

  // Start scanning toio core cubes
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() > 0) {
    toiocore = toiocore_list.at(0);
  }

  // Set callback for BLE connection state event
  toiocore->onConnection([](bool state) {
    M5.Log.println(state ? "connected" : "disconnected");
  });
}

void loop() {
  // When using a callback, always call loop() on the Toio object
  toio.loop();

  if (toiocore) {
    // Repeats BLE connection/disconnection every 10 seconds
    unsigned long now = millis();
    if (now - conn_time >= 10000) {
      if (toiocore->isConnected()) {
        toiocore->disconnect();
      } else {
        toiocore->connect();
      }
      conn_time = now;
    }
  }
}
```

### <a id="ToioCore-getBleProtocolVersion-method">x `getBleProtocolVersion()` method (Get BLE protocol version)</a>

Get the BLE protocol version of the toio core cube.

#### prototype declaration

```c++
std::string getBleProtocolVersion();
```

#### argument

none

#### code sample

```c++
std::string ble_ver = toiocore->getBleProtocolVersion();
M5.Log.println(ble_ver.c_str()); // example "2.3.0"
```

### <a id="ToioCore-playSoundEffect-method">x `playSoundEffect()` method (Play sound effect)</a>

Play preset sound effects on toio core cube.

#### prototype declaration

```c++
void playSoundEffect(uint8_t sound_id, uint8_t volume = 0xff);
```

#### argument

No. | Variable name | Type | Required | Description
:---|:-----------|:----------|:-------|:-------------
1   | `sound_id` | `uint8_t` | x     | Sound effect ID (0 - 10)
2   | `volume`   | `uint8_t` | &nbsp; | Sound volume (0 - 255)。255 if not specified.。

The meaning of the sound effect ID is as follows.

 ID |  name
:---------|:---------------
`0`       | Enter
`1`       | Selected
`2`       | Cancel
`3`       | Cursor
`4`       | Mat in
`5`       | Mat out
`6`       | Get 1
`7`       | Get 2
`8`       | Get 3
`9`       | Effect 1
`10`      | Effect 2

#### code sample

```c++
toiocore->playSoundEffect(3);
```

### <a id="ToioCore-playSoundRaw-method">x `playSoundRaw()` method  (Play MIDI note numbers)</a>

Writes the raw data defined in the toio core cube communication specification to the sound characteristic. It is mainly used to play MIDI note numbers. For more information, see [toio core cube sound specifications](https://toio.github.io/toio-spec/docs/ble_sound).

#### prototype declaration

```c++
void playSoundRaw(uint8_t* data, size_t length);
```

#### argument

No. |  Variable name    |  Type         |  Required   |  Description
:---|:---------|:-----------|:-------|:-------------
1   | `data`   | `uint8_t*` | x     | Byte string of data to write
2   | `length` | `size_t`   | x     | Number of bytes to write
#### code sample

The sample code below plays [Charumera](https://www.myojofoods.co.jp/charumera/play/index.html) at maximum volume.

```c++
uint8_t charumera_data[39] = {
  3,             // Control type (`3` for Play MIDI note number)
  1,             // Number of repetitions
  12,            // Number of operations
  14,  69, 255,  // 140ms, A5
  14,  71, 255,  // 140ms, B5
  56,  73, 255,  // 560ms, C#6
  14,  71, 255,  // 140ms, B5
  14,  69, 255,  // 140ms, A5
  114, 128, 255, // Rest
  14,  69, 255,  // 140ms, A5
  14,  71, 255,  // 140ms, B5
  14,  73, 255,  // 560ms, C#6
  14,  71, 255,  // 140ms, B5
  14,  69, 255,  // 140ms, A5
  56,  71, 255   // 560ms, B5
};
toiocore->playSoundRaw(charumera_data, 39);
```

### <a id="ToioCore-stopSound-method">x `stopSound()` method (Stop playing)</a>

Stop sound playing.

#### prototype declaration

```c++
void stopSound();
```

#### argument

なし

#### code sample

```c++
toiocore->stopSound();
```

### <a id="ToioCore-turnOnLed-method">x `turnOnLed()` method (Turn on the LED)</a>

Light up the LED(Indicator) on the toio core cube.

#### prototype declaration

```c++
void turnOnLed(uint8_t r, uint8_t g, uint8_t b);
```

#### argument

No. |  Variable name |  Type        |  Required   |  Description
:---|:------|:----------|:-------|:-------------
1   | `r`   | `uint8_t` | x     | Value for red(`0` - `255`)
2   | `g`   | `uint8_t` | x     | Value for green(`0` - `255`)
2   | `b`   | `uint8_t` | x     | Value for blue(`0` - `255`)

#### code sample

```c++
toiocore->turnOnLed(0xff, 0xff, 0x00); // Yellow
```

Note: Specifying black (`0x00, 0x00, 0x00`) turns it off.

### <a id="ToioCore-turnOffLed-method">x `turnOffLed()` method (Turn off LED)</a>

Turn off the toio core cube LED(Indicator).

#### prototype declaration

```c++
void turnOffLed();
```

#### argument

none

#### code sample

```c++
toiocore->turnOffLed();
```

### <a id="ToioCore-getBatteryLevel-method">x `getBatteryLevel()` method (Get battery level)</a>

Get the battery level (%) of toio core cube.

#### prototype declaration

```c++
uint8_t getBatteryLevel();
```

#### argument

なし

#### code sample

```c++
uint8_t batt_level = toiocore->getBatteryLevel();
M5.Log.printf("%d psercent\n", batt_level);
```

### <a id="ToioCore-onBattery-method">x `onBattery()` method (Set callback for battery event)</a>

Sets the callback for the toio core cube's battery event. Calls the callback function specified as an argument at 5 second intervals. The callback function is passed the battery level (in percent).

#### prototype declaration

```c++
typedef std::function<void(uint8_t level)> OnBatteryCallback;
void onBattery(OnBatteryCallback cb);
```

#### argument

No. |  Variable name   |  Type                  |  Required   |  Description
:---|:--------|:--------------------|:-------|:-------------
1   | `cb`    | `OnBatteryCallback` | x     | Callbakc function

#### code sample

The sample sketch below outputs the battery level every time a battery event is received.

To use a callback, call the [`loop()`](#Toio-loop-method) method of the `Toio` object within the `loop()` function in the `.ino` file. Note that callbacks can only handle events that occur after the `loop()` function in the `.ino` file starts executing.


```c++
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
Toio toio;

void setup() {
  // Initialize M5Stack
  M5.begin();
  M5.Power.begin();

  // Scan Toio Core Cubes in 3 seconds.
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // Start BLE connection.
  toiocore->connect();

  // Set callback for battery event
  toiocore->onBattery([](uint8_t level) {
    M5.Log.printf("%d パーセント\n", level);
  });
}

void loop() {
  // When using a callback, always call loop() on the Toio object.
  toio.loop();
}
```

### <a id="ToioCore-getButtonState-method">x `getButtonState()` method (Get button state)</a>

Gets the pressed state of the button on the bottom of the toio core cube (combined with the indicator). Returns `true` if pressed, otherwise returns `false`.

#### prototype declaration

```c++
bool getButtonState();
```

#### argument

none

#### code sample

```c++
if(toiocore->getButtonState()) {
  M5.Log.println("button is pressed.");
} else {
  M5.Log.println("No buttons are pressed.");
}
```

### <a id="ToioCore-onButton-method">x `onButton()` method (Set button event callback)</a>

Sets the callback for the toio core cube's button press event. If there is a change in the button press status, calls the callback function specified as an argument. A bool value representing the button press status is passed to the callback function.

#### prototype declaration

```c++
typedef std::function<void(bool state)> OnButtonCallback;
void onButton(OnButtonCallback cb);
```

#### argument

No. |  Variable name   |  Type                 |  Required   |  Description
:---|:--------|:-------------------|:-------|:-------------
1   | `cb`    | `OnButtonCallback` | x     | Callback function

#### code sample

The sample sketch below outputs the state each time there is a change in the button press state.

To use a callback, call the [`loop()`](#Toio-loop-method) method of the `Toio` object within the `loop()` function in the `.ino` file. Note that callbacks can only handle events that occur after the `loop()` function in the `.ino` file starts executing.


```c++
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
Toio toio;

void setup() {
  // Initialize M5Stack
  M5.begin();
  M5.Power.begin();

  // Scan Toio Core Cubes in 3 seconds.
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // Start BLE connection.
  toiocore->connect();

  // Set button state change event callback.
  toiocore->onButton([](bool state) {
    M5.Log.println(state ? "button press" : "button release");
  });
}

void loop() {
  // When using a callback, always call loop() on the Toio object
  toio.loop();
}
```

### <a id="ToioCore-getMotion-method">x `getMotion()` method (Get motion sensor status)</a>

Get the state of the toio core cube's motion sensor.

#### prototype declaration

```c++
struct ToioCoreMotionData {
  bool flat;
  bool clash;
  bool dtap;
  uint8_t attitude;
  uint8_t shake;
};

ToioCoreMotionData getMotion();
```

#### argument

none

#### Return value

A structure of type `ToioCoreMotionData` is returned. The meaning of each member is as follows.

Name       |  Type        |  Description
:----------|:----------|:------------------------
`flat`     | `bool`    | Horizontal detection (`true`: Horizontal, `false`: Not horizontal) 
`clash`    | `bool`    | Collision detection (`true`: Collision detected, `false`: No collision)
`dtap`     | `bool`    | Double-tap detection (`true`: Double-tap detected, `false`: Not double-tap)
`attitude` | `uint8_t` | Posture detection (Described later)
`shake`    | `uint8_t` | Shake detection (0x00 no detection, 0x01 - 0x0a strength of the shake)

Attitude detection The values taken by `attitude` and their meanings are as follows.

Value  | Cube posture
:---|:-----------------------
`1` | Top faces upward
`2` | Bottom faces upward
`3` | Rear side faces upward
`4` | Front side faces upward
`5` | Right side faces upward
`6` | Left side faces upward

For details on posture detection, please see [toio Core Cube Technical Specifications](https://toio.github.io/toio-spec/docs/ble_sensor).


#### code sample

```c++
ToioCoreMotionData motion = toiocore->getMotion();
M5.Log.printf("- Horizontal detection: %s\n", motion.flat ? "Horizontal" : "Not horizontal");
M5.Log.printf("- Collision detection: %s\n", motion.clash ? "Detect" : "None");
M5.Log.printf("- Double-tap detection: %s\n", motion.dtap ? "Detect" : "None");
M5.Log.printf("- Posture detection: %d\n",  motion.attitude);
M5.Log.printf("- Shake detection: %d\n",  motion.shake);
```

### <a id="ToioCore-onMotion-method">x `onMotion()` method (Set motion sensor callback)</a>

toio Core Sets the cube motion event callback. If there is a change in motion, call the callback function specified as an argument. A structure representing the motion is passed to the callback function.

#### prototype declaration

```c++
struct ToioCoreMotionData {
  bool flat;
  bool clash;
  bool dtap;
  uint8_t attitude;
  uint8_t shake;
};

typedef std::function<void(ToioCoreMotionData motion)> OnMotionCallback;
void onMotion(OnMotionCallback cb);
```

#### argument

No. |  Variable name   |  Type                 |  Required   |  Description
:---|:--------|:-------------------|:-------|:-------------
1   | `cb`    | `OnMotionCallback` | x     | Callback function

#### code sample

The sample sketch below outputs the state each time there is a change in motion.

To use a callback, call the [`loop()`](#Toio-loop-method) method of the `Toio` object within the `loop()` function in the `.ino` file. Note that callbacks can only handle events that occur after the `loop()` function in the `.ino` file starts executing.

```c++
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
Toio toio;

void setup() {
  // Initialize M5Stack
  M5.begin();
  M5.Power.begin();

  // Scan Toio Core Cubes in 3 seconds.
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // Start BLE connection.
  toiocore->connect();

  // Set motion event callback
  toiocore->onMotion([](ToioCoreMotionData motion) {
    M5.Log.println("------------------------------------------");
    M5.Log.printf("- Horizontal detection: %s\n", motion.flat ? "Horizontal" : "Not horizontal");
    M5.Log.printf("- Collision detection: %s\n", motion.clash ? "Detect" : "None");
    M5.Log.printf("- Double-tap detection: %s\n", motion.dtap ? "Detect" : "None");
    M5.Log.printf("- Posture detection: %d\n",  motion.attitude);
    M5.Log.printf("- Shake detection: %d\n",  motion.shake);
  });
}

void loop() {
  // When using a callback, always call loop() on the Toio object
  toio.loop();
}
```

### <a id="ToioCore-controlMotor-method">x `controlMotor()` method (Control motor)</a>

Controls the motor of the toio core cube.

#### prototype declaration

```c++
void controlMotor(bool ldir, uint8_t lspeed, bool rdir, uint8_t rspeed, uint16_t duration = 0);
```

#### argument

No. |  Variable name      |  Type         |  Required   |  Description
:---|:-----------|:-----------|:-------|:-------------
1   | `ldir`     | `bool`     | x     | Left motor rotation direction (`true`: Forward, `false`: Backward)
2   | `lspeed`   | `uint8_t`  | x     | Left motor speed (`0` - `115`)
3   | `rdir`     | `bool`     | x     | Right motor rotation direction (`true`: Forward, `false`: Backward)
4   | `rspeed`   | `uint8_t`  | x     | Right motor speed (`0` - `115`)
5   | `duration` | `uint16_t` | &nbsp; | Motor control duration (milli second)

For the correspondence between `lspeed` and `rspeed` values and actual speeds, please see [toio core cube communication specification](https://toio.github.io/toio-spec/docs/ble_motor).

If you specify `0` (default value) for `duration`, it will continue to move until it receives a command to stop (set `lspeed`, `rspeed` to `0`).

#### code sample

```c++
// Proceed while turning to the right
toiocore->controlMotor(true, 50, true, 40);
delay(5000);

// Stop
displayProgressMessage("Stopping...");
toiocore->controlMotor(true, 0, true, 0);
delay(5000);

// Proceed while turning left for 2 seconds.
toiocore->controlMotor(true, 40, true, 50, 2000);
delay(5000);
```

### <a id="ToioCore-drive-method">x `drive()` method (Driving like a car)</a>

This is a motor control method intended for use with a gamepad or joystick. Replaces motor control with throttle and steering operations.

#### prototype declaration

```c++
void drive(int8_t throttle, int8_t steering);
```

#### argument

No. | Variable name | Type | Required | Description
:---|:-----------|:----------|:-------|:-------------
1   | `throttle` | `int8_t`  | x     | Throtttle (`-100` - `100`)
2   | `steering` | `int8_t`  | x     | Steering (`-100` - `100`)

Specifying a positive value for `throttle` will move forward, specifying a negative value will move backward, and specifying `0` will stop. A positive value for `steering` will cause it to turn to the right, a negative value will cause it to turn to the left.

Steering operation is achieved by slowing down the rotation speed of one motor as the absolute value of `steering` increases. If you specify `100` for `throttle` and `100` for `steering`, the speed of the left motor will remain `100` and the speed of the right motor will be `0`. In other words, it will spin around to the left around the right tire. If you want movement like a radio-controlled car, it is a good idea to keep the `steering` limit to around `50`.

#### code sample

```c++
// Slowly turn left
toiocore->drive(20, -10);
delay(5000);

// Stop
toiocore->drive(0, 0);
```

If you want to rotate the left and right tires in opposite directions and rotate around the center of the body like a tank, use the [`controlMotor()`](#ToioCore-controlMotor-method) method described above. please use it。

### <a id="ToioCore-controlMotorWithTarget-method">x `controlMotorWithTarget()` method (Motor control with target specified)</a>

This is a method to specify one target point, control the motor, and move the toio Core Cube autonomously.

#### prototype declaration

```c++
void controlMotorWithTarget(uint8_t distinction, uint8_t timeout, uint8_t movement_type, 
  uint8_t maximum_speed, uint8_t speed_change_type,
  uint16_t target_x, uint16_t target_y, 
  uint16_t target_angle_degree, uint8_t target_angle_and_rotation_bits = 0);
```

#### argument

No. |  Variable name         |  Type        |  Required   |  Description
:---|:---------------|:----------|:-------|:-------------
1   | `distinction`  | `uint8_t`  | x     | Control distinction value
2   | `timeout`      | `uint8_t`  | x     | Timeout period(sec)
3   | `movement_type` | `uint8_t`  | x     | Movement type (`0` - `2`)
4   | `maximum_speed` | `uint8_t`  | x     | Value indicating maximum motor speed (`10` - `2550`)
5   | `speed_change_type` | `uint8_t`  | x | Motor speed change types (`0` - `3`)
6   | `target_x`     | `uint16_t`  | x     | X coordinate value of the target point (`0` - `65535`)
7   | `target_y`     | `uint16_t`  | x     | Y coordinate value of the target point (`-0` - `65535`)
8   | `target_angle_degree` | `uint16_t`  | x     | Θ angle of the cube at the target point  (`0` - `0x1fff`) 
9   | `target_angle_and_rotation_bits` | `uint8_t`  | &nbsp;  |Angle meaning and Direction of rotation (`0` - `6`)

#### code sample

```c++
// Move at a speed of 80 so as to stop at the position of coordinates (150, 200) and the direction of angle 0
toiocore->controlMotorWithTarget(1, 5, 0, 80, 0, 150, 200, 0);
delay(5000);
```

## <a id="ToioCore-controlMotorWithMultipleTargets-method">x `controlMotorWithMultipleTargets()` method (Motor control with multiple targets specified)</a>

This method specifies multiple target points and controls the motor to move the toio Core Cube autonomously.

#### prototype declaration

```c++
struct ToioCoreTargetPos {

  uint16_t posX; // X coordinate value of the target point
  uint16_t posY; // Y coordinate value of the target point
  uint16_t angleDegree:13; //  Θ angle of the cube at the target point (0 - 0x1fff)
  uint8_t angleAndRotation:3; // Angle meaning and Direction of rotation (0 - 6)
};

void controlMotorWithMultipleTargets(uint8_t distinction, uint8_t timeout, uint8_t movement_type, 
      uint8_t maximum_speed, uint8_t speed_change_type,  uint8_t addition_setting,
      uint8_t target_num, ToioCoreTargetPos *target_positions);
```

#### argument

No. |  Variable name         |  Type        |  Required   |  Description
:---|:---------------|:----------|:-------|:-------------
1   | `distinction`  | `uint8_t`  | x     | Control distinction value
2   | `timeout`      | `uint8_t`  | x     | Timeout period (sec)
3   | `movement_type` | `uint8_t`  | x     | Movement type (`0` - `2`)
4   | `maximum_speed` | `uint8_t`  | x     | Value indicating maximum motor speed (`10` - `2550`)
5   | `speed_change_type` | `uint8_t`  | x | Motor speed change types (`0` - `3`)
6   | `addition_setting` | `uint8_t`  | x | Write operation addition setting (`0` - `1`)
7   | `target_num`     | `uint16_t`  | x     | Number of target points (`1` - `29`)
8   | `target_positions` | `ToioCoreTargetPos *`  | x     | Pointer to array of target coordinates


#### code sample

```c++
// Move by specifying 5 target points.
  ToioTargetPos pos[5];
  pos[0].x = 250;
  pos[0].y = 250;
  pos[0].angle = 0;
  pos[1].x = 250;
  pos[1].y = 300;
  pos[1].angle = 90;
  pos[2].x = 300;
  pos[2].y = 300;
  pos[2].angle = 180;
  pos[3].x = 300;
  pos[3].y = 250;
  pos[3].angle = 270;
  pos[4].x = 250;
  pos[4].y = 250;
  pos[4].angle = 0;
  toiocore->controlMotorWithMultipleTargets(0, 0, 0, 50, 0, 0, 5, pos);
  delay(5000);
```

### <a id="ToioCore-controlMotorWithAcceleration-method">x `controlMotorWithAcceleration()` method (Motor control with acceleration specified)</a>

This is a method to control the motor by specifying the acceleration of the toio core cube.

#### prototype declaration

```c++
void controlMotorWithAcceleration(uint8_t translational_speed, uint8_t acceleration,
  uint16_t rotational_velocity, uint8_t rotational_direction, uint8_t travel_direction,
  uint8_t priority, uint8_t duration);
```

#### argument

No. |  Variable name         |  Type        |  Required   |  Description
:---|:---------------|:----------|:-------|:-------------
1   | `translational_speed`  | `uint8_t`  | x     | Translational speed of cube(`0` - `255`)
2   | `acceleration` | `uint8_t`  | x     | Acceleration of cube(increment (or decrement) in speed every 100 milliseconds. `0` - `255`)
3   | `rotational_velocity` | `uint16_t`  | x     | Rotational velocity when cube changes orientation (degree/second `0` - `65535`)
4   | `rotational_direction` | `uint8_t`  | x     | Rotational direction when cube changes orientation	 (`0` Forward - `1` Backward)
5   | `travel_direction` | `uint8_t`  | x | Direction of cube travel (`0` Forward - `1` Backward)
6   | `priority`     | `uint8_t`  | x     | Priority designation(`0` the translational speed and adjusts the rotational velocity  - `1` the rotational velocity and adjusts the translational speed)
7   | `duration`     | `uint8_t`  | x     | Duration of control  10 times the specified value in milliseconds, A value of 0 specifies for there to be no time limit (`0` - `255`)

#### code sample

```c++
// Move in a slow arc for 2 seconds.
toiocore->controlMotorWithAcceleration(50, 15, 30, 0, 0, 0, 200);
delay(5000);
```

### <a id="ToioCore-onMotor-method">x `onMotor()` method (Set response callback for motor control)</a>

Sets the callback for the toio core cube's motor control response event.
Either the response of motor control with target specification, response of motor control with multiple target specification, or motor speed information (disabled by default) is returned.

#### prototype declaration

```c++
// Response to motor control
struct ToioCoreMotorResponse {
  uint8_t controlType; // Control type
  union {
    uint8_t controlID; // Control identification value
    uint8_t leftSpeed; // or Left motor speed
  };
  union {
   uint8_t response; // or Response content
   uint8_t rightSpeed;  // or Right motor speed
  };
};

typedef std::function<void(ToioCoreMotorResponse motor_response)> OnMotorCallback;
void onMotor(OnMotoraCallback cb);
```

#### argument

No. |  Variable name   |  Type                 |  Required   |  Description
:---|:--------|:-------------------|:-------|:-------------
1   | `cb`    | `OnMotorCallback` | x     | Callback function

#### code sample

The sample sketch below obtains either the response of motor control with target specification, the response of motor control with multiple target specification, or motor speed information.

To use a callback, call the [`loop()`](#Toio-loop-method) method of the `Toio` object within the `loop()` function in the `.ino` file. Note that callbacks can only handle events that occur after the `loop()` function in the `.ino` file starts executing.

```c++
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
Toio toio;

void setup() {
  // Initialize M5Stack
  M5.begin();

  // Scan Toio Core Cubes in 3 seconds.
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // Start BLE connection.
  toiocore->connect();

  // Set callback for motor control response event
  toiocore->onMotor([](ToioCoreMotorResponse motor_res) {
    M5.Log.printf("Motor Event ");
    M5.Log.print(f"Type=%u", motor_res.controlType);
    if(motor_res.controlType == 0xe0 )
      M5.Log.printf( ", left speed=%u, right speed=%u\n", motor_res.leftSpeed, motor_res.rightSpeed);
    else
      M5.Log.printf( ", controlID=%u, response=%u\n", motor_res.controlID, motor_res.response);
  });  
}

void loop() {
  // When using a callback, always call loop() on the Toio object
  toio.loop();
}
```

### <a id="ToioCore-getIDReaderData-method">x `getIDReaderData()` method (Get the ID read sensor status, Get the position and angle on the mat)</a>

Get the ID read sensor status of the toio core cube. When the cube is on the mat, the value (ID) read by the sensor can be obtained from the mat. The IDs that can be read from the mat include the position ID, which represents the coordinate values ​​on the mat, and the standard ID, which represents the area on which the cube is placed, and either information will be returned.

#### prototype declaration

```c++

struct ToioCoreIDData {
  ToioCoreIDType type; // Information type
  union {
    ToioCorePositionIDData position; // Position X-Y coordinate values and cube angle
    ToioCoreStandardIDData standard; // Standard ID value and cube angle
  };
};

struct ToioCorePositionIDData {
  uint16_t cubePosX; // X coordinate value of the center of the cube
  uint16_t cubePosY; // Y coordinate value of the center of the cube
  uint16_t cubeAngleDegree; // Angle value of the center of the cube
  uint16_t sensorPosX; // X coordinate value of the identification sensor
  uint16_t sensorPosY; // Y coordinate value of the identification sensor
  uint16_t sensorAngleDegree; // Angle value of the identification sensor
};

struct ToioCoreStandardIDData {
  uint32_t standardID; // Standard ID value
  uint16_t cubeAngleDegree; // Cube angle
};

ToioCoreIDData getIDReaderData();
```

#### argument

none

#### Returen values

A structure of type `ToioCoreIDData` is returned. The meaning of each member is as follows.

Name       |  Type        |  Description
:----------|:----------|:------------------------
`type`     | `ToioCoreIDType`    | Information type (`ToioCoreIDTypeNone`: If it cannot be read, `ToioCoreIDTypePosition`: For position ID, `ToioCoreIDTypeStandard`: For standard ID) 
`position`    | `ToioCorePositionIDData`    | Position ID information
`standard`     | `ToioCoreStandardIDData`    | Standard ID information

If type is ToioCoreIDTypePosition, the position value returns a structure of type `ToioCorePositionIDData`. The meaning of each member is as follows.


Name       |  Type        |  Description
:----------|:----------|:------------------------
`cubePosX`     | `uint16_t`    | X coordinate value of the center of the 
`cubePosY`     | `uint16_t`    | Y coordinate value of the center of the 
`cubeAngleDegree`     | `uint16_t`    | Angle value of the center of the cube (degree)
`sensorPosX`     | `uint16_t`    | X coordinate value of the identification sensor
`sensorPosY`     | `uint16_t`    | Y coordinate value of the identification sensor
`sensorAngleDegree`     | `uint16_t`    | Angle value of the identification sensor (degree)

If type is ToioCoreIDTypeStandard, the standard value returns a structure of type `ToioCoreStandardIDData`. The meaning of each member is as follows.

Name       |  Type        |  Description
:----------|:----------|:------------------------
`standardID`     | `uint32_t`    | Standard ID value
`cubeAngleDegree`     | `uint16_t`    | Cube angle (degree)


For detailed specifications of position and standard values, see [toio core cube communication specification](https://toio.github.io/toio-spec/docs/ble_id).

For Standard ID values, please see [Standard ID list](https://toio.github.io/toio-spec/docs/hardware_standard_id).

#### code sample

```c++
ToioCoreIDData data = toiocore->getIDReaderData();
if (data.type == ToioCoreIDTypePosition) {
  M5.Log.printf("posX: %u posY: %u angle: %u\n",
    data.position.cubePosX, data.position.cubePosY, data.position.cubeAngleDegree);
}
else if (data.type == ToioCoreIDTypeStandard) {
  M5.Log.printf("Standard ID: %u\n", data.standard.standardID);
}
else {
  M5.Log.println("no id found.");
}
```

### <a id="ToioCore-onIDReaderData-method">x `onIDReaderData()` methodド (Set callback for ID reading sensor)</a>

Sets the callback for the ID read event of the toio core cube. Calls the callback function specified as an argument at regular intervals while the ID is being read, and when the ID reading is finished (failed). A structure representing the read ID is passed to the callback function.

#### prototype declaration

```c++
struct ToioCoreIDData {
  ToioCoreIDType type; // Information type
  union {
    ToioCorePositionIDData position; // Position X-Y coordinate values and cube angle
    ToioCoreStandardIDData standard; // Standard ID value and cube angle
  };
};

struct ToioCorePositionIDData {
  uint16_t cubePosX; // X coordinate value of the center of the cube
  uint16_t cubePosY; // Y coordinate value of the center of the cube
  uint16_t cubeAngleDegree; // Angle value of the center of the cube
  uint16_t sensorPosX; // X coordinate value of the identification sensor
  uint16_t sensorPosY; // Y coordinate value of the identification sensor
  uint16_t sensorAngleDegree; // Angle value of the identification sensor
};

struct ToioCoreStandardIDData {
  uint32_t standardID; // Standard ID value
  uint16_t cubeAngleDegree; // Cube angle
};

typedef std::function<void(ToioCoreIDData id_data)> OnIDDataCallback;
void onIDReaderData(OnIDDataCallback cb);
```

#### argument

No. |  Variable name   |  Type                 |  Required   |  Description
:---|:--------|:-------------------|:-------|:-------------
1   | `cb`    | `OnIDDataCallback` | x     | Callback function

#### code sample

The sample sketch below will output the status each time the ID is read or the ID reading is finished (failed).

To use a callback, call the [`loop()`](#Toio-loop-method) method of the `Toio` object within the `loop()` function in the `.ino` file. Note that callbacks can only handle events that occur after the `loop()` function in the `.ino` file starts executing.

```c++
#include <M5Unified.h>
#include <Toio.h>

// Declare a toio object
Toio toio;

void setup() {
  // Initialize M5Stack
  M5.begin();
  M5.Power.begin();

  // Scan Toio Core Cubes in 3 seconds.
  std::vector<ToioCore*> toiocore_list = toio.scan(3);
  if (toiocore_list.size() == 0) {
    return;
  }
  ToioCore* toiocore = toiocore_list.at(0);

  // Start BLE connection.
  toiocore->connect();

  // Set callback for ID read event
  toiocore->onIDReaderData([](ToioCoreIDData data) {
    if (data.type == ToioCoreIDTypePosition) {
      M5.Log.printf("posX: %u posY: %u angle: %u\n",
        data.position.cubePosX, data.position.cubePosY, data.position.cubeAngleDegree);
    }
    else if (data.type == ToioCoreIDTypeStandard) {
      M5.Log.printf("Standard ID: %u\n", data.standard.standardID);
    }
    else {
      M5.Log.println("no id found.");
    }
  });
}

void loop() {
  // When using a callback, always call loop() on the Toio object
  toio.loop();
}
```

---------------------------------------
## <a id="Sample-Sketches">6. Sample sketches</a>

Once the installation of this library is complete, you will be able to select `M5StackToio` from `File` -> `Sketch Example` in the Arduino IDE menu bar. The following six samples are provided.

Among these, `basic`, `event`, and `joystick_drive` are futomi-san's original sample codes [M5Stack Basic](https://www.switch-science.com/catalog/3647/) and [M5Stack Gray](https ://www.switch-science.com/catalog/3648/).

`basic_test`, `event_test`, `event2_test`, and `multi_corecube_test` are newly added sample codes that use the M5Unified library, so they will work with any M5Stack controller product that has one or more buttons. (The output results are output to the serial port instead of the LCD.)

### `basic_test`

This is a sketch that executes all the methods provided by this library, similar to the original sample sketch `basic`.

Please turn on the toio Core Cube in advance.

When M5Stack starts, it automatically scans the toio core cube. If the toio core cube is found, the device name and MAC address will be output to the serial port. If it is not found, press the reset button on the M5Stack button again to restart.

Once the toio Core Cube is discovered, it automatically connects to BLE and executes a series of methods sequentially, displaying the results on the serial port.

If you want to test movement with a target point, please prepare the simple playmat that comes with the toio Core Cube (single package) or change the coordinates of the target point to coordinates that are compatible with the playmat you have.
https://toio.github.io/toio-spec/docs/hardware_position_id

When the execution of a series of methods is completed, the BLE connection is automatically disconnected and terminated.

### `event_test`

Similar to the original sketch `event`, this is a sample sketch that handles the events provided by this library.
However, please note that the arduino ESP32 only allows up to 4 notification registrations, so you can only set callbacks that handle up to 4 events at the same time. (See also source code comments)
Important note: In the case of NimBLE, it seems that more than 5 callbacks can be set.

Please turn on the toio Core Cube in advance.

When M5Stack starts, it automatically scans the toio core cube. If the toio core cube is found, the device name and MAC address will be output to the serial port. If it is not found, press the reset button on the M5Stack button again to restart.

After the toio core cube is discovered, it listens for various events and displays the received event information on the serial port.
When you press the A button on M5Stack, the toio Core Cube moves to the specified target point, and when the movement is completed, it receives a Motor event and displays the result on the serial port. When performing this test, please prepare the simple playmat that comes with the toio Core Cube (single package) or change the coordinates of the target point to those that correspond to the playmat you have.
https://toio.github.io/toio-spec/docs/hardware_position_id

Pressing the A button on M5Stack for more than 2 seconds will connect/disconnect the BLE connection with toio Core Cube.

### `event2_test`

This is a sketch to test functions that are off by default (change ID notification interval, magnetic sensor, attitude angle notification, motor speed notification).

Please turn on the toio Core Cube in advance.

When M5Stack starts, it automatically scans the toio core cube. If the toio core cube is found, the device name and MAC address will be displayed in the serial port log. If it is not found, press the reset button on the M5Stack button again to restart.

When the connection is completed, the functions that are off by default (change ID notification interval, using the magnetic sensor, attitude angle notification, motor speed notification) are turned on, and each event is waited for, and the received event information is sent to the serial port. will be displayed in the log.

After the toio core cube is connected, press the A button on the M5Stack and the toio core cube will move under acceleration specified motor control. (You will be notified of speed changes.)

Pressing the A button on M5Stack for more than 2 seconds will disconnect or reconnect the BLE connection with the toio Core Cube.

Please note that the default BLE library of arduino ESP32 only works with up to 4 notify registrations, so you can only set up to 4 callbacks to handle events at the same time. (Motion, magnetism, and posture angle use the same characteristic, so they count as one.)

Note: In the case of NimBLE, it seems that more than 5 callbacks can be set.

### `multi_corecube_test`

This is a sample sketch that connects multiple toio Core Cubes, controls them, and receives events.

You can connect and move 2 to 6 toio core cubes.
Since NimBLE's default settings only allow up to three connections, change the value of #define CONFIG_BT_NIMBLE_MAX_CONNECTIONS in nimconfig.h to 6. (It seems that up to 9 can be connected with ESP32, but I have only 6 Toio Core Cubes so I haven't been able to try it.)
It seems that the default BLE library of arduino ESP32 can only connect up to 4.

Please turn on the Toio Core Cube and place it on the mat in advance. (It works with the simple play mat that comes with the toio Core Cube (single package). If you use another mat, please change the values of MAT_CENTER_X and MAT_CENTER_Y)

When M5Stack starts, it will automatically scan Toio Core Cube.
If toio Core Cube is found, the device name and MAC address will be displayed in the M5Stack serial port log. If it is not found, press the reset button on M5Stack again to restart it.

Once the connection with toio Core Cube is established, the indicator (LED) of Toio Core Cube will be lit constantly.
After connecting all Toio Core Cubes, press the A button on M5Stack to move to the position corresponding to the number of Toio Core Cubes (3 vertices of a triangle, 4 squares, 6 hexagons) Move. (The Toio Core Cube may collide depending on the position you place it for the first time, so please change the place by hand.)
Toio Core Cube changes its position every time you press the A button on M5Stack.
  
TWhen you press a button on a Toio Core Cube, the pressed Toio Core Cube will play a sound effect, and the unpressed Toio Core Cubes will rotate for 300ms.

---------------------------------------
Below is futomi-san's original sample code
In order to display the operation log on the M5Stack screen, please use the specified M5Stack controller product that has a screen.

### `basic`

This is a sketch that executes all the methods provided by this library.

Please turn on the toio Core Cube in advance.

When M5Stack starts, it automatically scans the toio core cube. When the toio core cube is found, the device name and MAC address will be displayed on the M5Stack screen. If it is not found, press the reset button on the M5Stack button again to restart.

When the toio Core Cube is discovered, it automatically connects with BLE and executes a series of methods sequentially, displaying the results on the M5Stack screen.

When the execution of a series of methods is completed, the BLE connection is automatically disconnected and terminated.

### `event`

This is a sample sketch that handles the events provided by this library.

Please turn on the toio Core Cube in advance.

When M5Stack starts, it automatically scans the toio core cube. When the toio core cube is found, the device name and MAC address will be displayed on the M5Stack screen. If it is not found, press the reset button on the M5Stack button again to restart.

After the toio Core Cube is discovered, press the A button on M5Stack to connect to the toio Core Cube via BLE. Once the connection is complete, it will wait for various events and display the received event information on the M5Stack screen.

### `joystick_drive`

This is a sample sketch of driving the toio core cube using the [joystick unit for M5Stack](https://www.switch-science.com/catalog/4050/).

  Please turn on the toio Core Cube in advance.

When M5Stack starts, it automatically scans the toio core cube. When the toio core cube is found, the device name and MAC address will be displayed on the M5Stack screen. If it is not found, press the reset button on the M5Stack button again to restart.

You need to calibrate the joystick before connecting BLE with toio Core Cube. The joystick coordinates will be displayed on the M5Stack screen. If the x and y values are not 0, the joystick center coordinates are off. Press the C button on M5Stack to calibrate.

After the toio Core Cube is discovered, press the A button on M5Stack to connect to the toio Core Cube via BLE. Once connected, you can drive the toio Core Cube with the joystick.

While connected to BLE, pressing the z-axis of the joystick will play Charmelo. Also, when you press the B button on the M5Stack, the LED on the toio core cube lights up white.

[![joystick_drive demo video](https://img.youtube.com/vi/FLccNi00Pds/0.jpg)](https://www.youtube.com/watch?v=FLccNi00Pds)

---------------------------------------
## <a id="Release-Note">Release note</a>

* v1.0.0 (2020-07-19)
  * First edition release(futomi-san's original version)

* v1.0.0 (2023-10-09)
  * kenichi884 version　First github publication, toio core cube BLE protocol v2.3.0 support.

* v1.0.0 (2023-12-24)
  * kenichi884 version　Corrected slightly sample sketches、Readme、Added the v1.0.0 tag to distinguish it from subsequent modifications.

* v1.0.1 (2023-12-24)
  * kenichi884 version　Changed to use NimBLE, changed Serial.print() in the sample code to M5.Log.printf()

* v1.0.2 (2024-01-07)
  * kenichi884 version　Fixed an issue where get methods and events (notify) were not returning information on individual toio core cubes when multiple toio core cubes were connected.

* v1.0.3. (2024-02-07)
  * kenichi884 version　Add Reaadme.md (nglish translated). Change struct ToioCoreIDData and struct ToioCoreMotorResponse definition (use union).
---------------------------------------
## <a id="References">References</a>

* [toio Official Web page](https://toio.io/)
* [toio コア キューブ 技術仕様(Japanese)](https://toio.github.io/toio-spec/)
* [toio Core Cube Specifications(English)](https://toio.github.io/toio-spec/en/)
* [M5Stack](https://m5stack.com/)
  * [ESP32 Basic Core IoT Development Kit](https://m5stack.com/collections/m5-core/products/basic-core-iot-development-kit)
  * [ESP32 GREY Development Kit with 9Axis Sensor](https://m5stack.com/collections/m5-core/products/grey-development-core)
  * [I2C Joystick Unit (MEGA328P)](https://m5stack.com/products/joystick-unit)
* [Switch Science](https://www.switch-science.com/)
  * [M5Stack Basic](https://www.switch-science.com/catalog/3647/)
  * [M5Stack Gray](https://www.switch-science.com/catalog/3648/)
  * [M5Stack Joystick unit](https://www.switch-science.com/catalog/4050/)

---------------------------------------
## <a id="License">License</a>

The MIT License (MIT)

Copyright (c) 2020 Futomi Hatano

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
