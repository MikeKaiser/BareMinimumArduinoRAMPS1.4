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
