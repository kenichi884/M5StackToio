/* ----------------------------------------------------------------
  ToioCore.h

  Copyright (c) 2020 Futomi Hatano. All right reserved.
  Original https://github.com/futomi
  Toio ID read support   https://github.com/mhama
  Protocol v2.3.0 or later and NimBLE support  https://github.com/kenichi884 

  Licensed under the MIT license.
  See LICENSE file in the project root for full license information.
  -------------------------------------------------------------- */
#ifndef ToioCore_h
#define ToioCore_h

#include <Arduino.h>
#include <string>
#include <functional>
#define USE_NIMBLE 1
#ifdef USE_NIMBLE
#include <NimBLEDevice.h>
#else
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#endif

// モーション検出情報
struct ToioCoreMotionData {
  bool flat;
  bool clash;
  bool dtap;
  uint8_t attitude;
  uint8_t shake;
};

// モーション検出の姿勢(attitude)
enum ToioCoreMotionPosture {
  TopFacesUpward = 1,       //　天面が上
  BottomFacesUpward = 2,    //  底面が上
  RearSideFacesUpward = 3,  //  背面が上
  FrontSideFacesUpward = 4, //  正面(toioロゴ)が上
  RightSideFacesUpward = 5, //  右面が上
  LeftSideFacesUpward = 6   //  左面が上
};

// 姿勢角情報
struct ToioCorePostureAngleEuler {
  int16_t roll;
  int16_t pitch;
  int16_t yaw;
};
struct ToioCorePostureAngleQuaternion {
  float_t w;
  float_t x;
  float_t y;
  float_t z;
};
struct ToioCorePostureAngleHighPrecisionEuler {
  float_t roll;
  float_t pitch;
  float_t yaw;
};

struct ToioCorePostureAngle{
  union {
    ToioCorePostureAngleEuler euler;
    ToioCorePostureAngleQuaternion quaternion;
    ToioCorePostureAngleHighPrecisionEuler eulerf;
  };
};

//磁気センサー情報
struct ToioCoreMagneticSensorData {
  uint8_t state;
  uint8_t strength;
  int8_t x;
  int8_t y;
  int8_t z;
};

// ID Readerで読み取ったIDのタイプ
enum ToioCoreIDType {
  ToioCoreIDTypeNone = 0,     // 読み取れなかった、または初期値
  ToioCoreIDTypePosition = 1, // Position ID
  ToioCoreIDTypeStandard = 2  // Standard ID
};

// Position IDの場合のデータ
struct ToioCorePositionIDData {
  uint16_t cubePosX;
  uint16_t cubePosY;
  uint16_t cubeAngleDegree;
  uint16_t sensorPosX;
  uint16_t sensorPosY;
  uint16_t sensorAngleDegree;
};

// Standard IDの場合のデータ
struct ToioCoreStandardIDData {
  uint32_t standardID;
  uint16_t cubeAngleDegree;
};

// ID Readerで読み取った結果のデータ構造
// Position IDの場合とStandard IDの場合があり、それぞれのパラメータにデータが入る
struct ToioCoreIDData {
  ToioCoreIDData() : type(ToioCoreIDTypeNone) {}
  ToioCoreIDData(
    ToioCoreIDType type,
    ToioCorePositionIDData position)
    : type(type),
    position(position) {}
  ToioCoreIDData(
    ToioCoreIDType type,
    ToioCoreStandardIDData standard)
    : type(type),
    standard(standard) {}
  ToioCoreIDType type;
  union {
    ToioCorePositionIDData position; // Position IDの場合のデータ
    ToioCoreStandardIDData standard; // Standard IDの場合のデータ
  };
};

// 目標指定付き移動のターゲット座標
struct ToioCoreTargetPos {
  uint16_t posX;
  uint16_t posY;
  uint16_t angleDegree:13;
  uint8_t angleAndRotation:3;
};

// 目標指定つき移動の移動タイプ
enum ToioCoreMovementType {
  MoveWhileRotating = 0,
  MoveWhileRotatingWithoutMovingBackwards = 1,
  RotateAfterMoving = 2
};

// 目標指定つき移動の速度変化タイプ
enum ToioCoreSpeedChangeType {
  SpeedConstant = 0,
  GradualAccelerationTowardsTheTargtPoint = 1,
  GradualDecelerationTowardsTheTargtPoint = 2,
  GradualAccelerationHalfwayThenDecelationToTheTargtPoint = 3
};

