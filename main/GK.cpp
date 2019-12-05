#include "GK.h"


GK::GK(std::shared_ptr<GKBluetooth> bleInterface){
	//start ard
//	this->startArduino();
//	this->InitHW();

//	this->StartBLE();
	ble = bleInterface;
	con = new ConnextAPI("");
	ss = new StateStore(userAddress, con, 2000);
	this->ASCIISplash();
	this->startCommands();


}

void GK::InitHW(){
    this->Splash();
    this->InitRadios();
}

void GK::InitRadios(){
    //todo:special stuff? lroa?
    //todo: remove StartWiFi() infinate loop
	this->StartWiFi();
//    this->StartBLE();

	//	this->doBLE();
//   this->doBLE();

}
//void GK::startBLEImpl(void *_this){
//	static_cast<GK *>(_this)->StartBLE();
//}
//
//void GK::doBLE(){
//	xTaskCreate(this->startBLEImpl, "BLE", 5000, this, 3, NULL);
//}



void GK::startCommandsImpl(void *_this)
{
    static_cast<GK *>(_this)->PollCommands();
}

void GK::startCommands()
{
	TaskHandle_t someTask = NULL;
    xTaskCreateUniversal(this->startCommandsImpl, "PollCommands", 5000, this, 1, &someTask, 1);
}
void GK::startPollingChannelImpl(void *_this)
{
    static_cast<GK *>(_this)->PollChannel();
}

void GK::startPollingChannel()
{
    Serial.println("Starting Channel Poll");
    xTaskCreate(this->startPollingChannelImpl, "PollChannel", 15000, this, 2, NULL);
}

void GK::startInitialDepositImpl(void *_this)
{
    static_cast<GK *>(_this)->DemoDeposit();
}

void GK::startInitialDeposit()
{
    Serial.println("Doing Initial Deposit");
    xTaskCreate(this->startInitialDepositImpl, "InitialDeposit", 15000, this,4, NULL);
}
void GK::DemoDeposit()
{
    ss->InititalDeviceDeposit("10000000", "0");
    vTaskDelay(2000/portTICK_RATE_MS);
    //ss->Collateralize();
    vTaskDelay(20000/portTICK_RATE_MS);
    //ss->ConfirmCollateralize();
    // ss->Start();
}
void GK::PollChannel()
{
    Serial.println("Polling Hub for Updated Balance");
    // esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    // esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);
//     this->AuthWithHub();
//     if (hubAuthed)
//         ss = new StateStore(uAddress);
    ss->Start();
}
void GK::PollCommands()
{
    bool commandIsRunning = false;
    Serial.println("Inside Poll commands");
    for (;;)
    {
//    	latestCommand = "a";
    	std::string latestCommand = ble->RxString();
//    	ble->pCharacteristic->toString();
//    	latestCommand = ble.swap()
//    	try{
//    	Serial.println(soeth.c_str());
//
//    	std::string someshit = ble->RxString();
//    	Serial.printf("\nSOMESHOT\n %s",someshit.c_str());
//    	}catch(...){}


        Serial.printf("\nCommand String: \n %s", latestCommand.c_str());

        if (latestCommand.compare("GGGGFFFFF") == 0)
        {
            Serial.println("FLY");
            break;
        }
        else if (latestCommand.compare("initDep") == 0 && commandIsRunning == false)
        {
            // Serial.println("Doing Initial Deposit");
            // this->AuthWithHub();
            // delay(300);
            // this->DemoDeposit();
            // Serial.println("\n\n\nHELLO DARKNESS MY OLD FRIEND");
            this->startInitialDeposit();
            commandIsRunning = true;
        }
        else if (latestCommand.compare("pollChannel") == 0)
        {
            this->startPollingChannel();
            latestCommand = "none";
        }
        else if (latestCommand.compare("stop") == 0)
        {
            Serial.println("STOP POLLING");
//TODO: Implement StateStore & Uncomment
//            ss->Stop();
//            ss->poll = false;
//            ss->poll.store(false);
//            Serial.printf("SS %i", (int)ss->poll);
        }
        //todo: delete rxString about here
        vTaskDelay(1000 /portTICK_RATE_MS );
    }
    vTaskDelay(1000 /portTICK_RATE_MS );


}

