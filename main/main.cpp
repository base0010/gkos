//#pragma GCC diagnostic ignored "[-Werror=narrowing]"
//#pragma GCC diagnostic ignored "-Werror=return-type"
//#pragma GCC diagnostic ignored "-Werror=maybe-uninitialized"

#include "FP.h"
#include <M5StickC.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include "pb.h"
#include "../gk_pbrpc/nanopb_cpp/protos/simple.pb.h"
#define GKOS_Version 0.1

TaskHandle_t loopTaskHandle = NULL;
bool loopTaskWDTEnabled = false;
std::shared_ptr<FPBluetooth> ble;

void startBLE(){
		Serial.println("Setting Up BLE\n");
		ble = std::make_shared<FPBluetooth>();
//todo: a better way to bootstrap debugging

}

void pbTest(){
	Serial.println("PB Stuffs");
	uint8_t buffer[128];
	SimpleMessage message = SimpleMessage_init_zero;
	/* Create a stream that will write to our buffer. */
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

	/* Fill in the lucky number */
	message.lucky_number = 13;

	/* Now we are ready to encode the message! */
	bool status = pb_encode(&stream, SimpleMessage_fields, &message);
	auto message_length = stream.bytes_written;

	if (!status) {
		Serial.println('/nEncoding PB failed :(');
	} else {
		//Decode Message
		SimpleMessage dMsg = SimpleMessage_init_zero;
		pb_istream_t dStream = pb_istream_from_buffer(buffer, message_length);

		auto dStatus = pb_decode(&dStream, SimpleMessage_fields, &dMsg);

		if (!dStatus) {
			Serial.printf("Decoding PB failed ;( %s\n", PB_GET_ERROR(&dStream));
		} else {
			//decode worked
			Serial.printf("your lucky number is: \n %d",
					(int) dMsg.lucky_number);
		}

	}


}
void setup(){
	Serial.begin(115200);
//	M5.begin();
	pbTest();
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
