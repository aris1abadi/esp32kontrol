#ifndef TIMEHANDLE_H
#define TIMEHANDLE_H

void time_init();
void time_loop();
void time_set(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year);
void ntp_init();
void ntp_sync();

uint8_t getJam();
uint8_t getMenit();
uint8_t getDtk();

uint8_t getTahun();
uint8_t getBl();
uint8_t getTgl();

bool isTimeUpdate();
void setTimeUpdate(bool sts);

#endif