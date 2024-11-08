#include <Arduino.h>
#include <EEPROM.h>
#include <nodeHandle.h>
#include <eepromHandle.h>
#include <kontrolHandle.h>
/*
alokasi aktuator pada eeprom 22 byte per node
1-10 aktuator(tiap node punya 2 relay )

51 - 100 sensor
*/

#define DEBUG_EEPROM

#define EE_READY_CODE 1000
#define EE_TASK_LEN 1001
#define EE_TASK_START 1002

#define MEM_TASK_USE 102 //  total 54byte terpakai

#define MEM_AKTUATOR_USE1 1
#define MEM_AKTUATOR_USE2 2
#define MEM_SENSOR_USE1 18
#define MEM_SENSOR_USE2 19
#define MEM_SENSOR_USE1_TYPE 20
#define MEM_SENSOR_USE2_TYPE 21
#define MEM_TARGET_BAWAH 22
#define MEM_TARGET_ATAS 23
#define MEM_INVERSE_MODE 24

#define MEM_AKTUATOR_MIXA 41
#define MEM_AKTUATOR_MIXB 42
#define MEM_AKTUATOR_MIXC 43
#define MEM_AKTUATOR_MIXADUK 44
#define MEM_AKTUATOR_MIXOUT 45
#define MEM_FLOW_SENSORA 46
#define MEM_FLOW_SENSORB 47
#define MEM_FLOW_SENSORC 48
#define MEM_FLOW_MIXOUT 49
#define MEM_TARGET_MIXA 50
#define MEM_TARGET_MIXB 51
#define MEM_TARGET_MIXC 52
#define MEM_TARGET_MIXOUT 53
#define MEM_MIXA_NAMA 54
#define MEM_MIXB_NAMA 71
#define MEM_MIXC_NAMA 87


/*
format penyimpanan task di EEPROM 54byte total

    uint8_t enable;             1byte
    uint8_t aktuatorUse1;       1byte
    uint8_t aktuatorUse2;       1byte
    uint8_t status;
    uint8_t durasiCount;
    uint8_t jadwalAktif;
    String jadwal;              15byte
    uint8_t sensorUse1;         1byte
    uint8_t sensorUse2;         1byte
    uint8_t sensorUse1Type;     1byte
    uint8_t sensorUse2Type;     1byte
    uint8_t targetBawah;        1byte
    uint8_t targetAtas;         1byte
    uint8_t inverseMode;        1byte
    String nama;                16byte
    float nowValue;
    float lastValue;
    //untuk task mi
    uint8_t aktuatorMixA;       1byte
    uint8_t aktuatorMixB;       1byte
    uint8_t aktuatorMixC;       1byte
    uint8_t aktuatorAduk;       1byte
    uint8_t aktuatorMixOut;     1byte

    uint8_t flowSensorA;        1byte
    uint8_t flowSensorB ;       1byte
    uint8_t flowSensorC;        1byte
    uint8_t flowMixOut;         1byte

    uint8_t targetMixA;         1byte
    uint8_t targetMixAStatus;

    uint8_t targetMixB;         1byte
    uint8_t targetMixBStatus;

    uint8_t targetMixC;         1byte
    uint8_t targetMixCStatus;

    uint8_t mixingCount ;
    uint8_t mixingTarget;       1byte


*/
extern myTask dataTask[];
extern myTask defaultTask[];
extern int dataTaskLen;

uint8_t ee_data[120];

uint8_t i = 0;
int kal = 0;
String nm = "";

