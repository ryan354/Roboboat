#include <PS2X_lib.h>  //for v1.6

/******************************************************************
   set pins connected to PS2 controller:
     - 1e column: original
     - 2e colmun: Stef?
   replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        A2  //14    
#define PS2_CMD        A3  //15
#define PS2_SEL        A4  //16
#define PS2_CLK        A5  //17

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   true
//#define rumble      true
#define rumble      true

PS2X ps2x; // create PS2 Controller Class

int error = 0;
String strData = "";

//Controller kanan
int btnCross, btnCircle, btnSquare, btnTriangle;
//Controller kiri
int btnPadUp, btnPadDown, btnPadLeft, btnPadRight;
//Controller Bawah
int btnR1, btnR2, btnL1, btnL2;
//Controller Analog;
int RY, RX, LY, LX;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  if (error == 0) {
    Serial.println("Ground is Ready!!");
  }
  if (error == 1) {
    Serial.println("Error");
  }
}

void loop() {
  controll();
  delay(100);
}

void controll() {
  ps2x.read_gamepad();
  //-==============// Mulai Identifikasi //-==============-//
  //Cross
  if (ps2x.Button(PSB_CROSS)) {
    Serial.println("X");
  }
  else {
    Serial.println("-X");
  }
  //Circle
  if (ps2x.Button(PSB_CIRCLE)) {
    Serial.println("O");
  }
  else {
    Serial.println("-O");
  }
  //Triangle
  if (ps2x.Button(PSB_TRIANGLE)) {
    Serial.println("D");
  }
  else {
    Serial.println("-D");
  }
  //Square
  if (ps2x.Button(PSB_SQUARE)) {
    Serial.println("H");
  }
  else {
    Serial.println("-H");
  }
  //PadUp
  //Analog
  RY = ps2x.Analog(PSS_RY);
  LY = ps2x.Analog(PSS_LY);
  RX = ps2x.Analog(PSS_RX);
  LX = ps2x.Analog(PSS_LX);
}
