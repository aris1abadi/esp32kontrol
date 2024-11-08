
#include <Arduino.h>
#include <Ticker.h>

#include <mySensorsHandle.h>
#include <nodeHandle.h>
#include <timeHandle.h>
// #include <MyMessage.h>

#define DEBUG_NODE

#define ON 1
#define OFF 0
// #define DEBUG_MYSENSOR
//  sensor NODE

#define V_STATUS 2
#define V_VAR1 24
#define V_VAR2 25
#define V_VAR3 26
#define V_VAR4 27
#define V_TEMP 0
#define V_HUM 1
#define V_TRIPPED 16
#define V_CURRENT 39
#define V_LEVEL 37
#define V_DISTANCE 13
#define  V_FLOW 34
#define V_VOLUME 35

#define I_BATTERY_LEVEL 0

Ticker MySensorTimer;

extern myNode aktuator[];
extern mySensor sensorLengas[];
extern mySensor sensorTemperature[];
extern mySensor sensorHumidity[];
extern mySensor sensorLengasIntermittent[];
extern mySensor sensorFlow[];

String mqttMsgInput = "";
bool mqttMsgReady = false;

String webMsg = "";

// hitung air
unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres = 0;
float totalLitres = 0;

int pestisidaCount = 0;
int lastPestisidaCount = 0;

float kalibrasiAir = 1.0; // di jadikan float pada pemakaian

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void kirim_ke_device(uint8_t node, uint8_t childnode, uint8_t type, String payload)
{
    String strOut = "";
    String resp = "";
    int isi = payload.toInt();

    strOut = String(node);
    strOut += ";";
    strOut += String(childnode);
    strOut += ";1;1;";
    strOut += String(type);
    strOut += ";";
    strOut += String(isi);
    strOut += "\n";

    mqttMsgInput = strOut;
    mqttMsgReady = true;
    Serial.println(strOut);
}

void kontrolAktuator(uint8_t nomerAktuator, uint8_t on_off)
{
    String strOut = "";
    String resp = "";

    strOut += String(aktuator[nomerAktuator - 1].nodeId);
    strOut += ';';
    strOut += String(aktuator[nomerAktuator - 1].childId);
    strOut += ";1;1;2;";
    if (on_off == ON)
    {
        strOut += "1\n";
        resp = "1";
    }
    else
    {
        strOut += "0\n";
        resp = "0";
    }
    mqttMsgInput = strOut;
    mqttMsgReady = true;
    Serial.println(strOut);
    // kirimKeRemote(String type,uint8_t nomer,String cmd,String msg)
    // kirimKeRemote("aktuator", nomerPompa, "status", resp);
}

void resetFlowSensor(uint8_t nomerSensor)
{
    // sensorFlow melekat pada aktuator
    String msg = String(sensorFlow[nomerSensor].nodeId);
    msg += ';';
    msg += String(sensorFlow[nomerSensor].childId);
    msg += ";1;1;25;0\n"; // v_VAR
    // kirim ke mySensor network
    mqttMsgInput = msg;
    mqttMsgReady = true;
}

void setSensorInterval(uint8_t interval, uint8_t nomerSensor, uint8_t jenisSensor)
{
    // interval dalam menit 1 - 60 menit
    // 0 untuk tes/demo kirim data tiap 2 detik
    String msg = "";
    if (jenisSensor == 1)
    {
        // lengas
        // lengas laser(sensor intermittent)
        msg = String(sensorLengas[nomerSensor - 1].nodeId);
        msg += ';';
        msg += String(sensorLengas[nomerSensor - 1].childId);
        msg += ";1;1;24;"; // v_VAR2
        msg += String(interval);
        msg += "\n";
        // kirim ke mySensor network

        mqttMsgInput = msg;
        mqttMsgReady = true;
    }
    else if (jenisSensor == 2)
    {
        // lengas laser(sensor intermittent)
        msg = String(sensorLengasIntermittent[nomerSensor - 1].nodeId);
        msg += ';';
        msg += String(sensorLengasIntermittent[nomerSensor - 1].childId);
        msg += ";1;1;24;"; // v_VAR2
        msg += String(interval);
        msg += "\n";
        // kirim ke mySensor network

        mqttMsgInput = msg;
        mqttMsgReady = true;
    }
    else if (jenisSensor == 3)
    {
        // ketinggian air
    }
}

