#include <U8g2lib.h>
#include "SdFat.h"

U8G2_ST7920_128X64_F_SW_SPI lcd(U8G2_R0, 23, 17, 16, U8X8_PIN_NONE); // from Pinout-Mega2560rev3_latest.pdf & QC12864B.pdf
SdFs                        sd;
FsFile                      file;
FsFile                      root;

const int Button_Stop  = 41;
const int Button_Yes   = 35;
const int Button_UpDn0 = 33; // UpDn0/1 are quadrature encoded
const int Button_UpDn1 = 31;

const int Axis_X_Dir   = 55; // A1
const int Axis_X_Step  = 54; // A0
const int Axis_X_En    = 38;
const int Axis_Y_Dir   = 61; // A7
const int Axis_Y_Step  = 60; // A6
const int Axis_Y_En    = 56; // A2
const int Axis_Z_Dir   = 48;
const int Axis_Z_Step  = 46; // ??? Active low
const int Axis_Z_En    = 62; // A8
const int Axis_E1_Dir  = 34;
const int Axis_E1_Step = 36;
const int Axis_E1_En   = 30;
const int Axis_E0_Dir  = 28;
const int Axis_E0_Step = 26;
const int Axis_E0_En   = 24;

const int FET_10 = 10; // ??? active low
const int FET_9  =  9; // ??? active low
const int FET_8  =  8; // ??? active low

const int Stop_0  = 3;
const int Stop_1  = 2;
const int Stop_2  = 14;
const int Stop_3  = 15;
const int Stop_4  = 18;
const int Stop_5  = 19;

const int I2C_SDA = 20;
const int I2C_SCL = 21;

const int Servo_0 = 11;
const int Servo_1 = 6;
const int Servo_2 = 5;
const int Servo_3 = 4;

const int Therm_0 = A13;
const int Therm_1 = A14;
const int Therm_2 = A15;

const int Aux1A_3_TX = 1; // TX
const int Aux1A_4_RX = 0; // RX
const int Aux1S_3 = A3;
const int Aux1S_4 = A4;

const int Aux2_3  = A5;
const int Aux2_4  = A9;
const int Aux2_5  = A10;
const int Aux2_6  = 40;
const int Aux2_7  = 44;
const int Aux2_8  = 42;
const int Aux2_9  = A12;
const int Aux2_10 = A11;



void setup()
{
  pinMode(Button_Stop,  INPUT_PULLUP);
  pinMode(Button_Yes,   INPUT_PULLUP);
  pinMode(Button_UpDn0, INPUT_PULLUP);
  pinMode(Button_UpDn1, INPUT_PULLUP);

  pinMode(Axis_X_Dir,   OUTPUT);
  pinMode(Axis_X_Step,  OUTPUT);
  pinMode(Axis_X_En,    OUTPUT);
  pinMode(Axis_Y_Dir,   OUTPUT);
  pinMode(Axis_Y_Step,  OUTPUT);
  pinMode(Axis_Y_En,    OUTPUT);
  pinMode(Axis_Z_Dir,   OUTPUT);
  pinMode(Axis_Z_Step,  OUTPUT);
  pinMode(Axis_Z_En,    OUTPUT);
  pinMode(Axis_E1_Dir,  OUTPUT);
  pinMode(Axis_E1_Step, OUTPUT);
  pinMode(Axis_E1_En,   OUTPUT);
  pinMode(Axis_E0_Dir,  OUTPUT);
  pinMode(Axis_E0_Step, OUTPUT);
  pinMode(Axis_E0_En,   OUTPUT);
  
  pinMode(FET_10,       OUTPUT);
  pinMode(FET_9,        OUTPUT);
  pinMode(FET_8,        OUTPUT);
  
  pinMode(Stop_0,       INPUT_PULLUP);
  pinMode(Stop_1,       INPUT_PULLUP);
  pinMode(Stop_2,       INPUT_PULLUP);
  pinMode(Stop_3,       INPUT_PULLUP);
  pinMode(Stop_4,       INPUT_PULLUP);
  pinMode(Stop_5,       INPUT_PULLUP);
  
  pinMode(I2C_SDA,      INPUT_PULLUP);
  pinMode(I2C_SCL,      INPUT_PULLUP);
  
  pinMode(Servo_0,      INPUT_PULLUP);
  pinMode(Servo_1,      INPUT_PULLUP);
  pinMode(Servo_2,      INPUT_PULLUP);
  pinMode(Servo_3,      INPUT_PULLUP);
  
  pinMode(Therm_0,      INPUT);
  pinMode(Therm_1,      INPUT);
  pinMode(Therm_2,      INPUT);
  
  pinMode(Aux1S_3,      INPUT_PULLUP);
  pinMode(Aux1S_4,      INPUT_PULLUP);
  
  pinMode(Aux2_3,       INPUT_PULLUP);
  pinMode(Aux2_4,       INPUT_PULLUP);
  pinMode(Aux2_5,       INPUT_PULLUP);
  pinMode(Aux2_6,       INPUT_PULLUP);
  pinMode(Aux2_7,       INPUT_PULLUP);
  pinMode(Aux2_8,       INPUT_PULLUP);
  pinMode(Aux2_9,       INPUT_PULLUP);
  pinMode(Aux2_10,      INPUT_PULLUP);

  
  Serial.begin(250000);
  lcd.begin();
  lcd.setFont(u8g2_font_5x7_tf);     
  while( !sd.begin( SdSpiConfig(SS, DEDICATED_SPI) ) )
  {
    lcd.clearBuffer();
    lcd.setCursor(10, 32);
    lcd.print("Insert SD");     
    lcd.sendBuffer();
  }

  char name[128];
  if (root.open("/"))
  {
    lcd.clearBuffer();
    int row = 8; // Font origin is at the lower left corner
    while( file.openNext(&root, O_RDONLY) )
    {
      lcd.setCursor(0, row);
      file.getName( name, 128 );
      lcd.print(name);
      Serial.println( name );     
      file.close();
      row+=8;
      if( row >= 64 )
        break;
    }
    lcd.sendBuffer();
  }
  else
  {
    Serial.println("Failed to open root");
  }

  delay(5000);
}

void loop()
{
  lcd.clearBuffer();
  
  int i=0, j=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Button_Stop));  i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Button_Yes));   i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Button_UpDn0)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Button_UpDn1)); i+=8;
  lcd.setCursor(i,j); lcd.print("millis() = "); lcd.print(millis());

  j+=8;
  i=0;
  lcd.setCursor(i,j); lcd.print(digitalRead(Stop_0)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Stop_1)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Stop_2)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Stop_3)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Stop_4)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Stop_5)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(I2C_SDA)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(I2C_SCL)); i+=8;
  
  j+=8;
  i=0;
  lcd.setCursor(i,j); lcd.print(digitalRead(Servo_0)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Servo_1)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Servo_2)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Servo_3)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux1S_3)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux1S_4)); i+=8;
  
  j+=8;
  i=0;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux2_3)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux2_4)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux2_5)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux2_6)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux2_7)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux2_8)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux2_9)); i+=8;
  lcd.setCursor(i,j); lcd.print(digitalRead(Aux2_10)); i+=8;

  j+=8;
  i=0;
  lcd.setCursor(i,j); lcd.print(analogRead(Therm_0)); i+= 3*8+4;
  lcd.setCursor(i,j); lcd.print(analogRead(Therm_1)); i+= 3*8+4;
  lcd.setCursor(i,j); lcd.print(analogRead(Therm_2)); i+= 3*8+4;
  
  lcd.sendBuffer();
}
