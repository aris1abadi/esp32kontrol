#include <Arduino.h>
#include <nodeHandle.h>
#include <eepromHandle.h>


myNode aktuator[24]={
    {1,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator1"},
    {1,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator2"},
    {2,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator3"},
    {2,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator4"},
    {3,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator5"},
    {3,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator6"},
    {4,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator7"},
    {4,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator8"},
    {5,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator9"},
    {5,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator10"},
    {6,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator11"},
    {6,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator12"},
    {7,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator13"},
    {7,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator14"},
    {8,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator15"},
    {8,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator16"},
    {9,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator17"},
    {9,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator18"},
    {10,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator19"},
    {10,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator20"},
    {12,1,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator21"},
    {12,2,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator22"},
    {12,3,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator23"},
    {12,4,0,0,0,/*lastUpdate*/{0,0,0,0,0,0},"Aktuator24"},
};
mySensor sensorFlow[24]={//sensorFlow ada tiap aktuator
    {1,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow1"},
    {1,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow2"},
    {2,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow3"},
    {2,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow4"},
    {3,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow5"},
    {3,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow6"},
    {4,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow7"},
    {4,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow8"},
    {5,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow9"},
    {5,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow10"},
    {6,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow11"},
    {6,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow12"},
    {7,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow13"},
    {7,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow14"},
    {8,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow15"},
    {8,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow16"},
    {9,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow17"},
    {9,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow18"},
    {10,11,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow19"},
    {10,12,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Flow20"},
    {12,20,0,10,0,0,0,455,1400,0,{0,0,0,0,0,0},"Sensor Flow21"},
    {12,21,0,10,0,0,0,455,1400,0,{0,0,0,0,0,0},"Sensor Flow22"},
    {12,22,0,10,0,0,0,455,1400,0,{0,0,0,0,0,0},"Sensor Flow23"},
    {12,23,0,10,0,0,0,455,1400,0,{0,0,0,0,0,0},"Sensor Flow24"},
};

mySensor sensorLengas[4]={
    {51,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Lengas1"},
    {52,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Lengas2"},
    {53,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Lengas3"},
    {54,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Lengas4"}
};

mySensor sensorTemperature[4]={
    {61,2,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Temp1"},
    {62,2,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Temp2"},
    {63,2,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Temp3"},
    {64,2,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Temp4"}
};

mySensor sensorHumidity[4]={
    {61,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Hum1"},
    {62,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Hum2"},
    {63,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Hum3"},
    {64,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Sensor Hum4"}
};

mySensor sensorLengasIntermittent[4]={
    {81,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Lengasinter1"},
    {82,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Lengasinter2"},
    {83,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Lengasinter3"},
    {84,1,0,10,0,0,0,4320,1400,0,{0,0,0,0,0,0},"Lengasinter4"}
};


void loadDefault(){
   
}




