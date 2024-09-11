#include <Arduino.h>
#include <nodeHandle.h>
#include <eepromHandle.h>

Aktuator aktuator1;
Aktuator aktuator2;
Aktuator aktuator3;
Aktuator aktuator4;
Aktuator aktuator5;
Aktuator aktuator6;
Aktuator aktuator7;
Aktuator aktuator8;

Sensor sensor1;//node 51 ch 10 lengas
Sensor sensor2;//node 52
Sensor sensor3;//node 53
Sensor sensor4;//node 54 ch 10
Sensor sensor5;//node 60 dht22
Sensor sensor6;//node 61 
Sensor sensor7;//node 60 dht22
Sensor sensor8;//node 61 
//Sensor sensorHumidity;  //node 60

void loadDefault(){
    
    loadNode(1);
    loadNode(2);
    loadNode(3);
    loadNode(4);
    loadNode(5);
    loadNode(6);
    loadNode(7);
    loadNode(8);

    loadSensor(1);
    loadSensor(2);
    loadSensor(3);
    loadSensor(4);
    loadSensor(5);
    loadSensor(6);
    loadSensor(7);
    loadSensor(8);    

}


