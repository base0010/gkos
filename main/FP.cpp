#include "FP.h"


FP::FP(std::shared_ptr<FPBluetooth> bleInterface){

	ble = bleInterface;
	this->ASCIISplash();
	this->startCommands();
}

void FP::InitHW(){
    this->Splash();
    this->InitRadios();
}

void FP::InitRadios(){
}

void FP::startCommandsImpl(void *_this)
{
    static_cast<FP *>(_this)->PollCommands();
}

void FP::startCommands()
{
	TaskHandle_t someTask = NULL;
    xTaskCreateUniversal(this->startCommandsImpl, "PollCommands", 5000, this, 1, &someTask, 1);
}


void FP::PollCommands()
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
//            this->startInitialDeposit();
        	Serial.println("Got Command from APP");
            commandIsRunning = true;
        }
        else if (latestCommand.compare("pollChannel") == 0)
        {
//            this->startPollingChannel();
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

void FP::ASCIISplash(){
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
void FP::Splash()
{
//    lcd->PrintLogo();
    //Look like we're doing something important
    vTaskDelay(500/portTICK_RATE_MS);
}
