#ifndef NODEHANDLE_H
#define NODEHANDLE_H

struct akt{
    uint8_t nodeId ;
    uint8_t childId;
    uint8_t status = 0;
    uint8_t val = 0;    
    uint8_t timeOut = 0;
    uint8_t jadwalAktif = 0;
    uint8_t durasiCount = 0;
    uint8_t durasiManual = 5;
    uint8_t sensorUseId = 0 ;
    uint8_t sensorUseChildId = 0;
    uint8_t triggerMin = 0;
    uint8_t triggerMax = 100;
    uint8_t jadwal[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};;//5 alokasi jadwal ,format J1,M1,D1,J2,M2,D2,J3,M3,D3,J4,M4,D4,J5,M5,D5
    uint8_t lastUpdate[6] = {0,0,0,0,0,0};//format th,bl,tg,jam.mn.dt
};

struct sen
{
   uint8_t nodeId ;
    uint8_t childId1 = 10;//lengas
    uint8_t childId2 = 11;//LOW triger
    uint8_t childId3 = 12;//HIGH triger
    uint8_t interval = 10;
    uint8_t status = 0;   
    uint8_t battLevel = 0;
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    uint8_t val3 = 0;
    unsigned int kalMin;//2byte di eeprom
    unsigned int kalMax;//2byte di eeprom
    uint8_t lastUpdate[6]={0,0,0,0,0,0};//format th,bl,tg,jam.mn.dt
};

typedef struct akt Aktuator;
typedef struct sen Sensor;

void loadDefault();
#endif