// 目標指定つき移動の角度と回転方向タイプ
enum ToioCoreAngleAndRotationType {
  AbsoluteDirectionOfLeastAmountOfRotation = 0,
  AbsoluteForward = 1,
  AbsoluteNegative = 2,
  RelativeForward = 3,
  RelativeNegative = 4,
  NoAngleNoDirection = 5,
  SameAsWithWriteOperationDirectionOfLeastAmountOfRotation = 6
};

// モーター制御の応答
struct ToioCoreMotorResponse {
  uint8_t controlType;
  union {
    uint8_t controlID; // Control identification value
    uint8_t leftSpeed; // or Left motor speed
  };
  union {
   uint8_t response; // Response content
   uint8_t rightSpeed;  // or Right motor speed
  };
};

enum ToioCoreMotorResponseControlType {
  WithTarget = 0x83,
  WithMultipleTargets = 0x84,
  MotorSpeed = 0xe0
};

enum ToioCoreMotorResponseContent {
  CompletedSuccessfully = 0,
  ResponseTimeout = 1,
  ToioIDmissed = 2,
  InvalidCombinationOfParameters = 3,
  InvalidState = 4,
  OtherWrittenControlAccepted = 5,
  NotSupported = 6,
  WriteOperationCannotBeAdded = 7
};

enum ToioCoreNotificationCondition {
  NotifyAllways = 0x00,
  NotifyChangesOnly = 0x01,
  StopAfter300msNoChange = 0xff
};

enum ToioCoreMagneticSensorFunctionSetting {
  DisableMagnetSensor = 0,
  EnableMagnetState = 1,
  EnableMagneticForce = 2
};

enum ToioCorePostureAngleType {
  AngleTypeEuller = 0x01,
  AngleTypeQuaternion = 0x02,
  AngleTypeHighPrecisionEuller = 0x03
};

enum ToioCoreConfigrationType {
  ResponseBLEProtocolVersion = 0x81,
  ResponseIDnotificationSettings = 0x98,
  ResponseIDmissedNotificationSettings = 0x99,
  ResponseMagneticSensorSettings = 0x9b,
  ResponseMotoroSpeedInformationAcquisitionSettings = 0x9c,
  ResponsePostureAngleDetectionSettings = 0x9d,
  ResponseSerializedInformationSettings = 0x9e,
  ResponseChangeConnectionInterval = 0xb0,
  ResponseObtainRequestedConnectionInterval = 0xb1,
  ResponseObtainActualConnectionInterval = 0xb2,
  SerializedData = 0xf0
};

// 設定変更の応答 infoType =  0x98 0x99 0x9b 0x9c 0x9d 0x9e 0xb0
struct ToioCoreSetConfigurationResponse {
    uint8_t reserved;
    uint8_t response;
};

// コネクションインターバル値 infoType =  0xb1 or 0xb2
struct ToioCoreConnectionIntervalSettings {
    uint8_t reserved;
    uint16_t minimum;
    uint16_t maximum;
};

// 設定characteristicsの読み出し/notifyの応答
struct ToioCoreConfigurationResponse {
  uint8_t infoType;
  union {
    ToioCoreSetConfigurationResponse config; // 設定応答 infoType = 0x98 0x99 0x9b 0x9c 0x9d 0x9e 0xb0
    ToioCoreConnectionIntervalSettings interval; // コネクションインターバル値 infoType =  0xb1 or 0xb2
    uint8_t serialized[19]; // シリアライズ情報   info Type = 0xf0
  }; 
};


// ms time divisor  ミリ秒から引数値に変換するための除数
const int MOTOR_CTRL_DURATION_UNIT = 10;
const int NOTIFICATION_INTERVAL_UNIT = 10;
const int MAGNET_NOTIFICATION_INTERVAL_UNIT = 20;
const float CONNECTION_INTERVAL_UNIT = 1.25;

typedef std::function<void(bool connected)> OnConnectionCallback;
typedef std::function<void(bool state)> OnButtonCallback;
typedef std::function<void(uint8_t level)> OnBatteryCallback;
typedef std::function<void(ToioCoreMotionData motion)> OnMotionCallback;
typedef std::function<void(ToioCorePostureAngle angle)> OnPostureAngleCallback;
typedef std::function<void(ToioCoreMagneticSensorData magnetic_sensor)> OnMagneticSensorCallback;
typedef std::function<void(ToioCoreIDData id_data)> OnIDDataCallback;
typedef std::function<void(ToioCoreMotorResponse motor_response)> OnMotorCallback;
typedef std::function<void(ToioCoreConfigurationResponse configration_response)> OnConfigurationCallback;

