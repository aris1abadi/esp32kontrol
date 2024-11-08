#include <Arduino.h>
#include <timeHandle.h>
#include <nodeHandle.h>
#include <mySensorsHandle.h>
#include <eepromHandle.h>
#include <kontrolHandle.h>
#include <mqttHandle.h>
#include <ArduinoJson.h>
#include <oledHandle.h>
// #include <MyMessage.h>

// inverseMode 0(dibawah targetBawah On,diatas target atas OFF)lengas,temperature
// inverseMode 1(dibawah targetBawah OFF,diatas target atas ON)humidity
// inverseMode 2(untuk ABmix)
// inverseMode 3(sama dengan mode 0 hanya tampilan pada dispaly berbeda uituk indikator air)

#define DEBUG_MQTT
#define USE_DELAY_SEND

#define ON 1
#define OFF 0
#define AKTUATOR_DELAY 3

uint8_t jadwalDelay = 0; // 1 menit delay untuk jadwal 4x60
uint8_t taskDelay = 0;   // satu detik 4 tick dari delay kontrol 250ms
bool sendToNodeStatus = false;
uint8_t jmlNode = 0;
uint8_t sendCount = 0;

String type_task = "";
String cmd = "";
String prefixAuto = "";

bool sendToSensorStatus = false;
uint8_t jmlSensor = 0;
uint8_t sendSensorCount = 0;
uint8_t lastSendCount = 0;

#define AUTOLEN 6

struct sn
{
    uint8_t nomerNode;
    uint8_t msg_type;
    uint8_t msg; // perintah 1=ON,0=OFF
    /* data */
};
sn aktuatorProp;
sn sensorProp;

sn sensorKontrolAntrian[10];
sn aktuatorKontrolAntrian[20];
extern mySensor sensorHumidity[];

