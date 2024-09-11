// total memori device 840 byte
// total memori sensor 67x20 1340 byte
/*
    uint8_t nodeId;
    uint8_t sensorId;
    uint8_t ambangAtas = 80;
    uint8_t ambangBawah = 70;   //yg dipake
    uint8_t sensorUse = 1;      //yg dipake
    uint8_t value = 0;          //on/off
    uint8_t status = 0;         //device status
    uint8_t timer[24] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // TIMER FORMAT Jam,Menit,Durasi,hari(bentuk bit)
    uint8_t lastUpdate[6];//format yy,bb,tg,jm,mn,dt

struct sensorDev{
    uint8_t nodeId;
    uint8_t sensorId;
    uint8_t type;
    uint8_t devSts;//device status
    uint8_t interval;
    uint8_t batt;
    uint8_t value;
    unsigned char param[30];
    unsigned char dataSave[30];

    format pesan dari node
    >> sensor id sebagai trpe sensor

    msg child id
    0 | aktuator/relay
    1 | temp/suhu
    2 | humidity
    3 | CO2
    4 | CO
    5 | CH4
    6 | gas1
    7 | gas2
    8| gas3
    9| gas4
    10| gas5
    11| gas6
    12| pH tanah
    13| lengas
    14| genangan_atas
    15| genangan_bawah
    16| curah hujan
    17| angin
    18|
*/

// uint8_t deviceCount = 0;
// uint8_t sensorCount = 0;
// uint8_t lengasCount = 0;

#include <Arduino.h>
#include <Ticker.h>

#include <mySensorsHandle.h>
#include <nodeHandle.h>
#include <timeHandle.h>

#define ON 1
#define OFF 0

#define DEBUG_MYSENSOR
// sensor NODE

#define V_STATUS 2
#define V_VAR1 24
#define V_VAR2 25
#define V_VAR3 26
#define V_VAR4 27
#define V_TEMP 0
#define V_HUM 1
#define V_CURRENT 39
#define V_LEVEL 37
#define I_BATTERY_LEVEL 0

Ticker MySensorTimer;

extern Aktuator aktuator1;
extern Aktuator aktuator2;
extern Aktuator aktuator3;
extern Aktuator aktuator4;
extern Aktuator aktuator5;
extern Aktuator aktuator6;
extern Aktuator aktuator7;
extern Aktuator aktuator8;

extern Sensor sensor1; 
extern Sensor sensor2; 
extern Sensor sensor3; 
extern Sensor sensor4; 
extern Sensor sensor5; 
extern Sensor sensor6; 
extern Sensor sensor7;
extern Sensor sensor8; 
 

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

void kontrolAktuator(uint8_t node, uint8_t on_off)
{
    String strOut ="";
    String resp = "";
    switch(node){
        case 1:
            strOut += "1;1;";
        break;
        case 2:
            strOut += "1;2;";
        break;
        case 3:
            strOut += "2;1;";
        break;
        case 4:
            strOut += "2;2;";
        break;
        case 5:
            strOut += "3;1;";
        break;

        case 6:
            strOut += "3;2;";
        break;
        case 7:
            strOut += "4;1;";
        break;
        case 8:
            strOut += "4;2;";
        break;
    }
   
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
    // kirimKeApp(String type,uint8_t nomer,String cmd,String msg)
    // kirimKeApp("aktuator", nomerPompa, "status", resp);
}

