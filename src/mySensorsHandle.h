#ifndef mySensorsHandle_h
#define mySensorsHandle_h

//void handleInp(MyMessage msg, String textMode);
void cekMySensors(String msg);
void kirim_ke_device(uint8_t node, uint8_t childnode, uint8_t type, String payload);
void kontrolAktuator(uint8_t nomerAktuator, uint8_t on_off);
String getValue(String data, char separator, int index);
float getSensorValue(uint8_t sensor,uint8_t index);
void setSensorInterval(uint8_t interval, uint8_t nomerSensor, uint8_t jenisSensor);
void resetFlowSensor(uint8_t nomerSensor);

#endif