#pragma once

#ifndef GK_H
#define GK_H

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "Arduino.h"
#include "WiFi.h"
#include "GKBluetooth.h"
#include "LCD.h"
#include "StateStore.h"
#include "ConnextAPI.h"

class GK{

public:
	GK(std::shared_ptr<GKBluetooth> bleInterface);
	void PollChannel();
	LCD *lcd;

private:

	std::shared_ptr<GKBluetooth> ble;
	int pollLambda = 500;
    std::string latestCommand = "";
    bool wifiConnected = false;
    bool commandPoll = true;

    StateStore *ss;
    ConnextAPI *con;



	 	void InitHW();
	    void InitRadios();


//	    static void startArduino();


//	    void StartBLE();
	    static void startBLEImpl(void*);
	    void doBLE();

	    void StartWiFi();
	    void Splash();
	    void AuthWithHub(bool retry = false);
	    bool hubAuthed = false;



	    void PollCommands();
	    static void startCommandsImpl(void *);
	    void startCommands();

//	    void PollChannel();
	    static void startPollingChannelImpl(void *);
	     void startPollingChannel();

	    void DemoDeposit();
	    static void startInitialDepositImpl(void*);
	     void startInitialDeposit();

	     void ASCIISplash();
};

#endif