myTask dataTask[AUTOLEN] = {
    //                                                                                                                                                                                    0123456789012345   0123456789012345   0123456789012345
    {0, 1, 1, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 1, 1, 1, 30, 32, 0, "Temperature     ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 2, 2, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 1, 2, 2, 60, 80, 1, "Humidity        ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 5, 6, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 1, 3, 3, 80, 90, 0, "Lengas          ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 7, 8, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 1, 5, 5, 5, 10, 0, "Genangan        ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 3, 3, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 2, 2, 4, 4, 10, 20, 3, "Intermittent    ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 10, 10, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 2, 6, 6, 30, 32, 2, "ABMix           ", 0, 0, 21, 22, 23, 24, 3, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 50, 180, 0, "Pupuk1          ", "Pupuk2          ", "Air             "},

};

myTask defaultTask[AUTOLEN] = {
    //                                                                                                                                                                                    0123456789012345   0123456789012345   0123456789012345
    {0, 1, 1, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 1, 1, 1, 30, 32, 0, "Temperature     ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 2, 2, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 1, 2, 2, 60, 80, 1, "Humidity        ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 5, 6, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 1, 3, 3, 80, 90, 1, "Lengas          ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 7, 8, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 1, 5, 5, 5, 10, 0, "Genangan        ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 3, 3, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 2, 2, 4, 4, 10, 20, 3, "Intermittent    ", 0, 0, 11, 12, 13, 14, 15, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 0, "Furadan         ", "BioCurah        ", "Air             "},
    {0, 10, 10, 0, 0, 0, /*jadwal 15byte*/ "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0", /*end jadwal*/ 1, 2, 6, 6, 30, 32, 2, "ABMix           ", 0, 0, 21, 22, 23, 24, 3, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 50, 180, 0, "Pupuk1          ", "Pupuk2          ", "Air             "},

};

int dataTaskLen = sizeof(dataTask) / sizeof(dataTask[0]);

String getTask(uint8_t index)
{
    // String outMsg = "{";
    JsonDocument outmsg;

    outmsg["enable"] = dataTask[index - 1].enable;
    outmsg["aktuatorUse1"] = dataTask[index - 1].aktuatorUse1;
    outmsg["aktuatorUse2"] = dataTask[index - 1].aktuatorUse2;
    outmsg["status"] = dataTask[index - 1].status;
    outmsg["jadwal"] = dataTask[index - 1].jadwal;
    outmsg["sensorUse1"] = dataTask[index - 1].sensorUse1;
    outmsg["sensorUse2"] = dataTask[index - 1].sensorUse2;
    outmsg["sensorUse1Type"] = dataTask[index - 1].sensorUse1Type;
    outmsg["sensorUse2Type"] = dataTask[index - 1].sensorUse2Type;
    outmsg["targetBawah"] = dataTask[index - 1].targetBawah;
    outmsg["targetAtas"] = dataTask[index - 1].targetAtas;
    outmsg["inverseMode"] = dataTask[index - 1].inverseMode;
    outmsg["nama"] = dataTask[index - 1].nama;
    if (dataTask[index - 1].inverseMode == 2)
    {
        outmsg["aktuatorMixA"] = dataTask[index - 1].aktuatorMixA;
        outmsg["aktuatorMixB"] = dataTask[index - 1].aktuatorMixB;
        outmsg["aktuatorMixC"] = dataTask[index - 1].aktuatorMixC;
        outmsg["aktuatorAduk"] = dataTask[index - 1].aktuatorAduk;
        outmsg["aktuatorMixOut"] = dataTask[index - 1].aktuatorMixOut;
        outmsg["flowAValue"] = 0;
        outmsg["flowBValue"] = 0;
        outmsg["flowCValue"] = 0;
        outmsg["flowMixOutValue"] = 0;
        outmsg["targetMixA"] = dataTask[index - 1].targetMixA;
        outmsg["targetMixB"] = dataTask[index - 1].targetMixB;
        outmsg["targetMixC"] = dataTask[index - 1].targetMixC;
        outmsg["mixingTarget"] = dataTask[index - 1].mixingTarget;
        outmsg["mixANama"] = dataTask[index - 1].mixAnama;
        outmsg["mixBNama"] = dataTask[index - 1].mixBnama;
        outmsg["mixCNama"] = dataTask[index - 1].mixCnama;
    }

    char jsMsg[1024];
    serializeJson(outmsg, jsMsg);
    // char outMsg[512];
    // sprintf(outMsg, "{C:%d,mixingTarget:%d}", dataTask[index - 1].enable, dataTask[index - 1].aktuatorUse1, dataTask[index - 1].aktuatorUse2, dataTask[index - 1].status, dataTask[index - 1].jadwal, dataTask[index - 1].sensorUse1, dataTask[index - 1].sensorUse2, dataTask[index - 1].sensorUse1Type, dataTask[index - 1].sensorUse2Type, dataTask[index - 1].targetBawah, dataTask[index - 1].targetAtas, dataTask[index - 1].inverseMode, dataTask[index - 1].nama, dataTask[index - 1].aktuatorMixA, dataTask[index - 1].aktuatorMixB, dataTask[index - 1].aktuatorMixC, dataTask[index - 1].aktuatorAduk, dataTask[index - 1].aktuatorMixOut, dataTask[index - 1].flowSensorA, dataTask[index - 1].flowSensorB, dataTask[index - 1].flowSensorC, dataTask[index - 1].flowMixOut, dataTask[index - 1].targetMixA, dataTask[index - 1].targetMixB, dataTask[index - 1].targetMixC, dataTask[index - 1].mixingTarget);

    return String(jsMsg);
}

void kontrol_init()
{
    // loadKontrol();
    //  testing node
}
/*
format pesan
-topic  abadinet-in/deviceId/type(kontrol/aktuator/sensor)/nomer device/cmd

-msg payload
*/

void cekMsg(String topic, String msg)
{
    // Serial.println(topic);
    type_task = getValue(topic, '/', 2);
    int setGet = (getValue(topic, '/', 3)).toInt();
    cmd = getValue(topic, '/', 4);
    prefixAuto = "auto";
    // untuk setGet jika 0 remote meminta value,jika 1 remote set value,3 response dari server
    if (cmd == "enable")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].enable = msg.toInt();
                    // simpan data di eeprom
                    simpanEnable(i, dataTask[i].enable);
                    updateDisplay();
                    if (dataTask[i].enable == 0)
                    {
                        // matikan aktuator
                        if (dataTask[i].inverseMode == 2)
                        {
                            dataTask[i].flowAValue = 0;
                            dataTask[i].flowBValue = 0;
                            dataTask[i].flowCValue = 0;
                            dataTask[i].flowMixOutValue = 0;

                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixA;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[0] = aktuatorProp;
                            
                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixA;
                            aktuatorProp.msg = AKTUATOR_DELAY;
                            aktuatorKontrolAntrian[1] = aktuatorProp;

                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixB;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[2] = aktuatorProp;

                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixC;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[3] = aktuatorProp;

                            aktuatorProp.nomerNode = dataTask[i].aktuatorAduk;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[4] = aktuatorProp;

                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixOut;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[5] = aktuatorProp;

                            jmlNode = 6;
                            sendCount = 0;
                            sendToNodeStatus = true;
                        }
                        else
                        {
                            aktuatorProp.nomerNode = dataTask[i].aktuatorUse1;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[0] = aktuatorProp;
                            
                            aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[1] = aktuatorProp;

                            jmlNode = 2;
                            sendCount = 0;
                            sendToNodeStatus = true;
                        }

                        dataTask[i].status = 0;
                    }
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" enable: ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(type_task, 3, cmd, String(dataTask[i].enable));
            }
        }
    }
    else if (cmd == "aktuatorUse1")
    {

        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].aktuatorUse1 = msg.toInt();
                    simpanAktuatorUse1(i, dataTask[i].aktuatorUse1);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].aktuatorUse1));
            }
        }
    }
    else if (cmd == "aktuatorUse2")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].aktuatorUse2 = msg.toInt();
                    simpanAktuatorUse2(i, dataTask[i].aktuatorUse2);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].aktuatorUse2));
            }
        }
    }
    else if (cmd == "jadwal")
    {
    }
    else if (cmd == "sensorUse1")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].sensorUse1 = msg.toInt();
                    simpanSensorUse1(i, dataTask[i].sensorUse1);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].sensorUse1));
            }
        }
    }
    else if (cmd == "sensorUse2")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].sensorUse2 = msg.toInt();
                    simpanSensorUse2(i, dataTask[i].sensorUse2);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].sensorUse2));
            }
        }
    }
    else if (cmd == "targetBawah")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].targetBawah = msg.toInt();
                    simpanTargetBawah(i, dataTask[i].targetBawah);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].targetBawah));
            }
        }
    }
    else if (cmd == "targetAtas")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].targetAtas = msg.toInt();
                    simpanTargetAtas(i, dataTask[i].targetAtas);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].targetAtas));
            }
        }
    }
    else if (cmd == "aktuatorMixA")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].aktuatorMixA = msg.toInt();
                    simpanAktuatorMixA(i, dataTask[i].aktuatorMixA);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].aktuatorMixA));
            }
        }
    }
    else if (cmd == "aktuatorMixB")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].aktuatorMixB = msg.toInt();
                    simpanAktuatorMixB(i, dataTask[i].aktuatorMixB);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].aktuatorMixB));
            }
        }
    }
    else if (cmd == "aktuatorMixC")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].aktuatorMixC = msg.toInt();
                    simpanAktuatorMixC(i, dataTask[i].aktuatorMixC);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].aktuatorMixC));
            }
        }
    }
    else if (cmd == "aktuatorMixOut")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].aktuatorMixOut = msg.toInt();
                    simpanAktuatorMixOut(i, dataTask[i].aktuatorMixOut);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].aktuatorMixOut));
            }
        }
    }
    else if (cmd == "aktuatorAduk")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].aktuatorAduk = msg.toInt();
                    simpanAktuatorAduk(i, dataTask[i].aktuatorAduk);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].aktuatorAduk));
            }
        }
    }
    else if (cmd == "flowSensorA")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].flowSensorA = msg.toInt();
                    simpanFlowSensorA(i, dataTask[i].flowSensorA);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].flowSensorA));
            }
        }
    }
    else if (cmd == "flowSensorB")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].flowSensorB = msg.toInt();
                    simpanFlowSensorB(i, dataTask[i].flowSensorB);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].flowSensorB));
            }
        }
    }
    else if (cmd == "flowSensorC")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].flowSensorC = msg.toInt();
                    simpanFlowSensorC(i, dataTask[i].flowSensorC);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].flowSensorC));
            }
        }
    }
    else if (cmd == "flowMixOut")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].flowMixOut = msg.toInt();
                    simpanFlowMixOut(i, dataTask[i].flowMixOut);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].flowMixOut));
            }
        }
    }
    else if (cmd == "targetMixA")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].targetMixA = msg.toInt();
                    simpanTargetMixA(i, dataTask[i].targetMixA);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].targetMixA));
            }
        }
    }
    else if (cmd == "targetMixB")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].targetMixB = msg.toInt();
                    simpanTargetMixB(i, dataTask[i].targetMixB);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].targetMixB));
            }
        }
    }
    else if (cmd == "targetMixC")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].targetMixC = msg.toInt();
                    simpanTargetMixC(i, dataTask[i].targetMixC);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].targetMixC));
            }
        }
    }
    else if (cmd == "mixingTarget")
    {
        for (int i = 0; i < dataTaskLen; i++)
        {
            prefixAuto = "auto";
            prefixAuto += dataTask[i].nama;
            if (prefixAuto == type_task)
            {
                if (setGet == 1)
                {
                    dataTask[i].mixingTarget = msg.toInt();
                    simpanTargetMixOut(i, dataTask[i].mixingTarget);
#ifdef DEBUG_MQTT
                    Serial.print(prefixAuto);
                    Serial.print(" set ");
                    Serial.print(cmd);
                    Serial.print(": ");
                    Serial.println(msg);
#endif
                }

                kirimKeRemote(prefixAuto, 3, cmd, String(dataTask[i].mixingTarget));
            }
        }
    }
    else if (cmd == "getAllTask")
    {
        if (type_task == "kontrol")
        {
            String taskMsg = "";
            for (int i = 1; i < dataTaskLen + 1; i++)
            {
                if (i > 1)
                {
                    taskMsg += ';';
                }
                taskMsg += getTask(i);
            }

            // response ke remote
            kirimKeRemote(type_task, 3, cmd, taskMsg);
        }
    }
    else if (cmd == "namaMixA")
    {
    }
    else if (cmd == "namaMixB")
    {
    }
    else if (cmd == "ResetEEPROM")
    {
        if (type_task == "kontrol")
        {
            simpanDefaultEEPROM();
            Serial.println("Reset EEPROM dan reboot");
            delay(3000);
            ESP.restart();
        }
    }
}

