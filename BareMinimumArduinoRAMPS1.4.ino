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

const int Aux1A_3 = 2;
const int Aux1A_4 = 1;
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
  pinMode(Button_Stop, INPUT_PULLUP);
  pinMode(Button_Yes,  INPUT_PULLUP);
  pinMode(Button_UpDn0, INPUT_PULLUP);
  pinMode(Button_UpDn1, INPUT_PULLUP);
  
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
  
  updateTime();
  lcd.setCursor(8,16); lcd.print(digitalRead(Button_Stop));
  lcd.setCursor(8,24); lcd.print(digitalRead(Button_Yes));
  lcd.setCursor(8,32); lcd.print(digitalRead(Button_UpDn0));
  lcd.setCursor(8,40); lcd.print(digitalRead(Button_UpDn1));
  
  lcd.sendBuffer();
}

void updateTime()
{
  lcd.setCursor(0, 8);
  lcd.print("millis() = ");     
  lcd.print(millis());
}
