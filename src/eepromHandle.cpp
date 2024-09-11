#include <Arduino.h>
#include <EEPROM.h>
#include <nodeHandle.h>

// allokasi 25 byte aktuator dan 20 byte untuk sensor
#define EE_NODE1 1000
#define EE_NODE2 1030
#define EE_NODE3 1060
#define EE_NODE4 1090
#define EE_NODE5 1120
#define EE_NODE6 1150
#define EE_NODE7 1180
#define EE_NODE8 1210

#define EE_SENSOR1 1500
#define EE_SENSOR2 1530
#define EE_SENSOR3 1560

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

uint8_t data[30];
unsigned int addr = 1000;
uint8_t i = 0;
int kal = 0;

void simpanNode(uint8_t node)
{
    switch (node)
    {
    case 1:
        data[0] = aktuator1.nodeId;
        data[1] = aktuator1.childId;
        data[2] = aktuator1.status;
        data[3] = aktuator1.val;
        data[4] = aktuator1.timeOut;
        data[5] = aktuator1.sensorUseId;
        data[6] = aktuator1.sensorUseChildId;
        data[7] = aktuator1.triggerMin;
        data[8] = aktuator1.triggerMax;
        // jadwal
        for (i = 0; i < 10; i++)
        {
            data[9 + i] = aktuator1.jadwal[i];
        }
        // last update
        for (i = 0; i < 6; i++)
        {
            data[19 + i] = aktuator1.lastUpdate[i];
        }

        break;

    case 2:
        data[0] = aktuator2.nodeId;
        data[1] = aktuator2.childId;
        data[2] = aktuator2.status;
        data[3] = aktuator2.val;
        data[4] = aktuator2.timeOut;
        data[5] = aktuator2.sensorUseId;
        data[6] = aktuator2.sensorUseChildId;
        data[7] = aktuator2.triggerMin;
        data[8] = aktuator2.triggerMax;
        // jadwal
        for (i = 0; i < 10; i++)
        {
            data[9 + i] = aktuator2.jadwal[i];
        }
        // last update
        for (i = 0; i < 6; i++)
        {
            data[19 + i] = aktuator2.lastUpdate[i];
        }

        break;

    case 3:
        data[0] = aktuator3.nodeId;
        data[1] = aktuator3.childId;
        data[2] = aktuator3.status;
        data[3] = aktuator3.val;
        data[4] = aktuator3.timeOut;
        data[5] = aktuator3.sensorUseId;
        data[6] = aktuator3.sensorUseChildId;
        data[7] = aktuator3.triggerMin;
        data[8] = aktuator3.triggerMax;
        // jadwal
        for (i = 0; i < 10; i++)
        {
            data[9 + i] = aktuator3.jadwal[i];
        }
        // last update
        for (i = 0; i < 6; i++)
        {
            data[19 + i] = aktuator3.lastUpdate[i];
        }

        break;

    case 4:
        data[0] = aktuator4.nodeId;
        data[1] = aktuator4.childId;
        data[2] = aktuator4.status;
        data[3] = aktuator4.val;
        data[4] = aktuator4.timeOut;
        data[5] = aktuator4.sensorUseId;
        data[6] = aktuator4.sensorUseChildId;
        data[7] = aktuator4.triggerMin;
        data[8] = aktuator4.triggerMax;
        // jadwal
        for (i = 0; i < 10; i++)
        {
            data[9 + i] = aktuator4.jadwal[i];
        }
        // last update
        for (i = 0; i < 6; i++)
        {
            data[19 + i] = aktuator4.lastUpdate[i];
        }

        break;

    case 5:
        data[0] = aktuator5.nodeId;
        data[1] = aktuator5.childId;
        data[2] = aktuator5.status;
        data[3] = aktuator5.val;
        data[4] = aktuator5.timeOut;
        data[5] = aktuator5.sensorUseId;
        data[6] = aktuator5.sensorUseChildId;
        data[7] = aktuator5.triggerMin;
        data[8] = aktuator5.triggerMax;
        // jadwal
        for (i = 0; i < 10; i++)
        {
            data[9 + i] = aktuator5.jadwal[i];
        }
        // last update
        for (i = 0; i < 6; i++)
        {
            data[19 + i] = aktuator5.lastUpdate[i];
        }

        break;

    case 6:
        data[0] = aktuator6.nodeId;
        data[1] = aktuator6.childId;
        data[2] = aktuator6.status;
        data[3] = aktuator6.val;
        data[4] = aktuator6.timeOut;
        data[5] = aktuator6.sensorUseId;
        data[6] = aktuator6.sensorUseChildId;
        data[7] = aktuator6.triggerMin;
        data[8] = aktuator6.triggerMax;
        // jadwal
        for (i = 0; i < 10; i++)
        {
            data[9 + i] = aktuator6.jadwal[i];
        }
        // last update
        for (i = 0; i < 6; i++)
        {
            data[19 + i] = aktuator6.lastUpdate[i];
        }

        break;

    case 7:
        data[0] = aktuator7.nodeId;
        data[1] = aktuator7.childId;
        data[2] = aktuator7.status;
        data[3] = aktuator7.val;
        data[4] = aktuator7.timeOut;
        data[5] = aktuator7.sensorUseId;
        data[6] = aktuator7.sensorUseChildId;
        data[7] = aktuator7.triggerMin;
        data[8] = aktuator7.triggerMax;
        // jadwal
        for (i = 0; i < 10; i++)
        {
            data[9 + i] = aktuator7.jadwal[i];
        }
        // last update
        for (i = 0; i < 6; i++)
        {
            data[19 + i] = aktuator7.lastUpdate[i];
        }

        break;

    case 8:
        data[0] = aktuator8.nodeId;
        data[1] = aktuator8.childId;
        data[2] = aktuator8.status;
        data[3] = aktuator8.val;
        data[4] = aktuator8.timeOut;
        data[5] = aktuator8.sensorUseId;
        data[6] = aktuator8.sensorUseChildId;
        data[7] = aktuator8.triggerMin;
        data[8] = aktuator8.triggerMax;
        // jadwal
        for (i = 0; i < 10; i++)
        {
            data[9 + i] = aktuator8.jadwal[i];
        }
        // last update
        for (i = 0; i < 6; i++)
        {
            data[19 + i] = aktuator8.lastUpdate[i];
        }

        break;
    }
    addr = ((node - 1) * 30) + 1000;
    for (i = 0; i < 25; i++)
    {
        EEPROM.write(addr + i, data[i]);
    }
    EEPROM.commit();
    Serial.print("Simpan ke EEPROM Node ");
    Serial.println(node);
}