void loadTask()
{

    uint8_t ee_test = EEPROM.read(EE_READY_CODE);
    if (ee_test == 0xAA)
    {
        Serial.println("load data Task dari eeprom");
        // load jml task
        uint8_t jmlTask = EEPROM.read(EE_TASK_LEN);
        Serial.print("Jumlah task: ");
        Serial.println(jmlTask);
        for (int j = 0; j < jmlTask; j++)
        {
            // load eeprom
            for (int i = 0; i < MEM_TASK_USE; i++)
            {
                ee_data[i] = EEPROM.read(EE_TASK_START + (j * MEM_TASK_USE) + i);
            }
            dataTask[j].enable = ee_data[0];
            dataTask[j].aktuatorUse1 = ee_data[1];
            dataTask[j].aktuatorUse2 = ee_data[2];
            //dataTask[j].jadwal = "";
            String jw ="";
            for (int a = 0; a < 15; a++)
            {
                if(a > 0){
                    jw += ',';
                }
                jw += char(ee_data[3 + a]);

            }
            dataTask[j].jadwal = String(jw);

            dataTask[j].sensorUse1 = ee_data[18];
            dataTask[j].sensorUse2 = ee_data[19];
            dataTask[j].sensorUse1Type = ee_data[20];
            dataTask[j].sensorUse2Type = ee_data[21];
            dataTask[j].targetBawah = ee_data[22];
            dataTask[j].targetAtas = ee_data[23];
            dataTask[j].inverseMode = ee_data[24];
            
            String nm = "";
            char nmTes ;
            for (int b = 0; b < 16; b++)
            {
                
                nmTes = char(ee_data[25 + b]);
                if(nmTes != NULL){
                    nm += nmTes;
                }
            }

            nm.replace(" ","");
            dataTask[j].nama = nm;
            //dataTask[j].nama.trim();
            dataTask[j].aktuatorMixA = ee_data[41];
            dataTask[j].aktuatorMixB = ee_data[42];
            dataTask[j].aktuatorMixC = ee_data[43];
            dataTask[j].aktuatorAduk = ee_data[44];
            dataTask[j].aktuatorMixOut = ee_data[45];
            dataTask[j].flowSensorA = ee_data[46];
            dataTask[j].flowSensorB = ee_data[47];
            dataTask[j].flowSensorC = ee_data[48];
            dataTask[j].flowMixOut = ee_data[49];
            dataTask[j].targetMixA = ee_data[50];
            dataTask[j].targetMixB = ee_data[51];
            dataTask[j].targetMixC = ee_data[52];
            dataTask[j].mixingTarget = ee_data[53];
            //nama mix A
            nm = "";            
            for (int b = 0; b < 16; b++)
            {
                
                nmTes = char(ee_data[54 + b]);
                if(nmTes != NULL){
                    nm += nmTes;
                }
            }

            nm.replace(" ","");
            dataTask[j].mixAnama = nm;

            
            //nama mix B
            nm = "";            
            for (int b = 0; b < 16; b++)
            {
                
                nmTes = char(ee_data[71 + b]);
                if(nmTes != NULL){
                    nm += nmTes;
                }
            }

            nm.replace(" ","");
            dataTask[j].mixBnama = nm;

            //nama mix C
            nm = "";            
            for (int b = 0; b < 16; b++)
            {
                
                nmTes = char(ee_data[87 + b]);
                if(nmTes != NULL){
                    nm += nmTes;
                }
            }

            nm.replace(" ","");
            dataTask[j].mixCnama = nm;
            

        }
    }
    else
    {
       simpanDefaultEEPROM();
    }
}

