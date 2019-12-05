//#pragma GCC diagnostic ignored "[-Werror=narrowing]"
//#pragma GCC diagnostic ignored "-Werror=return-type"
//#pragma GCC diagnostic ignored "-Werror=maybe-uninitialized"

#include "FP.h"
#include <M5StickC.h>
#include <pb_decode.h>
#include <pb_encode.h>


#define GKOS_Version 0.1

TaskHandle_t loopTaskHandle = NULL;
bool loopTaskWDTEnabled = false;

std::shared_ptr<FPBluetooth> ble;

void startBLE(){
		Serial.println("Setting Up BLE\n");
		ble = std::make_shared<FPBluetooth>();

		Serial.println("protocol buffer");
	    uint8_t buffer[128];
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(4));

}

void setup(){
	Serial.begin(115200);

//	M5.begin();
	startBLE();
	vTaskDelay(1000/portTICK_RATE_MS);

	Serial.println("Arduino and BLE Started");
}


void arduinoLoopTask(void *pvParameters){
//	M5.begin();

	setup();

	for(;;){
		if(loopTaskWDTEnabled){
			esp_task_wdt_reset();
		}
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}



void startArduino(){
	//radios might need to be setup in loopTask (see /arduino/main.cpp)
	initArduino();
	xTaskCreateUniversal(arduinoLoopTask, "loopTask",8192, NULL, 3, &loopTaskHandle, 0);

	//todo: less shitty way to make sure ble has started
	vTaskDelay(1000/portTICK_RATE_MS);
	Serial.println("Arduino Started");




}
extern "C" void app_main()
{
	startArduino();

	FP *fp = new FP(ble);


	M5.begin();
	M5.Lcd.fillScreen(BLUE);
	M5.Lcd.print("Totem Started!");
	vTaskDelay(1000/portTICK_RATE_MS);


}
