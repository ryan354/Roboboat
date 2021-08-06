
#include <SoftwareSerial.h>
#include <PS2X_lib.h>  //for v1.6

PS2X ps2x; // create PS2 Controller Class
SoftwareSerial mySerial(6, 7); // RX, TX
//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

String dataRemote;
int tblAtas, tblBawah, tblKiri, tblKanan, tblX, tblKotak;
int remoteHeader = 0xCA;  //header data remote
int dataHeader = 0xCC; //header data sensor dari Robot
long timeRequest;
long waitingTime = 300; //waktu tunggu request data (milisecond)

String hdIn = "00";
String dataIn1 = "00";
String dataIn2 = "00";
String csIn = "";

char hdInBuff[3];
char dataIn1Buff[3];
char dataIn2Buff[3];
char csInBuff[3];

void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);
  //mySerial.println("test");
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  error = ps2x.config_gamepad(13, 11, 10, 12, true, true); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  /*if (error == 0) {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }*/
  inputString.reserve(200);
}

void loop() {
  bacaRemote(); //fungsi baca data remote
  sendValue();  //mengirim paket data remote ke robot
  timeRequest = millis(); //mencatat nilai waktu request ke robot
  bacaData();   //membaca data dari Robot
  olahData();
  //mySerial.println("xx");
  delay(50);
}

void olahData() {
  if (stringComplete) {
    inputString.trim();
    
    parsingData();
    /*Serial.println(hdIn);
    Serial.println(dataIn1);
    Serial.println(dataIn2);
    Serial.println(csIn);*/
    int dtXOR = x2i(hdInBuff)^x2i(dataIn1Buff)^x2i(dataIn2Buff);
    if (x2i(csInBuff) == dtXOR) {
      if(x2i(hdInBuff)==dataHeader){
        mySerial.println(checkLength(String(x2i(dataIn1Buff)),2)+","+checkLength(String(x2i(dataIn2Buff)),2));//kirim ke ESP
        //Serial.println("Data Terkirim = "+String(x2i(dataIn1Buff))+","+String(x2i(dataIn2Buff)));
      }
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void parsingData() {
  inputString.toUpperCase();
  hdIn = inputString.substring(0, 2);
  dataIn1 = inputString.substring(2, 4);
  dataIn2 = inputString.substring(4, 6);
  csIn = inputString.substring(6, 8);

  hdIn.toCharArray(hdInBuff, 3);
  dataIn1.toCharArray(dataIn1Buff, 3);
  dataIn2.toCharArray(dataIn2Buff, 3);
  csIn.toCharArray(csInBuff, 3);
}

void bacaData() {
  while ( stringComplete == false && (millis() - timeRequest) < waitingTime/1) {
    while (Serial.available()) {
      // get the new byte:
      char inChar = (char)Serial.read();
      // add it to the inputString:
      inputString += inChar;
      // if the incoming character is a newline, set a flag
      // so the main loop can do something about it:
      if (inChar == '\n') {
        stringComplete = true;
      }
    }
  }
}

void bacaRemote() {
  ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
  tblAtas = 0;
  tblBawah = 0;
  tblKanan = 0;
  tblKiri = 0;
  tblX = 0;
  tblKotak = 0;
  if (ps2x.Button(PSB_START)) {}
  if (ps2x.Button(PSB_SELECT)) {}


  if (ps2x.Button(PSB_PAD_UP)) {
    tblAtas = 1;
  }
  if (ps2x.Button(PSB_PAD_RIGHT)) {
    tblKanan = 1;
  }
  if (ps2x.Button(PSB_PAD_LEFT)) {
    tblKiri = 1;
  }
  if (ps2x.Button(PSB_PAD_DOWN)) {
    tblBawah = 1;
  }


  //vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on
  //how hard you press the blue (X) button

  /*if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
  {
    if (ps2x.Button(PSB_L3))
      Serial.println("L3 pressed");
    if (ps2x.Button(PSB_R3))
      Serial.println("R3 pressed");
    if (ps2x.Button(PSB_L2))
      Serial.println("L2 pressed");
    if (ps2x.Button(PSB_R2))
      Serial.println("R2 pressed");
    if (ps2x.Button(PSB_GREEN))
      Serial.println("Triangle pressed");

  }*/


  if (ps2x.Button(PSB_RED)) {}

  if (ps2x.Button(PSB_PINK)) {
    tblKotak = 1;
  }

  if (ps2x.Button(PSB_BLUE)) {
    tblX = 1;
  }


  /*if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
  {
    Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC);
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC);
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC);
  }*/
}

void sendValue() {
  String packet;
  packet = String(remoteHeader, HEX);
  packet = packet + checkLength(String(tblAtas, HEX), 2);
  packet = packet + checkLength(String(tblBawah, HEX), 2);
  packet = packet + checkLength(String(tblKiri, HEX), 2);
  packet = packet + checkLength(String(tblKanan, HEX), 2);
  packet = packet + checkLength(String(tblKotak, HEX), 2);
  packet = packet + checkLength(String(tblX, HEX), 2);
  packet = packet + checkLength(String(checksum(), HEX), 2);
  Serial.println(packet);
}
String checkLength(String nilai, int panjang) {
  String x = nilai;
  while (x.length() < panjang) {
    x = "0" + x;
  }
  return x;
}
int checksum() {
  int x = remoteHeader ^ tblAtas ^ tblBawah ^ tblKiri ^ tblKanan ^ tblKotak ^ tblX;
  return x;
}
int x2i(char *s)
{
  int n = 0;
  for (;;) {
    char c = *s;
    if (c >= '0' && c <= '9') {
      n *= 16;
      n += c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
      n *= 16;
      n += (c - 'A') + 10;
    }
    else break;
    s++;
  }
  return n;
}