void kontrol_loop()
{
    // cek task
    static unsigned long kontrolDelay = millis();
    if ((millis() - kontrolDelay) > 250)
    {
        kontrolDelay = millis();

#ifndef USE_DELAY_SEND
        if (sendToSensorStatus)
        {
            resetFlowSensor(sensorKontrolAntrian[sendSensorCount].nomerNode);
            // kirim_ke_device(sensorKontrolAntrian[sendSensorCount].nomerNode, uint8_t childnode, uint8_t type, String payload);
            if (++sendSensorCount > (jmlSensor - 1))
            {
                sendToSensorStatus = false;
            }
        }
        else if (sendToNodeStatus)
        {

            if (aktuatorKontrolAntrian[sendCount].msg != 3)
            {
                kontrolAktuator(aktuatorKontrolAntrian[sendCount].nomerNode, aktuatorKontrolAntrian[sendCount].msg);
            }
            if (++sendCount > (jmlNode - 1))
            {
                sendToNodeStatus = false;
            }
        }
#endif
        // delay 1 detik untuk cek task
        if (++taskDelay > 4)
        {
            taskDelay = 0;
            // cek automatis disini
            /*
            for(int i= 0;i< 4;i++){
                Serial.print("Hum sensor");
                Serial.print(i);
                Serial.print(": ");
               Serial.println(sensorHumidity[i].val);
            }
            */
#ifdef USE_DELAY_SEND
            if (sendToNodeStatus)
            {

                kontrolAktuator(aktuatorKontrolAntrian[sendCount].nomerNode, aktuatorKontrolAntrian[sendCount].msg);

                if (++sendCount > (jmlNode - 1))
                {
                    sendToNodeStatus = false;
                }
            }

#endif
            for (int i = 0; i < dataTaskLen; i++)
            {
                // update data tampilan display/online disini
                String namaTask = "auto";
                namaTask += dataTask[i].nama;

                dataTask[i].nowValue = getSensorValue(dataTask[i].sensorUse1Type, dataTask[(i)].sensorUse1);
                /*
                Serial.print(dataTask[i].nama);
                Serial.print("-");
                Serial.print(dataTask[i].sensorUse1Type);
                Serial.print("-");
                Serial.print(dataTask[i].sensorUse1);
                Serial.print("-");
                Serial.println(dataTask[i].nowValue);
                */

                if (dataTask[i].lastValue != dataTask[i].nowValue)
                {
                    dataTask[i].lastValue = dataTask[i].nowValue;
                    // update data display
                    // update data di android remote

                    kirimKeRemote(namaTask, 0, "sensorValue", String(dataTask[i].nowValue, 1));
                }

                if (dataTask[i].enable == 1)
                {
                    // inverseMode 0(dibawah target On,diatas target atas OFF)
                    // inverseMode 1(dibawah target OFF,diatas target atas ON)
                    // inverseMode 2(untuk ABmix)
                    // inverseMode 3(sama dengan mode 0 hanya tampilan pada dispaly berbeda uituk indikator air)
                    if (dataTask[i].inverseMode == 0)
                    {
                        // lengas,genangan,intermitten
                        if (dataTask[i].nowValue <= dataTask[i].targetBawah)
                        {
                            if (dataTask[i].status == 1)
                            {
                                dataTask[i].status = 0;
                                // aktuator OFF
                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse1;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[0] = aktuatorProp;
                                // ulang pesan
                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[1] = aktuatorProp;
                                /*
                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[2] = aktuatorProp;

                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[3] = aktuatorProp;
                                */

                                jmlNode = 2;
                                sendCount = 0;
                                sendToNodeStatus = true;
                            }
                        }
                        else if (dataTask[i].nowValue >= dataTask[i].targetAtas)
                        {
                            if (dataTask[i].status == 0)
                            {
                                dataTask[i].status = 1;
                                // aktuator ON
                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse1;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[0] = aktuatorProp;

                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[1] = aktuatorProp;
/*
                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[2] = aktuatorProp;

                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[3] = aktuatorProp;
*/
                                jmlNode = 2;
                                sendCount = 0;
                                sendToNodeStatus = true;
                            }
                        }
                    }
                    else if (dataTask[i].inverseMode == 1)
                    {
                        if (dataTask[i].nowValue <= dataTask[i].targetBawah)
                        {
                            if (dataTask[i].status == 0)
                            {
                                dataTask[i].status = 1;
                                // aktuator ON
                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse1;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[0] = aktuatorProp;

                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[1] = aktuatorProp;
/*
                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[2] = aktuatorProp;

                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[3] = aktuatorProp;
                                */

                                jmlNode = 2;
                                sendCount = 0;
                                sendToNodeStatus = true;
                            }
                        }
                        else if (dataTask[i].nowValue >= dataTask[i].targetAtas)
                        {
                            if (dataTask[i].status == 1)
                            {
                                dataTask[i].status = 0;
                                // aktuator ON
                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse1;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[0] = aktuatorProp;

                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[1] = aktuatorProp;
/*
                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[2] = aktuatorProp;

                                aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[3] = aktuatorProp;
                                */

                                jmlNode = 2;
                                sendCount = 0;
                                sendToNodeStatus = true;
                            }
                        }
                    }
                    else if (dataTask[i].inverseMode == 2)
                    {
                        // data flow hanya dikirim saat abmix enable
                        dataTask[i].flowAValue = getSensorValue(5, dataTask[i].aktuatorMixA);
                        dataTask[i].flowBValue = getSensorValue(5, dataTask[i].aktuatorMixB);
                        dataTask[i].flowCValue = getSensorValue(5, dataTask[i].aktuatorMixC);
                        dataTask[i].flowMixOutValue = getSensorValue(5, dataTask[i].flowMixOutValue);
                        // cek perubhan sensor flow
                        if (dataTask[i].flowALastValue != dataTask[i].flowAValue)
                        {
                            dataTask[i].flowALastValue = dataTask[i].flowAValue;
                            kirimKeRemote(namaTask, 0, "sensorFlowAValue", String(dataTask[i].flowAValue, 2));
                        }

                        if (dataTask[i].flowBLastValue != dataTask[i].flowBValue)
                        {
                            dataTask[i].flowBLastValue = dataTask[i].flowBValue;
                            kirimKeRemote(namaTask, 0, "sensorFlowBValue", String(dataTask[i].flowBValue, 2));
                        }
                        if (dataTask[i].flowCLastValue != dataTask[i].flowCValue)
                        {
                            dataTask[i].flowCLastValue = dataTask[i].flowCValue;
                            kirimKeRemote(namaTask, 0, "sensorFlowCValue", String(dataTask[i].flowCValue, 2));
                        }
                        if (dataTask[i].flowMixOutLastValue != dataTask[i].flowMixOutValue)
                        {
                            dataTask[i].flowMixOutLastValue = dataTask[i].flowMixOutValue;
                            kirimKeRemote(namaTask, 0, "sensorFlowMixOutValue", String(dataTask[i].flowMixOutValue, 2));
                        }
                        // mix mode
                        if (dataTask[i].status == 0)
                        {
                            dataTask[i].flowAValue = 0;
                            dataTask[i].flowBValue = 0;
                            dataTask[i].flowCValue = 0;
                            dataTask[i].flowMixOutValue = 0;

                            dataTask[i].flowALastValue = 0;
                            dataTask[i].flowBLastValue = 0;
                            dataTask[i].flowCLastValue = 0;
                            dataTask[i].flowMixOutLastValue = 0;
                            // aktuator MixA ON;
                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixA;
                            aktuatorProp.msg = ON;
                            aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixA;
                            aktuatorProp.msg = ON;
                            aktuatorKontrolAntrian[1] = aktuatorProp;
                            */

                            jmlNode = 1;
                            sendCount = 0;
                            sendToNodeStatus = true;

                            dataTask[i].targetMixAStatus = 0;
                            dataTask[i].status = 1;
                        }
                        else if (dataTask[i].status == 1)
                        { // nyalakan pompa mixB

                            if ((dataTask[i].flowAValue >= (dataTask[i].targetMixA / 4)) && (dataTask[i].targetMixAStatus == 0))
                            {
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixB;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixB;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[1] = aktuatorProp;
*/
                                jmlNode = 1;
                                sendCount = 0;
                                sendToNodeStatus = true;
                                dataTask[i].targetMixBStatus = 0;

                                dataTask[i].status = 2;
                            }
                        }
                        else if (dataTask[i].status == 2)
                        { // aktuator C On

                            if ((dataTask[i].flowBValue >= (dataTask[i].targetMixB / 4)) && (dataTask[i].targetMixBStatus == 0))
                            {
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixC;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixC;
                                aktuatorProp.msg = ON;
                                aktuatorKontrolAntrian[1] = aktuatorProp;
                                */

                                jmlNode = 1;
                                sendCount = 0;
                                sendToNodeStatus = true;
                                dataTask[i].targetMixCStatus = 0;

                                dataTask[i].status = 3;
                            }
                        }
                        else if (dataTask[i].status == 3)
                        { // cektarget

                            if ((dataTask[i].flowAValue >= dataTask[i].targetMixA) && (dataTask[i].targetMixAStatus == 0))
                            {

                                dataTask[i].targetMixAStatus = 1;
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixA;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixA;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[1] = aktuatorProp;
*/
                                jmlNode = 1;
                                sendCount = 0;
                                sendToNodeStatus = true;
                            }

                            if ((dataTask[i].flowBValue >= dataTask[i].targetMixB) && (dataTask[i].targetMixBStatus == 0))
                            {

                                dataTask[i].targetMixBStatus = 1;
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixB;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixB;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[1] = aktuatorProp;
*/
                                jmlNode = 1;
                                sendCount = 0;
                                sendToNodeStatus = true;
                            }

                            if ((dataTask[i].flowCValue >= dataTask[i].targetMixC) && (dataTask[i].targetMixCStatus == 0))
                            {

                                dataTask[i].targetMixCStatus = 1;
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixC;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                                aktuatorProp.nomerNode = dataTask[i].aktuatorMixC;
                                aktuatorProp.msg = OFF;
                                aktuatorKontrolAntrian[1] = aktuatorProp;
                                */

                                jmlNode = 1;
                                sendCount = 0;
                                sendToNodeStatus = true;
                            }
                            if ((dataTask[i].targetMixAStatus == 1) && (dataTask[i].targetMixBStatus == 1) && (dataTask[i].targetMixCStatus == 1))
                            {

                                dataTask[i].status = 4;

                                dataTask[i].mixingCount = 0;
                            }
                        }
                        else if (dataTask[i].status == 4)
                        { // aduk campuran
                            aktuatorProp.nomerNode = dataTask[i].aktuatorAduk;
                            aktuatorProp.msg = ON;
                            aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                            aktuatorProp.nomerNode = dataTask[i].aktuatorAduk;
                            aktuatorProp.msg = ON;
                            aktuatorKontrolAntrian[1] = aktuatorProp;
*/
                            jmlNode = 1;
                            sendCount = 0;
                            sendToNodeStatus = true;

                            dataTask[i].status = 5;

                            dataTask[i].mixingCount = 0;
                        }
                    }
                    else if (dataTask[i].status == 5)
                    {
                        dataTask[i].mixingCount++;
                        if (dataTask[i].mixingCount >= dataTask[i].mixingTarget)
                        {
                            aktuatorProp.nomerNode = dataTask[i].aktuatorAduk;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                            aktuatorProp.nomerNode = dataTask[i].aktuatorAduk;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[1] = aktuatorProp;

                            aktuatorProp.nomerNode = dataTask[i].aktuatorAduk;
                            aktuatorProp.msg = AKTUATOR_DELAY;
                            aktuatorKontrolAntrian[2] = aktuatorProp;

                            aktuatorProp.nomerNode = dataTask[i].aktuatorAduk;
                            aktuatorProp.msg = AKTUATOR_DELAY;
                            aktuatorKontrolAntrian[3] = aktuatorProp;

                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixOut;
                            aktuatorProp.msg = ON;
                            aktuatorKontrolAntrian[4] = aktuatorProp;
                            */

                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixOut;
                            aktuatorProp.msg = ON;
                            aktuatorKontrolAntrian[1] = aktuatorProp;

                            jmlNode = 2;
                            sendCount = 0;
                            sendToNodeStatus = true;

                            dataTask[i].targetMixOut = dataTask[i].targetMixA + dataTask[i].targetMixB + dataTask[i].targetMixC;
                            dataTask[i].status = 6;
                        }

                        //---------------------------
                    }
                    if (dataTask[i].status == 6)
                    {
                        if (dataTask[i].flowMixOutValue > dataTask[i].targetMixOut)
                        {
                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixOut;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                            aktuatorProp.nomerNode = dataTask[i].aktuatorMixOut;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[1] = aktuatorProp;
*/
                            jmlNode = 1;
                            sendCount = 0;
                            sendToNodeStatus = true;
                            dataTask[i].status = 0;
                            dataTask[i].enable = 0;
                        }
                    }
                }
                else if (dataTask[i].inverseMode == 3)
                {
                    // intermitten
                    if (dataTask[i].nowValue <= dataTask[i].targetBawah)
                    {
                        if (dataTask[i].status == 0)
                        {
                            dataTask[i].status = 1;
                            // aktuator ON
                            aktuatorProp.nomerNode = dataTask[i].aktuatorUse1;
                            aktuatorProp.msg = ON;
                            aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                            aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                            aktuatorProp.msg = ON;
                            aktuatorKontrolAntrian[1] = aktuatorProp;
                            */

                            jmlNode = 1;
                            sendCount = 0;
                            sendToNodeStatus = true;
                        }
                    }
                    else if (dataTask[i].nowValue >= dataTask[i].targetAtas)
                    {
                        if (dataTask[i].status == 1)
                        {
                            dataTask[i].status = 0;
                            // aktuator ON
                            aktuatorProp.nomerNode = dataTask[i].aktuatorUse1;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[0] = aktuatorProp;
/*
                            aktuatorProp.nomerNode = dataTask[i].aktuatorUse2;
                            aktuatorProp.msg = OFF;
                            aktuatorKontrolAntrian[1] = aktuatorProp;
                            */

                            jmlNode = 1;
                            sendCount = 0;
                            sendToNodeStatus = true;
                        }
                    }
                }
            }
        }
        // cek jdwal tiap 1 menit
        if (++jadwalDelay > 240)
        {
            jadwalDelay = 0;
            // cek jadwal disini
        }
    }
}