void cekMySensors(String msg)
{
    uint8_t nodeId = (getValue(msg, ';', 0)).toInt();
    uint8_t childId = (getValue(msg, ';', 1)).toInt();
    uint8_t type_msg = (getValue(msg, ';', 4)).toInt();
    String pl = getValue(msg, ';', 5);
    String payload = "";
    if (pl.length() > 0)
    {
        for (uint8_t i = 0; i < pl.length(); i++)
        {
            if (pl.charAt(i) == '\r' || pl.charAt(i) == '\n')
            {
                break;
            }
            else
            {
                payload += pl.charAt(i);
            }
        }
    }
// update data/status
#ifdef DEBUG_NODE
    Serial.print(msg);
#endif
#ifdef DEBUG_MYSENSOR

    Serial.print("nodeId: ");
    Serial.print(nodeId);
    Serial.print(" chidId: ");
    Serial.print(childId);
    Serial.print(" Type: ");
    Serial.print(type_msg);
    Serial.print(" payload:");
    Serial.println(payload);
#endif
    // cek aktuator
    for (int i = 0; i < 24; i++)
    {
        if (aktuator[i].nodeId == nodeId)
        {
            if (aktuator[i].childId == childId)
            {
                if (type_msg == V_STATUS)
                {
                    aktuator[i].status = 1; // update status
                    aktuator[i].lastUpdate[0] = getTahun();
                    aktuator[i].lastUpdate[1] = getBl();
                    aktuator[i].lastUpdate[2] = getTgl();
                    aktuator[i].lastUpdate[3] = getJam();
                    aktuator[i].lastUpdate[4] = getMenit();
                    aktuator[i].lastUpdate[5] = getDtk();
                    aktuator[i].timeOut = 0;

                    aktuator[i].val = payload.toInt();

#ifdef DEBUG_NODE
                    Serial.print(aktuator[i].nama);
                    Serial.print(" update val: ");
                    Serial.print(payload);
                    Serial.print(" waktu: ");
                    Serial.print(aktuator[i].lastUpdate[0]);
                    Serial.print('-');
                    Serial.print(aktuator[i].lastUpdate[1]);
                    Serial.print('-');
                    Serial.print(aktuator[i].lastUpdate[2]);
                    Serial.print(' ');
                    Serial.print(aktuator[i].lastUpdate[3]);
                    Serial.print(':');
                    Serial.print(aktuator[i].lastUpdate[4]);
                    Serial.print(':');
                    Serial.println(aktuator[i].lastUpdate[5]);
#endif
                }
            }
            else if (sensorFlow[i].childId == childId)
            {
                if(type_msg == V_FLOW){
                    sensorFlow[i].status = 1; // update status
                    sensorFlow[i].lastUpdate[0] = getTahun();
                    sensorFlow[i].lastUpdate[1] = getBl();
                    sensorFlow[i].lastUpdate[2] = getTgl();
                    sensorFlow[i].lastUpdate[3] = getJam();
                    sensorFlow[i].lastUpdate[4] = getMenit();
                    sensorFlow[i].lastUpdate[5] = getDtk();
                    sensorFlow[i].timeOut = 0;
                    float vol = payload.toFloat()/sensorFlow[i].kalMin;
                    sensorFlow[i].val = vol;

#ifdef DEBUG_NODE
                    Serial.print(sensorFlow[i].nama);
                    Serial.print(" update val: ");
                    Serial.print(sensorFlow[i].val,2);
                    Serial.print(" waktu: ");
                    Serial.print(sensorFlow[i].lastUpdate[0]);
                    Serial.print('-');
                    Serial.print(sensorFlow[i].lastUpdate[1]);
                    Serial.print('-');
                    Serial.print(sensorFlow[i].lastUpdate[2]);
                    Serial.print(' ');
                    Serial.print(sensorFlow[i].lastUpdate[3]);
                    Serial.print(':');
                    Serial.print(sensorFlow[i].lastUpdate[4]);
                    Serial.print(':');
                    Serial.println(sensorFlow[i].lastUpdate[5]);
#endif
                }
            }
        }
    }
    // cek lengas
    for (int i = 0; i < 4; i++)
    {
        if ((sensorLengas[i].nodeId == nodeId) && (sensorLengas[i].childId == childId) && (type_msg == V_LEVEL))
        {
            sensorLengas[i].status = 1; // update status
            sensorLengas[i].lastUpdate[0] = getTahun();
            sensorLengas[i].lastUpdate[1] = getBl();
            sensorLengas[i].lastUpdate[2] = getTgl();
            sensorLengas[i].lastUpdate[3] = getJam();
            sensorLengas[i].lastUpdate[4] = getMenit();
            sensorLengas[i].lastUpdate[5] = getDtk();
            sensorLengas[i].timeOut = 0;
            sensorLengas[i].val = payload.toInt();

#ifdef DEBUG_NODE
            Serial.print(sensorLengas[i].nama);
            Serial.print(" update val: ");
            Serial.print(sensorLengas[i].val);
            Serial.print(" waktu: ");
            Serial.print(sensorLengas[i].lastUpdate[0]);
            Serial.print('-');
            Serial.print(sensorLengas[i].lastUpdate[1]);
            Serial.print('-');
            Serial.print(sensorLengas[i].lastUpdate[2]);
            Serial.print(' ');
            Serial.print(sensorLengas[i].lastUpdate[3]);
            Serial.print(':');
            Serial.print(sensorLengas[i].lastUpdate[4]);
            Serial.print(':');
            Serial.println(sensorLengas[i].lastUpdate[5]);
#endif
        }
    }

    // cek sensor temperature
    for (int i = 0; i < 4; i++)
    {
        if ((sensorTemperature[i].nodeId == nodeId) && (sensorTemperature[i].childId == childId) && (type_msg == V_TEMP))
        {
            sensorTemperature[i].status = 1; // update status
            sensorTemperature[i].lastUpdate[0] = getTahun();
            sensorTemperature[i].lastUpdate[1] = getBl();
            sensorTemperature[i].lastUpdate[2] = getTgl();
            sensorTemperature[i].lastUpdate[3] = getJam();
            sensorTemperature[i].lastUpdate[4] = getMenit();
            sensorTemperature[i].lastUpdate[5] = getDtk();
            sensorTemperature[i].timeOut = 0;
            sensorTemperature[i].val = payload.toFloat();

#ifdef DEBUG_NODE
            Serial.print(sensorTemperature[i].nama);
            Serial.print(" update val: ");
            Serial.print(sensorTemperature[i].val);
            Serial.print(" waktu: ");
            Serial.print(sensorTemperature[i].lastUpdate[0]);
            Serial.print('-');
            Serial.print(sensorTemperature[i].lastUpdate[1]);
            Serial.print('-');
            Serial.print(sensorTemperature[i].lastUpdate[2]);
            Serial.print(' ');
            Serial.print(sensorTemperature[i].lastUpdate[3]);
            Serial.print(':');
            Serial.print(sensorTemperature[i].lastUpdate[4]);
            Serial.print(':');
            Serial.println(sensorTemperature[i].lastUpdate[5]);
#endif
        }
    }

    // cek humidity
    for (int i = 0; i < 4; i++)
    {
        if ((sensorHumidity[i].nodeId == nodeId) && (sensorHumidity[i].childId == childId) && (type_msg == V_HUM))
        {
            sensorHumidity[i].status = 1; // update status
            sensorHumidity[i].lastUpdate[0] = getTahun();
            sensorHumidity[i].lastUpdate[1] = getBl();
            sensorHumidity[i].lastUpdate[2] = getTgl();
            sensorHumidity[i].lastUpdate[3] = getJam();
            sensorHumidity[i].lastUpdate[4] = getMenit();
            sensorHumidity[i].lastUpdate[5] = getDtk();
            sensorHumidity[i].timeOut = 0;
            sensorHumidity[i].val = payload.toFloat();

#ifdef DEBUG_NODE
            Serial.print(sensorHumidity[i].nama);
            Serial.print(" update val: ");
            Serial.print(sensorHumidity[i].val);
            Serial.print(" waktu: ");
            Serial.print(sensorHumidity[i].lastUpdate[0]);
            Serial.print('-');
            Serial.print(sensorHumidity[i].lastUpdate[1]);
            Serial.print('-');
            Serial.print(sensorHumidity[i].lastUpdate[2]);
            Serial.print(' ');
            Serial.print(sensorHumidity[i].lastUpdate[3]);
            Serial.print(':');
            Serial.print(sensorHumidity[i].lastUpdate[4]);
            Serial.print(':');
            Serial.println(sensorHumidity[i].lastUpdate[5]);
#endif
        }
    }

    // cek intermitent
    for (int i = 0; i < 4; i++)
    {
        if ((sensorLengasIntermittent[i].nodeId == nodeId) && (sensorLengasIntermittent[i].childId == childId) && (type_msg == V_DISTANCE))
        {
            sensorLengasIntermittent[i].status = 1; // update status
            sensorLengasIntermittent[i].lastUpdate[0] = getTahun();
            sensorLengasIntermittent[i].lastUpdate[1] = getBl();
            sensorLengasIntermittent[i].lastUpdate[2] = getTgl();
            sensorLengasIntermittent[i].lastUpdate[3] = getJam();
            sensorLengasIntermittent[i].lastUpdate[4] = getMenit();
            sensorLengasIntermittent[i].lastUpdate[5] = getDtk();
            sensorLengasIntermittent[i].timeOut = 0;
            int dist = payload.toInt();
            if(dist > 400){
                dist = 400;
            }
            if(dist < 100){
                dist = 100;
            }
            float distanceCM = (400 - dist)/10;
            sensorLengasIntermittent[i].val = distanceCM;

#ifdef DEBUG_NODE
            Serial.print(sensorLengasIntermittent[i].nama);
            Serial.print(" update val: ");
            Serial.print(sensorLengasIntermittent[i].val);
            Serial.print(" waktu: ");
            Serial.print(sensorLengasIntermittent[i].lastUpdate[0]);
            Serial.print('-');
            Serial.print(sensorLengasIntermittent[i].lastUpdate[1]);
            Serial.print('-');
            Serial.print(sensorLengasIntermittent[i].lastUpdate[2]);
            Serial.print(' ');
            Serial.print(sensorLengasIntermittent[i].lastUpdate[3]);
            Serial.print(':');
            Serial.print(sensorLengasIntermittent[i].lastUpdate[4]);
            Serial.print(':');
            Serial.println(sensorLengasIntermittent[i].lastUpdate[5]);
#endif
        }
    }
}

float getSensorValue(uint8_t sensor, uint8_t index)
{
    // sensor 1= temperature,2=humidity,3=lengas,4= intermitten,5 flowsensor
    uint8_t idx = index - 1;
    if (sensor == 1)
    {
        return sensorTemperature[idx].val;
    }
    else if (sensor == 2)
    {
        return sensorHumidity[idx].val;
    }
    else if (sensor == 3)
    {
        return sensorLengas[idx].val;
    }
    else if (sensor == 4)
    {
        return sensorLengasIntermittent[idx].val;
    }else if(sensor == 5){
        return sensorFlow[idx].val;
    }
    else
    {
        return 0;
    }
    /*
    switch(sensor){
        case 1:
            val = sensorTemperature[index].val;
        break;
        case 2:
            val = sensorHumidity[index].val;
        break;
        case 3:
            val = sensorLengas[index].val;
        break;
        case 4:
            val = sensorLengasIntermittent[index].val;
        break;

        case 5://genangan
            val = sensorLengasIntermittent[index].val;
        break;
        case 6://flow type
            val = sensorLengasIntermittent[index].val;
        break;

    }
    */
    // return val;
}
