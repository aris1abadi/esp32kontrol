#ifndef NODEHANDLE_H
#define NODEHANDLE_H

struct node
{
    //
    uint8_t nodeId;
    uint8_t childId;     
    uint8_t status;    
    uint8_t val;  
    uint8_t timeOut ;   
    uint8_t lastUpdate[6]; // format th,bl,tg,jam.mn.dt
    String nama;
    
};
struct sensor
{
    //30 byte length
    uint8_t nodeId;
    uint8_t childId;     
    uint8_t status;    
    uint8_t interval;    
    uint8_t battLevel;    
    float val; 
    int val2;
    int kalMin;
    int kalMax;
    uint8_t timeOut;
    uint8_t lastUpdate[6]; // format th,bl,tg,jam.mn.dt
    String nama ;
};

typedef struct node myNode;
typedef struct sensor mySensor;

void loadDefault();

#endif