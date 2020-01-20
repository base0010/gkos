#include "FP_Bluetooth.h"
#include "BLE_RPC.h"

#include <M5StickC.h>


#define BLE_SERVICE_NAME "FP"
#define SERVICE_UUID "5b75b75e-5add-11e9-8647-d663bd873d93"

#define CHARACTERISTIC_UUID_FP "613a4f5b-0023-4099-9abe-2161382025ed"


//this will be for protobuf impl.
//#define RPC_COMMAND_UUID "dff94330-ad82-48f3-aeac-87052723576a"

//has writable attribute for issuing new RPC Commands
//has readable attribute for showing RPC Command status

std::string RPC_COMMAND_PREAMBLE = "GK_RPC";


class ServerCallbacks : public BLEServerCallbacks
{
private:
	bool clientBLEConnected = false;

  public:

    void onConnect(BLEServer *pServer)
    {
        Serial.println("Client connected to Totem");
        M5.Lcd.println("client connected");
        //set stronger on connection established.
//        esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT=11,ESP_PWR_LVL_P9 );
//        todo: didn't work
        clientBLEConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        Serial.println("Client disconnected from Totem");
        M5.Lcd.println("client disconnected");
        clientBLEConnected = false;
    };
    bool isClientConnected(){
        return clientBLEConnected;
    }

};

//class ClientCallbacks : public BLECharacteristicCallbacks
//{
//private:
//	std::string rxValue;
//
//public:
//    void onWrite(BLECharacteristic *pCharacteristic)
//    {
//        rxValue = pCharacteristic->getValue();
//
//        if (sizeof(rxValue) > 0)
//        {
//            Serial.println("*********");
//            Serial.print("Received Value: ");
//
//            for (int i = 0; i < rxValue.length(); i++)
//            {
//                Serial.println(rxValue[i]);
//            }
//
//        }
//    }
//};


//callbacks for incoming RPC_Commands via RPC_COMMAND Characteristic

class ClientCallbacks : public BLECharacteristicCallbacks {
private:
	std::string rxValue;
	BLE_RPC *blerpc = new BLE_RPC();
public:

	  void onWrite(BLECharacteristic *pCharacteristic)
	    {
	        rxValue = pCharacteristic->getValue();

	        if (sizeof(rxValue) > 0)
	        {
	            Serial.println("\n*********");
	            Serial.print("Received Value On RPC UUID: ");
	            blerpc->setRX(rxValue);
	            }
	        }
};


FPBluetooth::FPBluetooth()
{
    BLEDevice::init(BLE_SERVICE_NAME);
    Serial.println("BLE Started");

    BLEDevice::setPower(ESP_PWR_LVL_P9);
    Serial.println("set ble power?");

    this->CreateServer();
    this->CreateService();
    this->CreateCharacteristic();

    pService->start();

//    pServer->getAdvertising()->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

    BLEDevice::startAdvertising();



    Serial.println("BLE Initalized");
    Serial.println("Service UUID:\n");
    Serial.println("attempt power set\n\n\n");
    BLEDevice::setPower(ESP_PWR_LVL_P9);
    Serial.println(SERVICE_UUID);

}

void FPBluetooth::CreateServer(){
    pServer = BLEDevice::createServer();
    ServerCallbacks *sCallbacks = new ServerCallbacks();
    pServer->setCallbacks(sCallbacks);
}

void FPBluetooth::CreateService(){
        pService = pServer->createService(BLEUUID(SERVICE_UUID));
}

void FPBluetooth::CreateCharacteristic(){
	Serial.println("BLE Characteristic Creation");
    pCharacteristic = pService->createCharacteristic(
        BLEUUID(CHARACTERISTIC_UUID_FP), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new ClientCallbacks());
    Serial.println("Created Characteristic");
    std::string rx = pCharacteristic->getValue();
    Serial.printf("\nRX:\n %s", rx.c_str());
}

//TX from Device PoV
void FPBluetooth::TxCharacteristic(uint8_t* data,size_t size){
    if(pCharacteristic){
        pCharacteristic->setValue(data, size);
        pCharacteristic->notify();
    }
}

std::string FPBluetooth::RxString()
{
	return pCharacteristic->getValue();
}

String FPBluetooth::getServiceUUID()
{
    return String(SERVICE_UUID);
}