void GK::ASCIISplash(){
	Serial.printf(R"EOF(@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;.            .+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,                        :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;                                +@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;                                      #@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`                                          ,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                               `@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                   `@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`                                                      ,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;                        .:+++++;:.                        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@                    ,@@@@@@@@@@@@@@@@@@@#.                   `@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@'                 `@@@@@@@@@@@@@@@@@@@@@@@@@@@+                  @@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@`                @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'                :@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@               .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@               `@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@              .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@               @@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@             `@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@+             @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+             @@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@+            ,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@             @@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@#            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'            @@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@            @@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@           .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@            @@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@           ,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@           `@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@           ;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`          :@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@:          ;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`          @@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@          :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@           @@@@@@@@@@@@@@@
@@@@@@@@@@@@@          .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@          `@@@@@@@@@@@@@@
@@@@@@@@@@@@,          @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@          `@@@@@@@@@@@@@
@@@@@@@@@@@@          @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@++@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@          @@@@@@@@@@@@@
@@@@@@@@@@@          @@@@@@@@@@@@@@@@@@@@@@@@@@@@@'              ,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@;         :@@@@@@@@@@@@
@@@@@@@@@@@         :@@@@@@@@@@@@@@@@@@@@@@@@@@:                    `@@@@@@@@@@@@@@@@@@@@@@@@@@@@          @@@@@@@@@@@@
@@@@@@@@@@          @@@@@@@@@@@@@@@@@@@@@@@@@                          @@@@@@@@@@@@@@@@@@@@@@@@@@@         `@@@@@@@@@@@
@@@@@@@@@#         @@@@@@@@@@@@@@@@@@@@@@@@                              @@@@@@@@@@@@@@@@@@@@@@@@@+         @@@@@@@@@@@
@@@@@@@@@         .@@@@@@@@@@@@@@@@@@@@@@.                                .@@@@@@@@@@@@@@@@@@@@@@@@         `@@@@@@@@@@
@@@@@@@@@         @@@@@@@@@@@@@@@@@@@@@@                                    @@@@@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@
@@@@@@@@         ;@@@@@@@@@@@@@@@@@@@@+                                      +@@@@@@@@@@@@@@@@@@@@@@         ,@@@@@@@@@
@@@@@@@@         @@@@@@@@@@@@@@@@@@@@,                                        ,@@@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@
@@@@@@@,        :@@@@@@@@@@@@@@@@@@@,                                          .@@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@
@@@@@@@         @@@@@@@@@@@@@@@@@@@:                  .;+@@@+.                  .@@@@@@@@@@@@@@@@@@@@@         @@@@@@@@
@@@@@@@        `@@@@@@@@@@@@@@@@@@#               .@@@@@@@@@@@@@@,               :@@@@@@@@@@@@@@@@@@@@         @@@@@@@@
@@@@@@`        @@@@@@@@@@@@@@@@@@@              +@@@@@@@@@@@@@@@@@@#              @@@@@@@@@@@@@@@@@@@@;        #@@@@@@@
@@@@@@         @@@@@@@@@@@@@@@@@@             ;@@@@@@@@@@@@@@@@@@@@@@'             @@@@@@@@@@@@@@@@@@@@         @@@@@@@
@@@@@@        :@@@@@@@@@@@@@@@@@;            @@@@@@@@@@@@@@@@@@@@@@@@@@             @@@@@@@@@@@@@@@@@@@         @@@@@@@
@@@@@;        @@@@@@@@@@@@@@@@@@           `@@@@@@@@@@@@@@@@@@@@@@@@@@@@`           @@@@@@@@@@@@@@@@@@@'        @@@@@@@
@@@@@         @@@@@@@@@@@@@@@@@.          .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.           @@@@@@@@@@@@@@@@@@@        '@@@@@@
@@@@@        `@@@@@@@@@@@@@@@@@           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@           #@@@@@@@@@@@@@@@@@@         @@@@@@
@@@@@        +@@@@@@@@@@@@@@@@:          @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@           @@@@@@@@@@@@@@@@@@`        @@@@@@
@@@@@        @@@@@@@@@@@@@@@@@          @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@          @@@@@@@@@@@@@@@@@@+        @@@@@@
@@@@:        @@@@@@@@@@@@@@@@@         `@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.         ,@@@@@@@@@@@@@@@@@@        @@@@@@
@@@@`        @@@@@@@@@@@@@@@@.         @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@          @@@@@@@@@@@@@@@@@@        +@@@@@
@@@@         @@@@@@@@@@@@@@@@         `@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.         @@@@@@@@@@@@@@@@@@        :@@@@@
@@@@        :@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@         `@@@@@@@@@@@@@@@@@        `@@@@@
@@@@        +@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@         ,@@@@@@@@@@@@@@@@@         @@@@@
@@@@        @@@@@@@@@@@@@@@@,        `@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:         @@@@@@@@@@@@@@@@@.        @@@@@
@@@@        @@@@@@@@@@@@@@@@         +@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@:        @@@@@
@@@@        @@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@;        @@@@@
@@@@        @@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@       .@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@+        @@@@@
@@@@        @@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@+        @@@@@
@@@@        @@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@'        @@@@@
@@@@        @@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@:        @@@@@
@@@@        @@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@.        @@@@@
@@@@        +@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@         @@@@@
@@@@        :@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@         @@@@@
@@@@        `@@@@@@@@@@@@@@@         +@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@        :@@@@@
@@@@         @@@@@@@@@@@@@@@,         @@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@,        ,@@@@@@@@@@@@@@@@@        +@@@@@
@@@@:        @@@@@@@@@@@@@@@`         @@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@         #@@@@@@@@@@@@@@@@@        @@@@@@
@@@@@        @@@@@@@@@@@@@@@@         `@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@@         @@@@@@@@@@@@@@@@@+        @@@@@@
@@@@@        #@@@@@@@@@@@@@@@          @@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@`         @@@@@@@@@@@@@@@@@.        @@@@@@
@@@@@        .@@@@@@@@@@@@@@@`         @@@@@@@@@@@@@       .@@@@@@@@@@@@@@@@@         `@@@@@@@@@@@@@@@@@         @@@@@@
@@@@@         @@@@@@@@@@@@@@@@          @@@@@@@@@@@@       .@@@@@@@@@@@@@@@@          @@@@@@@@@@@@@@@@@@        :@@@@@@
@@@@@:        @@@@@@@@@@@@@@@@          '@@@@@@@@@@@       .@@@@@@@@@@@@@@@'          @@@@@@@@@@@@@@@@@+        @@@@@@@
@@@@@@        ;@@@@@@@@@@@@@@@:          @@@@@@@@@@@       .@@@@@@@@@@@@@@@          .@@@@@@@@@@@@@@@@@         @@@@@@@
@@@@@@         @@@@@@@@@@@@@@@@           @@@@@@@@@@       .@@@@@@@@@@@@@@           @@@@@@@@@@@@@@@@@@         @@@@@@@
@@@@@@`        @@@@@@@@@@@@@@@@,           @@@@@@@@@       :@@@@@@@@@@@@@           `@@@@@@@@@@@@@@@@@+        +@@@@@@@
@@@@@@`        ,@@@@@@@@@@@@@@@@            @@@@@@@@       :@@@@@@@@@@@@            @@@@@@@@@@@@@@@@@@         @@@@@@@@
@@@@@@@         @@@@@@@@@@@@@@@@#            ;@@@@@@       :@@@@@@@@@@,            :@@@@@@@@@@@@@@@@@@         @@@@@@@@
@@@@@@@`        +@@@@@@@@@@@@@@@@`             @@@@@       :@@@@@@@@@              @@@@@@@@@@@@@@@@@@`        #@@@@@@@@
@@@@@@@@         @@@@@@@@@@@@@@@@@               '@@       :@@@@@@@               @@@@@@@@@@@@@@@@@@@         @@@@@@@@@
@@@@@@@@         +@@@@@@@@@@@@@@@@@                        :@@@@@@               @@@@@@@@@@@@@@@@@@@`        .@@@@@@@@@
@@@@@@@@#         @@@@@@@@@@@@@@@@@@                       '@@@@@.              +@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@
@@@@@@@@@         :@@@@@@@@@@@@@@@@@`                      +@@@@@              #@@@@@@@@@@@@@@@@@@@          @@@@@@@@@@
@@@@@@@@@;         @@@@@@@@@@@@@@@@@@@                     +@@@@@             @@@@@@@@@@@@@@@@@@@@@         @@@@@@@@@@@
@@@@@@@@@@          @@@@@@@@@@@@@@@@@@@                    +@@@@@            @@@@@@@@@@@@@@@@@@@@@          @@@@@@@@@@@
@@@@@@@@@@+         +@@@@@@@@@@@@@@@@@@@.                  +@@@@@          :@@@@@@@@@@@@@@@@@@@@@`         @@@@@@@@@@@@
@@@@@@@@@@@          @@@@@@@@@@@@@@@@@@@@@                 +@@@@@         @@@@@@@@@@@@@@@@@@@@@@#         .@@@@@@@@@@@@
@@@@@@@@@@@@          @@@@@@@@@@@@@@@@@@@@@'               +@@@@@       @@@@@@@@@@@@@@@@@@@@@@@@          @@@@@@@@@@@@@
@@@@@@@@@@@@`         `@@@@@@@@@@@@@@@@@@@@@@'             +@@@@@    `@@@@@@@@@@@@@@@@@@@@@@@@@          +@@@@@@@@@@@@@
@@@@@@@@@@@@@          ;@@@@@@@@@@@@@@@@@@@@@@@@.          @@@@@@ ,@@@@@@@@@@@@@@@@@@@@@@@@@@@           @@@@@@@@@@@@@@
@@@@@@@@@@@@@`          +@@@@@@@@@@@@@@@@@@@@@@@@@@+.     .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.          @@@@@@@@@@@@@@@
@@@@@@@@@@@@@@.          @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,          #@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,          .@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@           +@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.           @@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@           :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`           @@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@+           `@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@            @@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@;            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@            @@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@,            +@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.            `@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@,             @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@             `@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@'             :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`             @@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@+              +@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,              @@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@               '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.               @@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@                .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                `@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@.                 :@@@@@@@@@@@@@@@@@@@@@@@@@@@@.                 +@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@`                   `+@@@@@@@@@@@@@@@@@@@@'                    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.                       ,'@@@@@@@+:.                       ;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                       `@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#                                                    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+                                                @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#                                            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.                                      :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@`                                ,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'                         `+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'.              .+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
)EOF");
}
void GK::Splash()
{
    lcd->PrintLogo();
    //Look like we're doing something important
    vTaskDelay(500/portTICK_RATE_MS);
}