void loadNode(uint8_t node)
{
    addr = ((node - 1) * 30) + 1000;
    for (int i = 0; i < 30; i++)
    {
        data[i] = EEPROM.read(addr + i);
    }

    if (data[0] > 8)
    {
        Serial.println("Simpan Node default");
        // isi default eeprom
        aktuator1.nodeId = 1;
        aktuator1.childId = 1;
        simpanNode(1);

        aktuator2.nodeId = 1;
        aktuator2.childId = 2;
        simpanNode(2);

        aktuator3.nodeId = 2;
        aktuator3.childId = 1;
        simpanNode(3);

        aktuator4.nodeId = 2;
        aktuator4.childId = 2;
        simpanNode(4);

        aktuator5.nodeId = 3;
        aktuator5.childId = 1;
        simpanNode(5);

        aktuator6.nodeId = 3;
        aktuator6.childId = 2;
        simpanNode(6);

        aktuator7.nodeId = 4;
        aktuator7.childId = 1;
        simpanNode(7);

        aktuator8.nodeId = 4;
        aktuator8.childId = 2;
        simpanNode(8);
    }
    else
    {
        switch (node)
        {
        case 1:
            aktuator1.nodeId = data[0];
            aktuator1.childId = data[1];
            aktuator1.status = data[2];
            aktuator1.val = data[3];
            aktuator1.timeOut = data[4];
            aktuator1.sensorUseId = data[5];
            aktuator1.sensorUseChildId = data[6];
            aktuator1.triggerMin = data[7];
            aktuator1.triggerMax = data[8];
            // jadwal
            for (i = 0; i < 15; i++)
            {
                aktuator1.jadwal[i] = data[9 + i];
            }
            // last update
            for (i = 0; i < 6; i++)
            {
                aktuator1.lastUpdate[i] = data[24 + i];
            }

            break;

        case 2:
            aktuator2.nodeId = data[0];
            aktuator2.childId = data[1];
            aktuator2.status = data[2];
            aktuator2.val = data[3];
            aktuator2.timeOut = data[4];
            aktuator2.sensorUseId = data[5];
            aktuator2.sensorUseChildId = data[6];
            aktuator2.triggerMin = data[7];
            aktuator2.triggerMax = data[8];
            // jadwal
            for (i = 0; i < 15; i++)
            {
                aktuator2.jadwal[i] = data[9 + i];
            }
            // last update
            for (i = 0; i < 6; i++)
            {
                aktuator2.lastUpdate[i] = data[24 + i];
            }

            break;

        case 3:
            aktuator3.nodeId = data[0];
            aktuator3.childId = data[1];
            aktuator3.status = data[2];
            aktuator3.val = data[3];
            aktuator3.timeOut = data[4];
            aktuator3.sensorUseId = data[5];
            aktuator3.sensorUseChildId = data[6];
            aktuator3.triggerMin = data[7];
            aktuator3.triggerMax = data[8];
            // jadwal
            for (i = 0; i < 15; i++)
            {
                aktuator3.jadwal[i] = data[9 + i];
            }
            // last update
            for (i = 0; i < 6; i++)
            {
                aktuator3.lastUpdate[i] = data[24 + i];
            }

            break;

        case 4:
            aktuator4.nodeId = data[0];
            aktuator4.childId = data[1];
            aktuator4.status = data[2];
            aktuator4.val = data[3];
            aktuator4.timeOut = data[4];
            aktuator4.sensorUseId = data[5];
            aktuator4.sensorUseChildId = data[6];
            aktuator4.triggerMin = data[7];
            aktuator4.triggerMax = data[8];
            // jadwal
            for (i = 0; i < 15; i++)
            {
                aktuator4.jadwal[i] = data[9 + i];
            }
            // last update
            for (i = 0; i < 6; i++)
            {
                aktuator4.lastUpdate[i] = data[24 + i];
            }

            break;

        case 5:
            aktuator5.nodeId = data[0];
            aktuator5.childId = data[1];
            aktuator5.status = data[2];
            aktuator5.val = data[3];
            aktuator5.timeOut = data[4];
            aktuator5.sensorUseId = data[5];
            aktuator5.sensorUseChildId = data[6];
            aktuator5.triggerMin = data[7];
            aktuator5.triggerMax = data[8];
            // jadwal
            for (i = 0; i < 15; i++)
            {
                aktuator5.jadwal[i] = data[9 + i];
            }
            // last update
            for (i = 0; i < 6; i++)
            {
                aktuator5.lastUpdate[i] = data[24 + i];
            }

            break;

        case 6:
            aktuator6.nodeId = data[0];
            aktuator6.childId = data[1];
            aktuator6.status = data[2];
            aktuator6.val = data[3];
            aktuator6.timeOut = data[4];
            aktuator6.sensorUseId = data[5];
            aktuator6.sensorUseChildId = data[6];
            aktuator6.triggerMin = data[7];
            aktuator6.triggerMax = data[8];
            // jadwal
            for (i = 0; i < 15; i++)
            {
                aktuator6.jadwal[i] = data[9 + i];
            }
            // last update
            for (i = 0; i < 6; i++)
            {
                aktuator6.lastUpdate[i] = data[24 + i];
            }

            break;

        case 7:
            aktuator7.nodeId = data[0];
            aktuator7.childId = data[1];
            aktuator7.status = data[2];
            aktuator7.val = data[3];
            aktuator7.timeOut = data[4];
            aktuator7.sensorUseId = data[5];
            aktuator7.sensorUseChildId = data[6];
            aktuator7.triggerMin = data[7];
            aktuator7.triggerMax = data[8];
            // jadwal
            for (i = 0; i < 15; i++)
            {
                aktuator7.jadwal[i] = data[9 + i];
            }
            // last update
            for (i = 0; i < 6; i++)
            {
                aktuator7.lastUpdate[i] = data[24 + i];
            }

            break;

        case 8:
            aktuator8.nodeId = data[0];
            aktuator8.childId = data[1];
            aktuator8.status = data[2];
            aktuator8.val = data[3];
            aktuator8.timeOut = data[4];
            aktuator8.sensorUseId = data[5];
            aktuator8.sensorUseChildId = data[6];
            aktuator8.triggerMin = data[7];
            aktuator8.triggerMax = data[8];
            // jadwal
            for (i = 0; i < 15; i++)
            {
                aktuator8.jadwal[i] = data[9 + i];
            }
            // last update
            for (i = 0; i < 6; i++)
            {
                aktuator8.lastUpdate[i] = data[24 + i];
            }

            break;
        }
    }

    Serial.print("--------------------------\nLoad node ");
    Serial.println(node);

    Serial.print("NodeId: ");
    Serial.println(data[0]);
    Serial.print("ChildId: ");
    Serial.println(data[1]);
    Serial.print("Status: ");
    Serial.println(data[2]);
    Serial.print("Val: ");
    Serial.println(data[3]);
    Serial.print("SensorUseId: ");
    Serial.println(data[5]);
    Serial.print("SensorUseChildId: ");
    Serial.println(data[6]);
    Serial.print("TrigerMin: ");
    Serial.println(data[7]);
    Serial.print("TrigerMax: ");
    Serial.println(data[8]);
    Serial.print("Jadwal: ");
    Serial.print(data[9]);
    Serial.print(":");
    Serial.print(data[10]);
    Serial.print("-");
    Serial.print(data[11]);
    Serial.print("\t");
    Serial.print(data[12]);
    Serial.print(":");
    Serial.print(data[13]);
    Serial.print("-");
    Serial.print(data[14]);
    Serial.print("\t");
    Serial.print(data[15]);
    Serial.print(":");
    Serial.print(data[16]);
    Serial.print("-");
    Serial.print(data[17]);
    Serial.print("\t");
    Serial.print(data[18]);
    Serial.print(":");
    Serial.print(data[19]);
    Serial.print("-");
    Serial.print(data[20]);
    Serial.print("\t");
    Serial.print(data[21]);
    Serial.print(":");
    Serial.print(data[22]);
    Serial.print("-");
    Serial.print(data[23]);
    Serial.print("\t");
    Serial.print("LastUpdate 20");
    Serial.print(data[24]);
    Serial.print("/");
    Serial.print(data[25]);
    Serial.print("/");
    Serial.print(data[26]);
    Serial.print(" ");
    Serial.print(data[27]);
    Serial.print(":");
    Serial.print(data[28]);
    Serial.print(":");
    Serial.println(data[29]);
}

