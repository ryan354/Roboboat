//Bisa lah
//Grim Reaper SAS

#include <Servo.h>
Servo brusless;
Servo Direct;

String dataMasuk = "";
boolean parsing = false;



int pwmA;
int pwm;
bool controll;

//Controller kanan
int btnCross, btnCircle, btnSquare, btnTriangle;
//Controller kiri
int btnPadUp, btnPadDown, btnPadLeft, btnPadRight;
//Controller Bawah
int btnR1, btnR2, btnL1, btnL2;
//Controller Analog;
int RY, RX, LY, LX;
int arm = 25;
int dir = 90;
void setup() {
  brusless.attach(9);
  brusless.write(arm);

  Direct.attach(10);
  Direct.write(arm);

  Serial.begin(9600);
  dataMasuk = "";
  pwm = 255;
  Serial.println("Grim Reaper SAS");
  Serial.println("Ready To Race!!");


}

void loop() {

  SerialEvent();
  preParsing();

}

//-========// Setup //========-

void SerialEvent() {
  if (Serial.available()) {
    char dtMasuk = Serial.read();
    Serial.print(dtMasuk);
    dataMasuk += dtMasuk;
    if (dtMasuk == '\n') {
      parsing = true;
    }
  }
}

void preParsing() {
  if (parsing) {
    controll = true;
    parsingData();
    Surfing();
    parsing = false;
    controll = false;
    dataMasuk = "";
  }
}

void parsingData() {
  int j = 0;
  String data[30];
  char dataIn[30][5];
  data[j] = "";
  for (int i = 0; i < dataMasuk.length(); i++) {
    if (dataMasuk[i] == ',') {
      j++;
      data[j] = "";
    }
    else {
      data[j] += dataMasuk[i];
      data[j].toCharArray(dataIn[j], 5);
    }
  }
  //-========// Penyimpanan ke variable //========-
  btnCross = x2i(dataIn[0]);
  btnCircle = x2i(dataIn[1]);
  btnSquare = x2i(dataIn[2]);
  btnTriangle = x2i(dataIn[3]);
  btnPadUp = x2i(dataIn[4]);
  btnPadDown = x2i(dataIn[5]);
  btnPadLeft = x2i(dataIn[6]);
  btnPadRight = x2i(dataIn[7]);
  btnR1 = x2i(dataIn[8]);
  btnR2 = x2i(dataIn[9]);
  btnL1 = x2i(dataIn[10]);
  btnL2 = x2i(dataIn[11]);
  RY = x2i(dataIn[12]);
  RX = x2i(dataIn[13]);
  LY = x2i(dataIn[14]);
  LX = x2i(dataIn[15]);

}

//-========// Konversi Hex ke INT //========-
int x2i(char *s)
{
  int x = 0;
  for (;;) {
    char c = *s;
    if (c >= '0' && c <= '9')


    {
      x *= 16;
      x += c - '0';
    }
    else if (c >= 'a' && c <= 'f') {
      x *= 16;
      x += (c - 'a') + 10;
    }
    else break;
    s++;
  }
  return x;
}


//-========// Controll PWM (L2/R2) //========-


//-========// Controll Set //========-
void Surfing() {

  if (controll) {
    analogMode();
    low();
    normal();
    full();
    kiri();
    kanan();

  }
  else {
    brusless.write(arm);
    Direct.write(dir);
  }
}

//-========// Analog Set //========-
/*void analogModePWM() {
  if (LY != 128 || LX != 128) {
    if (LY > 128 || LX > 128) {
      pwmA += 20;
      if (pwmA > 255) {
        pwmA = 255;
      }
    }
    else if (LY < 128 || LX < 128) {
      pwmA += 20;
      if (pwmA > 255) {
        pwmA = 255;
      }
    }
  }
  else if (RY != 128 || RX != 128) {
    if (RY > 128 || RX > 128) {
      pwmA += 20;
      if (pwmA > 255) {
        pwmA = 255;
      }
    }
    else if (RY < 128 || RX < 128) {
      pwmA += 20;
      if (pwmA > 255) {
        pwmA = 255;
      }
    }
  }
  else {
    pwmA -= 30;
    motorMati();
    if (pwmA < 150) {
      pwmA = 150;
    }
  }
  }*/

void analogMode() {
  //  analogModePWM();


  if (LX < 128) {
    Serial.print("Kiri!!  ");
    Direct.write(1);
  }
  else if (LX > 128) {
    Serial.print("Kanan!!  ");
    Direct.write(179);
  }

  else {
    Direct.write(dir);
    brusless.write(arm);
  }
}


//-========// Drive Set //========-


void kanan() {
  if (btnPadRight == 255) {
    Serial.println("Kanan!!");
    Direct.write(180);
  }
}
void kiri() {
  if (btnPadLeft == 255) {
    Serial.println("Kiri!!");
    Direct.write(1);
  }
}
void low() {
  if (btnSquare == 255) {
    Serial.println("LOW Speed");
    brusless.write(50);
  }

}



void normal() {
  if (btnCircle == 255) {
    Serial.println("Normal Speed");
    brusless.write(100);
  }
}

void full() {
  if (btnCross == 255) {
    Serial.println("Full Speed");
    brusless.write(170);
  }

}



