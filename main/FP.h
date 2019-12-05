#pragma once

#ifndef FP_H
#define FP_H

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "Arduino.h"
#include "WiFi.h"
#include "FP_Bluetooth.h"
#include "LCD.h"
#include "StateStore.h"
#include "ConnextAPI.h"

class FP{

public:
	FP(std::shared_ptr<FPBluetooth> bleInterface);


private:

	std::shared_ptr<FPBluetooth> ble;
	 	void InitHW();
	    void InitRadios();

    static void startBLEImpl(void*);
	    void doBLE();


	    void Splash();



	    void PollCommands();
	    static void startCommandsImpl(void *);
	    void startCommands();


	     void ASCIISplash();
};

#endif