void simpanSensor(uint8_t sensor)
{
    switch (sensor)
    {
    case 1:
        data[0] = sensor1.nodeId;
        data[1] = sensor1.childId1;
        data[2] = sensor1.childId2;
        data[3] = sensor1.childId3;
        data[4] = sensor1.interval;
        data[5] = sensor1.status;
        data[6] = sensor1.battLevel;
        data[7] = sensor1.val1;
        data[8] = sensor1.val2;
        data[9] = sensor1.val3;

        data[10] = sensor1.kalMin >> 8;
        data[11] = sensor1.kalMin & 0x00FF;
        data[12] = sensor1.kalMax >> 8;
        data[13] = sensor1.kalMax & 0x00FF;

        // last update
        for (i = 0; i < 6; i++)
        {
            data[14 + i] = sensor1.lastUpdate[i];
        }

        break;

        case 2:
        data[0] = sensor2.nodeId;
        data[1] = sensor2.childId1;
        data[2] = sensor2.childId2;
        data[3] = sensor2.childId3;
        data[4] = sensor2.interval;
        data[5] = sensor2.status;
        data[6] = sensor2.battLevel;
        data[7] = sensor2.val1;
        data[8] = sensor2.val2;
        data[9] = sensor2.val3;

        data[10] = sensor2.kalMin >> 8;
        data[11] = sensor2.kalMin & 0x00FF;
        data[12] = sensor2.kalMax >> 8;
        data[13] = sensor2.kalMax & 0x00FF;

        // last update
        for (i = 0; i < 6; i++)
        {
            data[14 + i] = sensor2.lastUpdate[i];
        }

        break;
        case 3:
        data[0] = sensor3.nodeId;
        data[1] = sensor3.childId1;
        data[2] = sensor3.childId2;
        data[3] = sensor3.childId3;
        data[4] = sensor3.interval;
        data[5] = sensor3.status;
        data[6] = sensor3.battLevel;
        data[7] = sensor3.val1;
        data[8] = sensor3.val2;
        data[9] = sensor3.val3;

        data[10] = sensor3.kalMin >> 8;
        data[11] = sensor3.kalMin & 0x00FF;
        data[12] = sensor3.kalMax >> 8;
        data[13] = sensor3.kalMax & 0x00FF;

        // last update
        for (i = 0; i < 6; i++)
        {
            data[14 + i] = sensor3.lastUpdate[i];
        }

        break;
        case 4:
        data[0] = sensor4.nodeId;
        data[1] = sensor4.childId1;
        data[2] = sensor4.childId2;
        data[3] = sensor4.childId3;
        data[4] = sensor4.interval;
        data[5] = sensor4.status;
        data[6] = sensor4.battLevel;
        data[7] = sensor4.val1;
        data[8] = sensor4.val2;
        data[9] = sensor4.val3;

        data[10] = sensor4.kalMin >> 8;
        data[11] = sensor4.kalMin & 0x00FF;
        data[12] = sensor4.kalMax >> 8;
        data[13] = sensor4.kalMax & 0x00FF;

        // last update
        for (i = 0; i < 6; i++)
        {
            data[14 + i] = sensor4.lastUpdate[i];
        }

        break;
        case 5:
        data[0] = sensor5.nodeId;
        data[1] = sensor5.childId1;
        data[2] = sensor5.childId2;
        data[3] = sensor5.childId3;
        data[4] = sensor5.interval;
        data[5] = sensor5.status;
        data[6] = sensor5.battLevel;
        data[7] = sensor5.val1;
        data[8] = sensor5.val2;
        data[9] = sensor5.val3;

        data[10] = sensor5.kalMin >> 8;
        data[11] = sensor5.kalMin & 0x00FF;
        data[12] = sensor5.kalMax >> 8;
        data[13] = sensor5.kalMax & 0x00FF;

        // last update
        for (i = 0; i < 6; i++)
        {
            data[14 + i] = sensor5.lastUpdate[i];
        }

        break;
        case 6:
        data[0] = sensor6.nodeId;
        data[1] = sensor6.childId1;
        data[2] = sensor6.childId2;
        data[3] = sensor6.childId3;
        data[4] = sensor6.interval;
        data[5] = sensor6.status;
        data[6] = sensor6.battLevel;
        data[7] = sensor6.val1;
        data[8] = sensor6.val2;
        data[9] = sensor6.val3;

        data[10] = sensor6.kalMin >> 8;
        data[11] = sensor6.kalMin & 0x00FF;
        data[12] = sensor6.kalMax >> 8;
        data[13] = sensor6.kalMax & 0x00FF;

        // last update
        for (i = 0; i < 6; i++)
        {
            data[14 + i] = sensor6.lastUpdate[i];
        }

        break;
        case 7:
        data[0] = sensor7.nodeId;
        data[1] = sensor7.childId1;
        data[2] = sensor7.childId2;
        data[3] = sensor7.childId3;
        data[4] = sensor7.interval;
        data[5] = sensor7.status;
        data[6] = sensor7.battLevel;
        data[7] = sensor7.val1;
        data[8] = sensor7.val2;
        data[9] = sensor7.val3;

        data[10] = sensor7.kalMin >> 8;
        data[11] = sensor7.kalMin & 0x00FF;
        data[12] = sensor7.kalMax >> 8;
        data[13] = sensor7.kalMax & 0x00FF;

        // last update
        for (i = 0; i < 6; i++)
        {
            data[14 + i] = sensor7.lastUpdate[i];
        }

        break;
        case 8:
        data[0] = sensor8.nodeId;
        data[1] = sensor8.childId1;
        data[2] = sensor8.childId2;
        data[3] = sensor8.childId3;
        data[4] = sensor8.interval;
        data[5] = sensor8.status;
        data[6] = sensor8.battLevel;
        data[7] = sensor8.val1;
        data[8] = sensor8.val2;
        data[9] = sensor8.val3;

        data[10] = sensor8.kalMin >> 8;
        data[11] = sensor8.kalMin & 0x00FF;
        data[12] = sensor8.kalMax >> 8;
        data[13] = sensor8.kalMax & 0x00FF;

        // last update
        for (i = 0; i < 6; i++)
        {
            data[14 + i] = sensor8.lastUpdate[i];
        }

        break;
    }
}