void setSensorInterval(uint8_t n_id, uint8_t sensorType, uint8_t dtk)
{
    String msgOut = String(n_id);
    msgOut += ';';
    msgOut += String(sensorType);
    msgOut += ";1;0;24;";
    msgOut += String(dtk);
    msgOut += '\n';

    mqttMsgInput = msgOut;
    mqttMsgReady = true;

    Serial.print("Kirim ke sensor >> ");
    Serial.println(msgOut);
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
#ifdef DEBUG_MYSENSOR
    Serial.print(msg);
    Serial.print("nodeId: ");
    Serial.print(nodeId);
    Serial.print(" chidId: ");
    Serial.print(childId);
    Serial.print(" Type: ");
    Serial.print(type_msg);
    Serial.print(" payload:");
    Serial.println(payload);
#endif

    if ((nodeId == aktuator1.nodeId) && (childId == aktuator1.childId))
    {
        // update aktuator1
        // last update
        aktuator1.lastUpdate[0] = getTahun();
        aktuator1.lastUpdate[1] = getBl();
        aktuator1.lastUpdate[2] = getTgl();
        aktuator1.lastUpdate[3] = getJam();
        aktuator1.lastUpdate[4] = getMenit();
        aktuator1.lastUpdate[5] = getDtk();

        aktuator1.status = 1;
        aktuator1.timeOut = 0;

        if (type_msg == V_STATUS)
        {
            if (payload == "1")
            {
                aktuator1.val = 1;
            }
            else
            {
                aktuator1.val = 0;
            }
        }
        Serial.print("Aktuator 1 update: ");
        Serial.println(payload);
    }
    else if ((nodeId == aktuator2.nodeId) && (childId == aktuator2.childId))
    {
        // update aktuator2
        // last update
        aktuator2.lastUpdate[0] = getTahun();
        aktuator2.lastUpdate[1] = getBl();
        aktuator2.lastUpdate[2] = getTgl();
        aktuator2.lastUpdate[3] = getJam();
        aktuator2.lastUpdate[4] = getMenit();
        aktuator2.lastUpdate[5] = getDtk();

        aktuator2.status = 1;
        aktuator2.timeOut = 0;

        if (type_msg == V_STATUS)
        {
            if (payload == "1")
            {
                aktuator2.val = 1;
            }
            else
            {
                aktuator2.val = 0;
            }
        }
        Serial.print("Aktuator 2 update: ");
        Serial.println(payload);
    }
    else if ((nodeId == aktuator3.nodeId) && (childId == aktuator3.childId))
    {
        // update aktuator3
        // last update
        aktuator3.lastUpdate[0] = getTahun();
        aktuator3.lastUpdate[1] = getBl();
        aktuator3.lastUpdate[2] = getTgl();
        aktuator3.lastUpdate[3] = getJam();
        aktuator3.lastUpdate[4] = getMenit();
        aktuator3.lastUpdate[5] = getDtk();

        aktuator3.status = 1;
        aktuator3.timeOut = 0;

        if (type_msg == V_STATUS)
        {
            if (payload == "1")
            {
                aktuator3.val = 1;
            }
            else
            {
                aktuator3.val = 0;
            }
        }
        Serial.print("Aktuator 3 update: ");
        Serial.println(payload);
    }
    else if ((nodeId == aktuator4.nodeId) && (childId == aktuator4.childId))
    {
        // update aktuator4
        // last update
        aktuator4.lastUpdate[0] = getTahun();
        aktuator4.lastUpdate[1] = getBl();
        aktuator4.lastUpdate[2] = getTgl();
        aktuator4.lastUpdate[3] = getJam();
        aktuator4.lastUpdate[4] = getMenit();
        aktuator4.lastUpdate[5] = getDtk();

        aktuator4.status = 1;
        aktuator4.timeOut = 0;

        if (type_msg == V_STATUS)
        {
            if (payload == "1")
            {
                aktuator4.val = 1;
            }
            else
            {
                aktuator4.val = 0;
            }
        }
        Serial.print("Aktuator 4 update: ");
        Serial.println(payload);
    }
    else if ((nodeId == aktuator5.nodeId) && (childId == aktuator5.childId))
    {
        // update aktuator5
        // last update
        aktuator5.lastUpdate[0] = getTahun();
        aktuator5.lastUpdate[1] = getBl();
        aktuator5.lastUpdate[2] = getTgl();
        aktuator5.lastUpdate[3] = getJam();
        aktuator5.lastUpdate[4] = getMenit();
        aktuator5.lastUpdate[5] = getDtk();

        aktuator5.status = 1;
        aktuator5.timeOut = 0;

        if (type_msg == V_STATUS)
        {
            if (payload == "1")
            {
                aktuator5.val = 1;
            }
            else
            {
                aktuator5.val = 0;
            }
        }
        Serial.print("Aktuator 5 update: ");
        Serial.println(payload);
    }
    else if ((nodeId == aktuator6.nodeId) && (childId == aktuator6.childId))
    {
        // update aktuator6
        // last update
        aktuator6.lastUpdate[0] = getTahun();
        aktuator6.lastUpdate[1] = getBl();
        aktuator6.lastUpdate[2] = getTgl();
        aktuator6.lastUpdate[3] = getJam();
        aktuator6.lastUpdate[4] = getMenit();
        aktuator6.lastUpdate[5] = getDtk();

        aktuator6.status = 1;
        aktuator6.timeOut = 0;

        if (type_msg == V_STATUS)
        {
            if (payload == "1")
            {
                aktuator6.val = 1;
            }
            else
            {
                aktuator6.val = 0;
            }
        }
        Serial.print("Aktuator 6 update: ");
        Serial.println(payload);
    }
    else if ((nodeId == aktuator7.nodeId) && (childId == aktuator7.childId))
    {
        // update aktuator7
        // last update
        aktuator7.lastUpdate[0] = getTahun();
        aktuator7.lastUpdate[1] = getBl();
        aktuator7.lastUpdate[2] = getTgl();
        aktuator7.lastUpdate[3] = getJam();
        aktuator7.lastUpdate[4] = getMenit();
        aktuator7.lastUpdate[5] = getDtk();

        aktuator7.status = 1;
        aktuator7.timeOut = 0;

        if (type_msg == V_STATUS)
        {
            if (payload == "1")
            {
                aktuator7.val = 1;
            }
            else
            {
                aktuator7.val = 0;
            }
        }
        Serial.print("Aktuator 7 update: ");
        Serial.println(payload);
    }
    else if ((nodeId == aktuator8.nodeId) && (childId == aktuator8.childId))
    {
        // update aktuator8
        // last update
        aktuator8.lastUpdate[0] = getTahun();
        aktuator8.lastUpdate[1] = getBl();
        aktuator8.lastUpdate[2] = getTgl();
        aktuator8.lastUpdate[3] = getJam();
        aktuator8.lastUpdate[4] = getMenit();
        aktuator8.lastUpdate[5] = getDtk();

        aktuator8.status = 1;
        aktuator8.timeOut = 0;

        if (type_msg == V_STATUS)
        {
            if (payload == "1")
            {
                aktuator8.val = 1;
            }
            else
            {
                aktuator8.val = 0;
            }
        }
        Serial.print("Aktuator 8 update: ");
        Serial.println(payload);
    }
    else if (nodeId == sensor1.nodeId)
    {
        // update lengas 1
        // last update
        sensor1.lastUpdate[0] = getTahun();
        sensor1.lastUpdate[1] = getBl();
        sensor1.lastUpdate[2] = getTgl();
        sensor1.lastUpdate[3] = getJam();
        sensor1.lastUpdate[4] = getMenit();
        sensor1.lastUpdate[5] = getDtk();

        sensor1.status = 1;
        // sensor1.timeOut = 0;
        if (childId == sensor1.childId1)
        {
            if (type_msg == V_LEVEL)
            {
                //lengas
                sensor1.val1 = payload.toInt();
            }
            else if (type_msg == V_VAR1)
            {
                sensor1.interval = payload.toInt();
            }
        }
        else if (childId == 255)
        {
            if (type_msg == I_BATTERY_LEVEL)
            {
                sensor1.battLevel = payload.toInt();
            }
        }
        else if (childId == sensor1.childId2)
        {
            // LOW triger
          
                sensor1.val2 = payload.toInt();
            
        }
        else if (childId == sensor1.childId3)
        {
            // HIGH triger
           sensor1.val3 = payload.toInt();
        }
    }
    else if (nodeId == sensor2.nodeId)
    {
        // update lengas 2
        // last update
        sensor2.lastUpdate[0] = getTahun();
        sensor2.lastUpdate[1] = getBl();
        sensor2.lastUpdate[2] = getTgl();
        sensor2.lastUpdate[3] = getJam();
        sensor2.lastUpdate[4] = getMenit();
        sensor2.lastUpdate[5] = getDtk();

        sensor2.status = 1;
        // sensor2.timeOut = 0;
        if (childId == sensor2.childId1)
        {
            if (type_msg == V_LEVEL)
            {
                //lengas
                sensor2.val1 = payload.toInt();
            }
            else if (type_msg == V_VAR1)
            {
                sensor2.interval = payload.toInt();
            }
        }
        else if (childId == 255)
        {
            if (type_msg == I_BATTERY_LEVEL)
            {
                sensor2.battLevel = payload.toInt();
            }
        }
        else if (childId == sensor2.childId2)
        {
            // LOW triger
            sensor2.val2 = payload.toInt();
        }
        else if (childId == sensor2.childId3)
        {
            // HIGH triger
           sensor2.val3 = payload.toInt();
        }
    }else if (nodeId == sensor3.nodeId)
    {
        // update lengas 1
        // last update
        sensor3.lastUpdate[0] = getTahun();
        sensor3.lastUpdate[1] = getBl();
        sensor3.lastUpdate[2] = getTgl();
        sensor3.lastUpdate[3] = getJam();
        sensor3.lastUpdate[4] = getMenit();
        sensor3.lastUpdate[5] = getDtk();

        sensor3.status = 1;
        // sensor3.timeOut = 0;
        if (childId == sensor3.childId1)
        {
            if (type_msg == V_LEVEL)
            {
                //lengas
                sensor3.val1 = payload.toInt();
            }
            else if (type_msg == V_VAR1)
            {
                sensor3.interval = payload.toInt();
            }
        }
        else if (childId == 255)
        {
            if (type_msg == I_BATTERY_LEVEL)
            {
                sensor3.battLevel = payload.toInt();
            }
        }
        else if (childId == sensor3.childId2)
        {
            // LOW triger
          
                sensor3.val2 = payload.toInt();
            
        }
        else if (childId == sensor3.childId3)
        {
            // HIGH triger
           sensor3.val3 = payload.toInt();
        }
    }
    else if (nodeId == sensor4.nodeId)
    {
        // update lengas 1
        // last update
        sensor4.lastUpdate[0] = getTahun();
        sensor4.lastUpdate[1] = getBl();
        sensor4.lastUpdate[2] = getTgl();
        sensor4.lastUpdate[3] = getJam();
        sensor4.lastUpdate[4] = getMenit();
        sensor4.lastUpdate[5] = getDtk();

        sensor4.status = 1;
        // sensor4.timeOut = 0;
        if (childId == sensor4.childId1)
        {
            if (type_msg == V_LEVEL)
            {
                //lengas
                sensor4.val1 = payload.toInt();
            }
            else if (type_msg == V_VAR1)
            {
                sensor4.interval = payload.toInt();
            }
        }
        else if (childId == 255)
        {
            if (type_msg == I_BATTERY_LEVEL)
            {
                sensor4.battLevel = payload.toInt();
            }
        }
        else if (childId == sensor4.childId2)
        {
            // LOW triger
          
                sensor4.val2 = payload.toInt();
            
        }
        else if (childId == sensor4.childId3)
        {
            // HIGH triger
           sensor4.val3 = payload.toInt();
        }
    }
    
    
    else if (nodeId == sensor5.nodeId)
    {
        // update sensor dht22
        // last update
        sensor5.lastUpdate[0] = getTahun();
        sensor5.lastUpdate[1] = getBl();
        sensor5.lastUpdate[2] = getTgl();
        sensor5.lastUpdate[3] = getJam();
        sensor5.lastUpdate[4] = getMenit();
        sensor5.lastUpdate[5] = getDtk();
        sensor5.status = 1;
        // sensor5.timeOut = 0;
        if (childId == sensor5.childId1)
        {
            if (type_msg == V_TEMP)
            {
                sensor5.val1 = payload.toInt();
            }
            else if (type_msg == V_VAR1)
            {
                sensor5.interval = payload.toInt();
            }else if (type_msg == V_HUM)
            {
                sensor5.val2 = payload.toInt();
            }
        }
        else if (childId == 255)//internal report 
        {
            if (type_msg == I_BATTERY_LEVEL)
            {
                sensor5.battLevel = payload.toInt();
            }
        }
        
    }
}
