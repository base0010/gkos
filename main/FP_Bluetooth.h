#pragma once
#ifndef FPBluetooth_H
#define FPBluetooth_H

#include <Arduino.h>
#include "BLEDevice.h"
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <string>

class FPBluetooth{
	private:
	BLEServer *pServer = nullptr;
	BLEService *pService = nullptr;
	BLEAdvertising *pAdvertising = nullptr;


    public:

      FPBluetooth();

      String getServiceUUID();
      BLECharacteristic *pCharacteristic = nullptr;

      void CreateServer();

      void CreateService();
      void CreateCharacteristic();
      void TxCharacteristic(uint8_t *data, size_t size);

      std::string RxString();

//    private:
};

#endif
