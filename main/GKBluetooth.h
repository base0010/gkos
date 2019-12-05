#pragma once
#ifndef GKBluetooth_H
#define GKBluetooth_H


#include <Arduino.h>
#include "BLEDevice.h"
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#include <string>

// #define SERVICE_UUID = "6b3e35e8-2299-11e9-ab14-d663bd873d93";
// #define CHARACTERISTIC_UUID_GKP = "6b3e3728-2299-11e9-ab14-d663bd873d93";
// #define BLE_SERVICE_NAME = "EBike";



class GKBluetooth{
	private:

//	BLECharacteristic *pCharacteristic = nullptr;
	BLEServer *pServer = nullptr;
	BLEService *pService = nullptr;
	BLEAdvertising *pAdvertising = nullptr;





    public:

      GKBluetooth();

      String getServiceUUID();
      BLECharacteristic *pCharacteristic = nullptr;

      void CreateServer();

      void CreateService();
      void CreateCharacteristic();
      void TxCharacteristic(uint8_t *data, size_t size);
      //uint8_t* RxCharacteristic();
      std::string RxString();

    private:
};

#endif