// ---------------------------------------------------------------
// ToioCore クラス
// ---------------------------------------------------------------
class ToioCore {
  private:
    const char* _TOIO_SERVICE_UUID     = "10b20100-5b3b-4571-9508-cf3efcd7bbae";
    const char* _TOIO_CHAR_UUID_BATT   = "10b20108-5b3b-4571-9508-cf3efcd7bbae";
    const char* _TOIO_CHAR_UUID_LIGHT  = "10b20103-5b3b-4571-9508-cf3efcd7bbae";
    const char* _TOIO_CHAR_UUID_SOUND  = "10b20104-5b3b-4571-9508-cf3efcd7bbae";
    const char* _TOIO_CHAR_UUID_BUTTON = "10b20107-5b3b-4571-9508-cf3efcd7bbae";
    const char* _TOIO_CHAR_UUID_MOTION = "10b20106-5b3b-4571-9508-cf3efcd7bbae";
    const char* _TOIO_CHAR_UUID_CONF   = "10b201ff-5b3b-4571-9508-cf3efcd7bbae";
    const char* _TOIO_CHAR_UUID_MOTOR  = "10b20102-5b3b-4571-9508-cf3efcd7bbae";
    const char* _TOIO_CHAR_UUID_ID_READER  = "10b20101-5b3b-4571-9508-cf3efcd7bbae";

    BLEAdvertisedDevice* _device;
    BLEClient* _client;

    BLERemoteCharacteristic* _char_battery;
    BLERemoteCharacteristic* _char_light;
    BLERemoteCharacteristic* _char_sound;
    BLERemoteCharacteristic* _char_button;
    BLERemoteCharacteristic* _char_motion;
    BLERemoteCharacteristic* _char_conf;
    BLERemoteCharacteristic* _char_motor;
    BLERemoteCharacteristic* _char_id_reader;

    OnConnectionCallback _onconnection;
    OnButtonCallback _onbutton;
    OnBatteryCallback _onbattery;
    OnMotionCallback _onmotion;
    OnPostureAngleCallback _onpostureangle;
    OnMagneticSensorCallback _onmagneticsensor;
    OnIDDataCallback _on_id_reader;
    OnMotorCallback _onmotor;
    OnConfigurationCallback _onconfig;

  public:  ///  ToioClientCallbackからアクセスするためにpublic指定
    void setConnectionFlags(BLEClient *client);
  private:
    bool _event_connection_updated;
    BLEClient* _current_client;
    BLEClient* _last_client;

    bool _event_battery_updated;
    uint8_t _event_battery_level;
    bool _event_button_updated;
    bool _event_button_state;
    bool _event_motion_updated;
    ToioCoreMotionData _event_motion_data;
    bool _event_posture_angle_updated;
    ToioCorePostureAngle _event_posture_angle_data;  
    bool _event_magnetic_sensor_updated;
    ToioCoreMagneticSensorData _event_magnetic_sensor_data;
    bool _event_id_data_updated;
    ToioCoreIDData _event_id_data;
    bool _event_motor_updated;
    ToioCoreMotorResponse _event_motor_response;
    bool _event_config_updated;
    ToioCoreConfigurationResponse _event_config_response;

  private:
    void _wait(const unsigned long msec);

    // BLEのバイト配列からToioCoreIDDataに変換する。異常値の場合はfalseを返す
    static bool _convertBLEBytesToIDData(const uint8_t *bytes, int length, ToioCoreIDData & id_data);
    // BLEのバイト配列からToioCorePositionIDDataに変換する。
    static void _convertBLEBytesToPositionIDData(const uint8_t *bytes, ToioCorePositionIDData & pos_data);
    // BLEのバイト配列からToioCoreStandardIDDataに変換する。
    static void _convertBLEBytesToStandardIDData(const uint8_t *bytes, ToioCoreStandardIDData & std_data);

  public:
    // コンストラクタ
  
    ToioCore(BLEAdvertisedDevice& device);

    // デストラクタ
    virtual ~ToioCore();

    // アドレスを取得
    std::string getAddress();

    // デバイス名を取得
    std::string getName();

    // 接続
    bool connect();

    // 切断
    void disconnect();

    // 接続状態を返す
    bool isConnected();

    // 接続状態イベントのコールバックをセット
    void onConnection(OnConnectionCallback cb);

    // サウンド再生開始 (生データ指定)
    void playSoundRaw(uint8_t* data, size_t length);

