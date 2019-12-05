#include "GKBluetooth.h"
// 5b75b75e-5add-11e9-8647-d663bd873d93

// // 5b75bd62-5add-11e9-8647-d663bd873d93

#define SERVICE_UUID "5b75b75e-5add-11e9-8647-d663bd873d93"
#define CHARACTERISTIC_UUID_GKP "5b75bd62-5add-11e9-8647-d663bd873d93"
#define BLE_SERVICE_NAME "GK"

//std::string rxValue;
//BLECharacteristic *pCharacteristic;
//bool clientBLEConnected = false;


class ServerCallbacks : public BLEServerCallbacks
{
private:
	bool clientBLEConnected = false;

  public:

    void onConnect(BLEServer *pServer)
    {
        Serial.println("Client connected to GATE");
        clientBLEConnected = true;

    };

    void onDisconnect(BLEServer *pServer)
    {
        Serial.println("Client disconnected from GATE");
        clientBLEConnected = false;
        // updateChallenge();
    };
    bool isClientConnected(){
        return clientBLEConnected;
    }

};

class ClientCallbacks : public BLECharacteristicCallbacks
{
private:
	std::string rxValue;

public:
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        rxValue = pCharacteristic->getValue();

        if (sizeof(rxValue) > 0)
        {
            Serial.println("*********");
            Serial.print("Received Value: ");

            for (int i = 0; i < rxValue.length(); i++)
            {
                Serial.println(rxValue[i]);
            }

        }
    }
};


GKBluetooth::GKBluetooth()
{
    BLEDevice::init(BLE_SERVICE_NAME);
    Serial.println("BLE Started");

    this->CreateServer();
    this->CreateService();
    this->CreateCharacteristic();

    pService->start();
    pServer->getAdvertising()->start();

//    pAdvertising = BLEDevice::getAdvertising();
//    pAdvertising->addServiceUUID(SERVICE_UUID);
//
//    pAdvertising->start();




    Serial.println("BLE Initalized");

}

void GKBluetooth::CreateServer(){
    pServer = BLEDevice::createServer();
    ServerCallbacks *sCallbacks = new ServerCallbacks();

    pServer->setCallbacks(sCallbacks);



}

void GKBluetooth::CreateService(){
    //server has to be instanitated
//    if(pServer == nullptr)
        pService = pServer->createService(BLEUUID(SERVICE_UUID));

}

void GKBluetooth::CreateCharacteristic(){
	Serial.println("Creating Char");
    pCharacteristic = pService->createCharacteristic(
        BLEUUID(CHARACTERISTIC_UUID_GKP), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setCallbacks(new ClientCallbacks());
    Serial.println("Created Characteristic");
    std::string rx = pCharacteristic->getValue();
    Serial.printf("\nRX:\n %s", rx.c_str());
}

//TX from Device PoV
void GKBluetooth::TxCharacteristic(uint8_t* data,size_t size){
    if(pCharacteristic)
        pCharacteristic->setValue(data, size);
        pCharacteristic->notify();
}

std::string GKBluetooth::RxString()
{
//	std::string rx = "";
////	if(pCharacteristic != nullptr)
//		rx = pCharacteristic->getValue();
//	return rx;
	return pCharacteristic->getValue();
}

String GKBluetooth::getServiceUUID()
{
    return String(SERVICE_UUID);
}