void simpanDefaultEEPROM(){
     // isi eeprom dengan default
        for (int j = 0; j < dataTaskLen; j++)
        {
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j)), defaultTask[j].enable);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + MEM_AKTUATOR_USE1), defaultTask[j].aktuatorUse1);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + MEM_AKTUATOR_USE2), defaultTask[j].aktuatorUse2);

            for (int a = 0; a < 15; a++)
            {
                EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 3 + a), '0');
            }

            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 18), defaultTask[j].sensorUse1);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 19), defaultTask[j].sensorUse2);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 20), defaultTask[j].sensorUse1Type);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 21), defaultTask[j].sensorUse2Type);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 22), defaultTask[j].targetBawah);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 23), defaultTask[j].targetAtas);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 24), defaultTask[j].inverseMode);
            
            for (int b = 0; b < 16; b++)
            {
                EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 25 + b), defaultTask[j].nama.charAt(b));
            }

            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 41), defaultTask[j].aktuatorMixA);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 42), defaultTask[j].aktuatorMixB);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 43), defaultTask[j].aktuatorMixC);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 44), defaultTask[j].aktuatorAduk);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 45), defaultTask[j].aktuatorMixOut);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 46), defaultTask[j].flowSensorA);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 47), defaultTask[j].flowSensorB);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 48), defaultTask[j].flowSensorC);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 49), defaultTask[j].flowMixOut);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 50), defaultTask[j].targetMixA);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 51), defaultTask[j].targetMixB);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 52), defaultTask[j].targetMixC);
            EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 53), defaultTask[j].mixingTarget);
            for (int b = 0; b < 16; b++)
            {
                EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 54 + b), defaultTask[j].mixAnama.charAt(b));
            }
            for (int b = 0; b < 16; b++)
            {
                EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 71 + b), defaultTask[j].mixBnama.charAt(b));
            }
            for (int b = 0; b < 16; b++)
            {
                EEPROM.write((EE_TASK_START + (MEM_TASK_USE * j) + 87 + b), defaultTask[j].mixCnama.charAt(b));
            }
            EEPROM.commit();
        }
        EEPROM.write(EE_TASK_LEN, dataTaskLen);
        EEPROM.write(EE_READY_CODE, 0xAA);
        EEPROM.commit();

        Serial.println("Simpan default value task");
}

void simpanEnable(uint8_t idx, uint8_t value)
{
    EEPROM.write((EE_TASK_START + (idx * MEM_TASK_USE) + 0), value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan Enable: ");
    Serial.println(value);
    #endif
}

void simpanAktuatorUse1(uint8_t idx, uint8_t nomerAktuator)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_AKTUATOR_USE1, nomerAktuator);
    EEPROM.commit();
    #ifdef DEBUG_EEPROM
    Serial.print("Simpan aktuator1: ");
    Serial.println(nomerAktuator);
    #endif
}
void simpanAktuatorUse2(uint8_t idx, uint8_t nomerAktuator)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_AKTUATOR_USE2, nomerAktuator);
    EEPROM.commit();
    #ifdef DEBUG_EEPROM
    Serial.print("Simpan aktuator2: ");
    Serial.println(nomerAktuator);
    #endif
}

void simpanSensorUse1(uint8_t idx, uint8_t nomerSensor)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_SENSOR_USE1, nomerSensor);
    EEPROM.commit();
    #ifdef DEBUG_EEPROM
    Serial.print("Simpan sensorUse1: ");
    Serial.println(nomerSensor);
    #endif
}

void simpanSensorUse2(uint8_t idx, uint8_t nomerSensor)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_SENSOR_USE2, nomerSensor);
    EEPROM.commit();
    #ifdef DEBUG_EEPROM
    Serial.print("Simpan sensorUse2: ");
    Serial.println(nomerSensor);
    #endif
}

void simpanSensorUse1Type(uint8_t idx, uint8_t nomerSensor)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_SENSOR_USE1_TYPE, nomerSensor);
    EEPROM.commit();
    #ifdef DEBUG_EEPROM
    Serial.print("Simpan sensorUse1 type: ");
    Serial.println(nomerSensor);
    #endif
}

void simpanSensorUse2Type(uint8_t idx, uint8_t nomerSensor)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_SENSOR_USE2_TYPE, nomerSensor);
    EEPROM.commit();
    #ifdef DEBUG_EEPROM
    Serial.print("Simpan sensorUse2 type: ");
    Serial.println(nomerSensor);
    #endif
}

void simpanTargetBawah(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_TARGET_BAWAH, value);
    EEPROM.commit();
    #ifdef DEBUG_EEPROM
    Serial.print("Simpan target Bawah: ");
    Serial.println(value);
    #endif
}

