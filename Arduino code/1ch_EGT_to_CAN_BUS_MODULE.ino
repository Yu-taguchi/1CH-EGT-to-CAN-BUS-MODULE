/****************************************
2025/02/02 Rev 1.0
基準接点温度計測、および故障診断機能を追加
****************************************/
#include "Adafruit_MAX31855.h"
#include "mcp2515.h"
#define MAXCS 10
Adafruit_MAX31855 thermocouple(MAXCS);
struct can_frame canMsg1;
struct can_frame canMsg2;
MCP2515 mcp2515(9);
unsigned int a, bH, bL;

void setup() {
  canMsg1.can_id = 0x5A0;  //送信IDを変更する場合は、ココの値を変更する。(例)LINK G4XのIDが1440(10進数)の場合、0x5A0(16進数)を記入。数字冒頭の0xは16進数の値である事を示すおまじない。
  canMsg1.can_dlc = 5;
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  delay(500);
}

void loop() {
  //測温接点温度(T1)の取得
  double c = thermocouple.readCelsius();
  //基準接点温度(T0)の取得
  double d = thermocouple.readInternal();
  //故障診断
  if (isnan(c)) {
    canMsg1.data[4] = 0x03; //FAIL
  } else {
    canMsg1.data[4] = 0x01; //Active
  }
  //取得温度(T0)のデータセット
  c = c * 10;
  int k = (int)c;
  bH = k >> 8;
  bL = k & 0x00FF;
  canMsg1.data[0] = bH;  //送信内容
  canMsg1.data[1] = bL;  //送信内容
  //取得温度(T1)のデータセット
  d = d * 10;
  k = (int)d;
  bH = k >> 8;
  bL = k & 0x00FF;
  canMsg1.data[2] = bH;  //送信内容
  canMsg1.data[3] = bL;  //送信内容

  //データ送信
  mcp2515.sendMessage(&canMsg1);
  delay(20);
}
