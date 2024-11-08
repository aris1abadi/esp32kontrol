#ifndef KONTROLHANDLE_H
#define KONTROLHANDLE_H

void kontrol_loop();
void kontrol_init();
void cekMsg(String topic, String msg);
String getTask(uint8_t index);

/*
struct task
{
    uint8_t enable = 0;
    uint8_t aktuatorUse1 = 0;
    uint8_t aktuatorUse2 = 0;
    uint8_t status = 0; // 1 on,0 off
    uint8_t durasiCount = 0;
    uint8_t jadwalAktif = 0; // 1-5
    uint8_t jadwal[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t sensorUse1 = 0;    // isi 0 jika tidak pake sensor
    uint8_t sensorUse2 = 80; // ambang triger
    uint8_t targetOn = 1;
    uint8_t targetOff = 1;
    char nama[16]={'A','u','t','o',' ','1',0,0,0,0,0,0,0,0,0,0};
    //untuk task mi
    uint8_t aktuatorMixA = 11;
    uint8_t aktuatorMixB = 12;
    uint8_t aktuatorMixC = 13;
    uint8_t aktuatorAduk = 14;
    uint8_t aktuatorMixOut = 15;

    uint8_t flowSensorA = 1;
    uint8_t flowSensorB = 2;
    uint8_t flowSensorC = 3;
    uint8_t flowMixOut = 4;

    uint8_t targetMixA = 0;    
    uint8_t targetMixAStatus = 0;

    uint8_t targetMixB = 0;    
    uint8_t targetMixBStatus = 0;

    uint8_t targetMixC = 0;    
    uint8_t targetMixCStatus = 0;

    uint8_t mixingCount = 0;    // durasi mixing
    uint8_t mixingTarget = 180; // durasi mixing (+- 3 menit)
};


*/
struct task
{
    uint8_t enable;
    uint8_t aktuatorUse1;
    uint8_t aktuatorUse2;
    uint8_t status; // 1 on,0 off
    uint8_t durasiCount;
    uint8_t jadwalAktif; // 1-5
    String jadwal;
    uint8_t sensorUse1;    // default sensor1    
    uint8_t sensorUse2; // isi 0 jika tidak pake sensor
    uint8_t sensorUse1Type;//1= temperature,2= humidity,3=lengas,4=intermitent,5=genangan,6=flowsensor
    uint8_t sensorUse2Type;
    uint8_t targetBawah;
    uint8_t targetAtas;
    uint8_t inverseMode;//0 (dibawah target bawah ON ,diata target atas OFF)untuk lengas,humidity
                        //1 (dibawh target bawah OFF,diatas target atas ON) unutk temperature
                        //2 mode mix
    String nama;//16byte
    float nowValue;
    float lastValue;
    //untuk task mi
    uint8_t aktuatorMixA;
    uint8_t aktuatorMixB;
    uint8_t aktuatorMixC;
    uint8_t aktuatorAduk;
    uint8_t aktuatorMixOut;

    uint8_t flowSensorA;
    uint8_t flowSensorB ;
    uint8_t flowSensorC;
    uint8_t flowMixOut;

    float flowAValue;
    float flowALastValue;

    float flowBValue;
    float flowBLastValue;

    float flowCValue;
    float flowCLastValue;

    float flowMixOutValue;
    float flowMixOutLastValue;

    uint8_t targetMixA;    
    uint8_t targetMixAStatus;

    uint8_t targetMixB;    
    uint8_t targetMixBStatus;

    uint8_t targetMixC;    
    uint8_t targetMixCStatus;


    uint8_t mixingCount ;    // durasi mixing
    uint8_t mixingTarget; // durasi mixing (+- 3 menit)

    int targetMixOut;

    String mixAnama;
    String mixBnama;
    String mixCnama;
};

typedef struct task myTask;
#endif