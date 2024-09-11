// #include <SPI.h>
#include <RTClib.h>
#include <timeHandle.h>

#define USE_NTP

// RTC_DS1307 rtc;
RTC_DS3231 rtc;

// #define USE_NTP

uint8_t lastMenit = 0;
uint8_t lastJam = 0;
uint8_t jamNow = 0;
uint8_t menitNow = 0;
uint8_t th,bl,tgl,dtk;
uint8_t hariNow = 0;
unsigned long time_delay = 0;
uint8_t updateSync_count = 0;
// bool siramStatus = false;

String timeNow = "";
DateTime now;

bool timeIsUpdate = false;

// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000);

char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabo", "Kamis", "Jumat", "Sabtu"};

void time_init()
{

  Serial.println("time init");

  // Wire.begin(SDA,SCL);

  Wire.begin();
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // rtcTimer.attach(20, time_update);
}

void time_set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
  // January 21, 2014 at 3am you would call:

  rtc.adjust(DateTime(year, month, dayOfMonth, hour, minute, second));
}

uint8_t getJam(){
  return jamNow;
}

uint8_t getMenit(){
  return menitNow;
}

uint8_t getTahun(){
  return th;
}

uint8_t getTgl(){
  return tgl;

}
uint8_t getBl(){
  return bl;
}

uint8_t getDtk(){
  return dtk;
}

bool isTimeUpdate(){
  return timeIsUpdate;
}

void setTimeUpdate(bool sts){
  timeIsUpdate = sts;
}

void time_update()
{

  now = rtc.now();
  dtk = now.second();

  if (lastMenit != now.minute())
  {
    timeIsUpdate = true;
    timeNow = String(now.year());
    timeNow += "-";
    if (now.month() < 10)
      timeNow += "0";
    timeNow += now.month();
    timeNow += "-";
    if (now.day() < 10)
      timeNow += "0";
    timeNow += now.day();
    timeNow += " ";
    if (now.hour() < 10)
      timeNow += "0";
    timeNow += now.hour();
    timeNow += ":";
    if (now.minute() < 10)
      timeNow += "0";
    timeNow += now.minute();
    timeNow += ":";
    if (now.second() < 10)
      timeNow += "0";
    timeNow += now.second();

    Serial.println(timeNow);

    lastMenit = now.minute(); // cek setiap menit saja
    jamNow = now.hour();
    hariNow = now.dayOfTheWeek();
    th = (now.year() - 2000);
    bl = now.month();
    tgl = now.day();

    menitNow = lastMenit;
  }
}

void time_loop()
{
  if ((millis() - time_delay) > 1000)
  {
    time_delay = millis();
    time_update();
  }
}

String getTimeNow()
{
  return timeNow;
}