void simpanTargetAtas(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_TARGET_ATAS, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan target Atas: ");
    Serial.println(value);
    #endif
}

void simpanInverseMode(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_INVERSE_MODE, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan Inverse mode: ");
    Serial.println(value);
    #endif
}

void simpanAktuatorMixA(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_AKTUATOR_MIXA, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan aktuator MixA: ");
    Serial.println(value);
    #endif
}

void simpanAktuatorMixB(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_AKTUATOR_MIXB, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan aktuator MixB: ");
    Serial.println(value);
    #endif
}

void simpanAktuatorMixC(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_AKTUATOR_MIXC, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan aktuator MixC: ");
    Serial.println(value);
    #endif
}
void simpanAktuatorAduk(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_AKTUATOR_MIXADUK, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan aktuator Aduk ");
    Serial.println(value);
    #endif
}


void simpanAktuatorMixOut(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_AKTUATOR_MIXOUT, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan aktuator Mix out: ");
    Serial.println(value);
    #endif
}

void simpanFlowSensorA(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_FLOW_SENSORA, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan flowsensor A: ");
    Serial.println(value);
    #endif
}

void simpanFlowSensorB(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_FLOW_SENSORB, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan flowsensor B: ");
    Serial.println(value);
    #endif
}

void simpanFlowSensorC(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_FLOW_SENSORC, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan flowsensor C: ");
    Serial.println(value);
    #endif
}

void simpanFlowMixOut(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_FLOW_MIXOUT, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan flowsensor MixOut: ");
    Serial.println(value);
    #endif
}

void simpanTargetMixA(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_TARGET_MIXA, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan target MixA: ");
    Serial.println(value);
    #endif
}

void simpanTargetMixB(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_TARGET_MIXB, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan target MixB: ");
    Serial.println(value);
    #endif
}

void simpanTargetMixC(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_TARGET_MIXC, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan target MixC: ");
    Serial.println(value);
    #endif
}

void simpanTargetMixOut(uint8_t idx, uint8_t value)
{
    EEPROM.write(EE_TASK_START + (idx * MEM_TASK_USE) + MEM_TARGET_MIXOUT, value);
    EEPROM.commit();
     #ifdef DEBUG_EEPROM
    Serial.print("Simpan target MixOut: ");
    Serial.println(value);
    #endif
}

void simpanMixANama(String nama,uint8_t idx){
    int i = 0;
    if(nama.length() < 16){
        for(i = 0; i <(16 - nama.length());i++){
            nama += " ";
        }
    }
    for(i = 0;i < 16;i++){
        EEPROM.write((EE_TASK_START + (MEM_TASK_USE * idx) + MEM_MIXA_NAMA + i),nama.charAt(i));
    }
    EEPROM.commit();
    Serial.print("Simpan nama MixA: ");
    Serial.println(nama);
}

void simpanMixBNama(String nama,uint8_t idx){
    int i = 0;
    if(nama.length() < 16){
        for(i = 0; i <(16 - nama.length());i++){
            nama += " ";
        }
    }
    for(i = 0;i < 16;i++){
        EEPROM.write((EE_TASK_START + (MEM_TASK_USE * idx) + MEM_MIXB_NAMA + i),nama.charAt(i));
    }
    EEPROM.commit();
    Serial.print("Simpan nama MixB: ");
    Serial.println(nama);
}

void simpanMixCNama(String nama,uint8_t idx){
    int i = 0;
    if(nama.length() < 16){
        for(i = 0; i <(16 - nama.length());i++){
            nama += " ";
        }
    }
    for(i = 0;i < 16;i++){
        EEPROM.write((EE_TASK_START + (MEM_TASK_USE * idx) + MEM_MIXC_NAMA + i),nama.charAt(i));
    }
    EEPROM.commit();
    Serial.print("Simpan nama MixC: ");
    Serial.println(nama);
}