void loadSensor(uint8_t sensor)
{
    addr = ((sensor - 1) * 30) + 1500;
    for (int i = 0; i < 20; i++)
    {
        data[i] = EEPROM.read(addr + i);
    }

    if (data[0] > 65)
    {
        Serial.println("Simpan Sensor default");
        // isi default eeprom
        // lengas 1
        sensor1.nodeId = 51;
        simpanSensor(1);

        // lengas 2
        sensor2.nodeId = 52;
        simpanSensor(2);

        // lengas 3
        sensor3.nodeId = 53;
        simpanSensor(3);

        // lengas 4
        sensor4.nodeId = 54;
        simpanSensor(4);

        // sht22
        sensor5.nodeId = 60;
        sensor5.childId1 = 0;
        sensor5.childId2 = 1;
        sensor5.childId3 = 3;
        simpanSensor(5);

        // sht22
        sensor6.nodeId = 61;
        sensor6.childId1 = 0;
        sensor6.childId2 = 1;
        sensor6.childId3 = 3;
        simpanSensor(6);

        // npk
        sensor7.nodeId = 62;
        sensor7.childId1 = 0;
        sensor7.childId2 = 1;
        sensor7.childId3 = 3;
        simpanSensor(7);

        //  flow sensor
        sensor8.nodeId = 63;
        sensor8.childId1 = 0;
        sensor8.childId2 = 1;
        sensor8.childId3 = 3;
        simpanSensor(8);
    }
    else
    {
        switch (sensor)
        {
        case 1:
            sensor1.nodeId = data[0];
            sensor1.childId1 = data[1];
            sensor1.childId2 = data[2];
            sensor1.childId3 = data[3];
            sensor1.interval = data[4];
            sensor1.status = data[5];
            sensor1.battLevel = data[6];
            sensor1.val1 = data[7];
            sensor1.val2 = data[8];
            sensor1.val3 = data[9];
            kal = (data[10] * 256) + data[11];
            sensor1.kalMin = kal;
            kal = (data[12] * 256) + data[13];
            sensor1.kalMax = kal;
            // last update
            for (i = 0; i < 6; i++)
            {
                sensor1.lastUpdate[i] = data[14 + i];
            }

            break;

        case 2:
            sensor2.nodeId = data[0];
            sensor2.childId1 = data[1];
            sensor2.childId2 = data[2];
            sensor2.childId3 = data[3];
            sensor2.interval = data[4];
            sensor2.status = data[5];
            sensor2.battLevel = data[6];
            sensor2.val1 = data[7];
            sensor2.val2 = data[8];
            sensor2.val3 = data[9];
            kal = (data[10] * 256) + data[11];
            sensor2.kalMin = kal;
            kal = (data[12] * 256) + data[13];
            sensor2.kalMax = kal;
            // last update
            for (i = 0; i < 6; i++)
            {
                sensor2.lastUpdate[i] = data[14 + i];
            }

            break;
        case 3:
            sensor3.nodeId = data[0];
            sensor3.childId1 = data[1];
            sensor3.childId2 = data[2];
            sensor3.childId3 = data[3];
            sensor3.interval = data[4];
            sensor3.status = data[5];
            sensor3.battLevel = data[6];
            sensor3.val1 = data[7];
            sensor3.val2 = data[8];
            sensor3.val3 = data[9];
            kal = (data[10] * 256) + data[11];
            sensor3.kalMin = kal;
            kal = (data[12] * 256) + data[13];
            sensor3.kalMax = kal;
            // last update
            for (i = 0; i < 6; i++)
            {
                sensor3.lastUpdate[i] = data[14 + i];
            }

            break;
        case 4:
            sensor4.nodeId = data[0];
            sensor4.childId1 = data[1];
            sensor4.childId2 = data[2];
            sensor4.childId3 = data[3];
            sensor4.interval = data[4];
            sensor4.status = data[5];
            sensor4.battLevel = data[6];
            sensor4.val1 = data[7];
            sensor4.val2 = data[8];
            sensor4.val3 = data[9];
            kal = (data[10] * 256) + data[11];
            sensor4.kalMin = kal;
            kal = (data[12] * 256) + data[13];
            sensor4.kalMax = kal;
            // last update
            for (i = 0; i < 6; i++)
            {
                sensor4.lastUpdate[i] = data[14 + i];
            }

            break;
        case 5:
            sensor5.nodeId = data[0];
            sensor5.childId1 = data[1];
            sensor5.childId2 = data[2];
            sensor5.childId3 = data[3];
            sensor5.interval = data[4];
            sensor5.status = data[5];
            sensor5.battLevel = data[6];
            sensor5.val1 = data[7];
            sensor5.val2 = data[8];
            sensor5.val3 = data[9];
            kal = (data[10] * 256) + data[11];
            sensor5.kalMin = kal;
            kal = (data[12] * 256) + data[13];
            sensor5.kalMax = kal;
            // last update
            for (i = 0; i < 6; i++)
            {
                sensor5.lastUpdate[i] = data[14 + i];
            }

            break;
        case 6:
            sensor6.nodeId = data[0];
            sensor6.childId1 = data[1];
            sensor6.childId2 = data[2];
            sensor6.childId3 = data[3];
            sensor6.interval = data[4];
            sensor6.status = data[5];
            sensor6.battLevel = data[6];
            sensor6.val1 = data[7];
            sensor6.val2 = data[8];
            sensor6.val3 = data[9];
            kal = (data[10] * 256) + data[11];
            sensor6.kalMin = kal;
            kal = (data[12] * 256) + data[13];
            sensor6.kalMax = kal;
            // last update
            for (i = 0; i < 6; i++)
            {
                sensor6.lastUpdate[i] = data[14 + i];
            }

            break;
        case 7:
            sensor7.nodeId = data[0];
            sensor7.childId1 = data[1];
            sensor7.childId2 = data[2];
            sensor7.childId3 = data[3];
            sensor7.interval = data[4];
            sensor7.status = data[5];
            sensor7.battLevel = data[6];
            sensor7.val1 = data[7];
            sensor7.val2 = data[8];
            sensor7.val3 = data[9];
            kal = (data[10] * 256) + data[11];
            sensor7.kalMin = kal;
            kal = (data[12] * 256) + data[13];
            sensor7.kalMax = kal;
            // last update
            for (i = 0; i < 6; i++)
            {
                sensor7.lastUpdate[i] = data[14 + i];
            }

            break;
        case 8:
            sensor8.nodeId = data[0];
            sensor8.childId1 = data[1];
            sensor8.childId2 = data[2];
            sensor8.childId3 = data[3];
            sensor8.interval = data[4];
            sensor8.status = data[5];
            sensor8.battLevel = data[6];
            sensor8.val1 = data[7];
            sensor8.val2 = data[8];
            sensor8.val3 = data[9];
            kal = (data[10] * 256) + data[11];
            sensor8.kalMin = kal;
            kal = (data[12] * 256) + data[13];
            sensor8.kalMax = kal;
            // last update
            for (i = 0; i < 6; i++)
            {
                sensor8.lastUpdate[i] = data[14 + i];
            }

            break;
        }
    }

     Serial.print("--------------------------\nLoad sensor ");
    Serial.println(sensor);

    Serial.print("NodeId: ");
    Serial.println(data[0]);
    Serial.print("ChildId1: ");
    Serial.println(data[1]);
    Serial.print("ChildId2: ");
    Serial.println(data[2]);
    Serial.print("ChildId3: ");
    Serial.println(data[3]);
    Serial.print("interval: ");
    Serial.println(data[4]);
    Serial.print("Status: ");
    Serial.println(data[5]);    
    Serial.print("Batt Leval: ");
    Serial.println(data[6]);
    Serial.print("Val1: ");
    Serial.println(data[7]);
    Serial.print("Val2: ");
    Serial.println(data[8]);
    Serial.print("Val3: ");
    Serial.println(data[9]);
    kal = (data[10] * 256 ) + data[11];
    Serial.print("Kal Min: ");
    Serial.println(kal);
    kal = (data[12] * 256 ) + data[13];
    Serial.print("Kal Max: ");
    Serial.println(kal);
    Serial.print("LastUpdate 20");
    Serial.print(data[14]);
    Serial.print("/");
    Serial.print(data[15]);
    Serial.print("/");
    Serial.println(data[16]);
    Serial.print(" ");
    Serial.print(data[17]);
    Serial.print(":");
    Serial.print(data[18]);
    Serial.print(":");
    Serial.println(data[19]);
}