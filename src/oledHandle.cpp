#include <Arduino.h>
#include <U8g2lib.h>
#include <timeHandle.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void oled_init()
{
  // u8g2.setI2CAddress(0x7A);
  u8g2.begin();
  u8g2.setFont(u8g2_font_moosenooks_tr);
  // u8g2.setFont(u8g2_font_luIS14_tr);
  u8g2.drawStr(0, 16, "karjoAgro");
  u8g2.sendBuffer();
  Serial.println("karjoAgro start");
}

void displayJam()
{
  // u8g2_font_profont22_mn
  u8g2.setFont(u8g2_font_moosenooks_tr);
  String tm = "";
  if (getJam() < 10)
  {
    tm += "0";
  }
  tm += String(getJam());
  tm += ":";
  if (getMenit() < 10)
  {
    tm += "0";
  }
  tm += getMenit();

  u8g2.drawStr(0, 32, tm.c_str());
}

uint8_t m = 24;

void oled_loop()
{
  static long dl = millis();
  char m_str[3];
  strcpy(m_str, u8x8_u8toa(m, 2));
  u8g2.firstPage();

  if ((millis() - dl) > 1000)
  {
    dl = millis();
    do
    {
      displayJam();
    } while (u8g2.nextPage());
  }
}