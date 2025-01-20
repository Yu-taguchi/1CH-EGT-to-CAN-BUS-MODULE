#include <Adafruit_MAX31855.h>
#include <mcp2515.h>
#define MAXCS 10
Adafruit_MAX31855 thermocouple(MAXCS);
struct can_frame canMsg1;
struct can_frame canMsg2;
MCP2515 mcp2515(9);
unsigned int a, bH, bL;

void setup() {
  canMsg1.can_id = 0x5A0;  //送信IDを変更する場合は、ココの値を変更する。(例)LINK G4XのIDが1440(10進数)の場合、0x5A0(16進数)を記入。数字冒頭の0xは16進数の値である事を示すおまじない。
  canMsg1.can_dlc = 2;
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  delay(500);
} 

void loop() {
  double c = thermocouple.readCelsius();
  c = c * 10;
  int k = (int)c;
  bH = k >> 8;
  bL = k & 0x00FF;
  canMsg1.data[0] = bH;  //送信内容
  canMsg1.data[1] = bL;  //送信内容
  mcp2515.sendMessage(&canMsg1);
  delay(20);
}