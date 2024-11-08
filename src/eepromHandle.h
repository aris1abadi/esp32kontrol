#ifndef EEPROMHANDLE_H
#define EEPROMHANDLE_H


void loadTask();

void simpanEnable(uint8_t idx, uint8_t value);
void simpanAktuatorUse1(uint8_t idx, uint8_t nomerAktuator);
void simpanAktuatorUse2(uint8_t idx, uint8_t nomerAktuator);
void simpanSensorUse1(uint8_t idx, uint8_t nomerSensor);
void simpanSensorUse2(uint8_t idx, uint8_t nomerSensor);
void simpanSensorUse1Type(uint8_t idx, uint8_t nomerSensor);
void simpanSensorUse2Type(uint8_t idx, uint8_t nomerSensor);
void simpanTargetBawah(uint8_t idx, uint8_t value);
void simpanTargetAtas(uint8_t idx, uint8_t value);
void simpanInverseMode(uint8_t idx, uint8_t value);
void simpanAktuatorMixA(uint8_t idx, uint8_t value);
void simpanAktuatorMixB(uint8_t idx, uint8_t value);
void simpanAktuatorMixC(uint8_t idx, uint8_t value);
void simpanAktuatorAduk(uint8_t idx, uint8_t value);
void simpanAktuatorMixOut(uint8_t idx, uint8_t value);
void simpanFlowSensorA(uint8_t idx, uint8_t value);
void simpanFlowSensorB(uint8_t idx, uint8_t value);
void simpanFlowSensorC(uint8_t idx, uint8_t value);
void simpanFlowMixOut(uint8_t idx, uint8_t value);
void simpanTargetMixA(uint8_t idx, uint8_t value);
void simpanTargetMixB(uint8_t idx, uint8_t value);
void simpanTargetMixC(uint8_t idx, uint8_t value);
void simpanTargetMixOut(uint8_t idx, uint8_t value);
void simpanDefaultEEPROM();


#endif