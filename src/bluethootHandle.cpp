#define USE_BLE

#ifdef USE_BLE
#include <NimBLEDevice.h>
#include <kontrolHandle.h>
#include <mySensorsHandle.h>

bool isBusy = false;
bool btDisconnectStatus = false;
long disCDelay;

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

String btBuff = "";

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
    /***************** New - Security handled here ********************
    ****** Note: these are the same return values as defaults ********/
    uint32_t onPassKeyRequest()
    {
        Serial.println("Server PassKeyRequest");
        return 123456;
    }

    bool onConfirmPIN(uint32_t pass_key)
    {
        Serial.print("The passkey YES/NO number: ");
        Serial.println(pass_key);
        return true;
    }

    void onAuthenticationComplete(ble_gap_conn_desc desc)
    {
        Serial.println("Starting BLE work!");
    }
    /*******************************************************************/
};

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue();

        String btTopic = "";
        String btPayload = "";
        bool endMsg = false;

        if (rxValue.length() > 0)
        {

            // Serial.println("*********");

            for (int i = 0; i < rxValue.length(); i++)
            {
                btBuff += rxValue[i];
                if (rxValue[i] == '\n')
                {
                    endMsg = true;
                    break;
                }
                // Serial.print(rxValue[i]);
            }
            if (endMsg)
            {
                if (btBuff.length() > 5)
                {
                    btTopic = getValue(btBuff, ';', 0);
                    btPayload = getValue(btBuff, ';', 1);
                    cekMsg(btTopic, btPayload);
                    Serial.print("btMsg: ");
                    Serial.println(btBuff);
                     Serial.print("topic: ");
                    Serial.println(btTopic);
                     Serial.print("paylod: ");
                    Serial.println(btPayload);
                    btBuff = "";
                    endMsg = false;
                }
                else
                {
                    btBuff = "";
                }
            }

            // Serial.println();
            //  Serial.println("*********");
        }
    }
};

void kirimBluethooth(String msg)
{
    if (deviceConnected && !isBusy)
    {
        isBusy = true;
        msg += '\n';

        int packetSize = NimBLEDevice::getMTU() - 3; // 3 byte digunakan untuk overhead
        //int packetSize = 20;
        if (msg.length() > packetSize)
        {
            int msgCount = 0;
            int packetCount = 0;
            String packet = "";
            for (int i = 0; i < msg.length(); i += packetSize)
            {
                packetCount = i;
                packet = msg.substring(i, i + packetSize);

                pTxCharacteristic->setValue(packet);
                pTxCharacteristic->notify();
                msgCount++;

                delay(10); // jeda kecil untuk memastikan paket sampai
            }
            if (msg.length() > packetCount)
            {
                Serial.println("kirim paket terakhir:");
                delay(10);
                String lastPacket = msg.substring(packetCount, msg.length() - 1);
                pTxCharacteristic->setValue(lastPacket);
                pTxCharacteristic->notify();
                Serial.println(lastPacket);
            }

            Serial.print("packet size:");
            Serial.println(packetSize);
            Serial.print("jml packet:");
            Serial.println(msg.length());
            Serial.print("jml Terkirim:");
            Serial.println(packetCount);
            Serial.print("jml paket:");
            Serial.println(msgCount);
        }
        else
        {
            pTxCharacteristic->setValue(msg);
            pTxCharacteristic->notify();
            // Serial.print("Kirim ke BLE: ");
            // Serial.println(msg);
        }
        isBusy = false;
    }
}

void bluethooth_init()
{
    // Create the BLE Device
    BLEDevice::init("karjoAgro V01");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        /******* Enum Type NIMBLE_PROPERTY now *******
            BLECharacteristic::PROPERTY_NOTIFY
            );
        **********************************************/
        NIMBLE_PROPERTY::NOTIFY);

    /***************************************************
     NOTE: DO NOT create a 2902 descriptor
     it will be created automatically if notifications
     or indications are enabled on a characteristic.

     pCharacteristic->addDescriptor(new BLE2902());
    ****************************************************/

    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        /******* Enum Type NIMBLE_PROPERTY now *******
                BLECharacteristic::PROPERTY_WRITE
                );
        *********************************************/
        NIMBLE_PROPERTY::WRITE);

    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start the service
    pService->start();

    // Start advertising
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");
}

void bluethooth_loop()
{

    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        if (!btDisconnectStatus)
        {
            btDisconnectStatus = true;
            disCDelay = millis();
        }
    }
    if (btDisconnectStatus)
    {
        if ((millis() - disCDelay) > 500)
        {
            btDisconnectStatus = false;
            pServer->startAdvertising(); // restart advertising
            Serial.println("start advertising");
            oldDeviceConnected = deviceConnected;
        }
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

#else

#include <BluetoothSerial.h>
#include <kontrolHandle.h>
#include <mySensorsHandle.h>

String device_name = "karjoAgro V01";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

String btMsgInput;
bool btMsgReady;
String btBuff = "";
String btTopic = "";
String btPayload = "";
uint8_t separateCount = 0;

bool isBluethoothConnected()
{
    return SerialBT.connected();
}

void kirimBluethooth(String msg)
{
    // format topic=msg
    if (SerialBT.connected())
    {
        SerialBT.print(msg);
    }
}

void bluethooth_init()
{
    SerialBT.begin(device_name);
    // Setup locally attached sensors
}

void presentation()
{
    // Present locally attached sensors
}

void bluethooth_loop()
{
    // Send locally attached sensor data here
    if (SerialBT.available())
    {
        char b = SerialBT.read();
        if (b == ';')
        {
            if (separateCount == 0)
            {
                separateCount = 1;
                btBuff = "";
            }
            else if (separateCount == 1)
            {
                btTopic = btBuff;
                separateCount = 2;
            }

            else if (separateCount == 2)
            {
                btPayload = btBuff;
                separateCount = 0;
                cekMsg(btTopic, btPayload);
            }
            // String btTopic = getValue(btBuff, ';', 0);
            // String btPayload = getValue(btBuff, ';', 1);
            // cekMsg(btTopic, btPayload);
            Serial.println(btBuff);
            btBuff = "";
        }
        else
        {
            btBuff += b;
        }
    }
}

#endif