    // サウンド再生開始 (効果音)
    void playSoundEffect(uint8_t sound_id, uint8_t volume = 0xff);

    // サウンド再生停止
    void stopSound();

    // LED 点灯
    void turnOnLed(uint8_t r, uint8_t g, uint8_t b);

    // LED 消灯
    void turnOffLed();

    // バッテリーレベルを取得
    uint8_t getBatteryLevel();

    // バッテリーイベントのコールバックをセット
    void onBattery(OnBatteryCallback cb);

    // ボタンの状態を取得
    bool getButtonState();

    // ボタンイベントのコールバックをセット
    void onButton(OnButtonCallback cb);

    // モーションセンサーの状態を取得
    ToioCoreMotionData getMotion();

    // モーションセンサーのコールバックをセット
    void onMotion(OnMotionCallback cb, OnMagneticSensorCallback mag_cb = nullptr, OnPostureAngleCallback angle_cb = nullptr);

    // BLE プロトコルバージョン取得
    std::string getBleProtocolVersion();

    // 水平検出のしきい値設定
    void setFlatThreshold(uint8_t deg = 45);

    // 衝突検出のしきい値設定
    void setClashThreshold(uint8_t level = 7);

    // ダブルタップ検出の時間間隔の設定
    void setDtapThreshold(uint8_t level = 5);

    // 読み取りセンサーの ID 通知頻度設定
    void setIDnotificationSettings(uint8_t minimum_interval, uint8_t condition);

    // 読み取りセンサーの ID missed 通知感度設定
    void setIDmissedNotificationSettings(uint8_t sensitivity);

    // 磁気センサーの設定
    void setMagneticSensorSettings(uint8_t interval, uint8_t condition, uint8_t function = EnableMagneticForce);

    // モーターの速度情報の取得の設定
    void setMotorSpeedInformationAcquistionSettings(bool enable);
    
    // 姿勢角検出の設定
    void setPostureAngleDetectionSettings(uint8_t interval, uint8_t condition, uint8_t angle_type = AngleTypeEuller);

    // シリアライズ情報の通知設定
    void setSerializedInformationSettings(uint8_t interval, uint8_t condition);

    // 設定の応答コールバックをセット
    void onConfiguration(OnConfigurationCallback cb);

    // コネクションインターバル値の変更
    void setConnectionInterval(uint16_t minimum, uint16_t maximum);

    // コネクションインターバル要求値の取得
    void getRequestedConnectionInterval(uint16_t& minimum, uint16_t& maximum);

    // 現在のコネクションインターバル値の取得
    void getAcctualConnectionInterval(uint16_t& minimum, uint16_t& maximum);

    // 設定変更の応答を取得
    ToioCoreConfigurationResponse getConfigurationResponse();

    // モーター制御 (引数の値をそのまま送信するローレベルのメソッド)
    void controlMotor(bool ldir, uint8_t lspeed, bool rdir, uint8_t rspeed, uint16_t duration = 0);

    // 運転 (モーター制御をスロットルとステアリング操作に置き換える)
    void drive(int8_t throttle, int8_t steering);

    // 目標指定付きモーター制御 (目標１つ)
    void controlMotorWithTarget(uint8_t distinction, uint8_t timeout, uint8_t movement_type, 
      uint8_t maximum_speed, uint8_t speed_change_type,
      uint16_t target_x, uint16_t target_y, 
      uint16_t target_angle_degree, uint8_t target_angle_and_rotation_bits = 0);

    // 目標指定付きモーター制御 (目標複数)
    void controlMotorWithMultipleTargets(uint8_t distinction, uint8_t timeout, uint8_t movement_type, 
      uint8_t maximum_speed, uint8_t speed_change_type,  uint8_t addition_setting,
      uint8_t target_num, ToioCoreTargetPos *target_positions);

    // 加速度指定モーター制御
    void controlMotorWithAcceleration(uint8_t translational_speed, uint8_t acceleration,
      uint16_t rotational_velocity, uint8_t rotational_direction = 0, uint8_t travel_direction = 0,
      uint8_t priority = 0, uint8_t duration = 0);

    // モーター制御の応答を取得
    ToioCoreMotorResponse getMotor();

    // モーター制御の応答コールバックをセット
    void onMotor(OnMotorCallback cb);

    // ID Reader の読み取り結果を取得
    ToioCoreIDData getIDReaderData();

    // ID Readerのコールバックをセット
    void onIDReaderData(OnIDDataCallback cb);

    // Toio.cpp から呼ばれる (.ino からは直接呼ばない)
    void _loop();
};

#endif
