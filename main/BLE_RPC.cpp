#include "BLE_RPC.h"

	void testFn() {
		Serial.println("testFN/n/n/n/");
	};

	BLE_RPC::BLE_RPC(){
		this->commandMap["Test"] = &testFn;
	}


	void BLE_RPC::sendNFTMetaTx(std::string address){

	}

	//returns stripped RPC Command if any
	std::string BLE_RPC::isValidRPCPreamble(std::string fullRx){
			size_t  commandPos = fullRx.find(this->RPC_COMMAND_PREAMBLE);
			Serial.println(commandPos);
			std::string command;
			if(commandPos != std::string::npos){
				Serial.println("Recieved An RPC Command");
				command = fullRx.substr(commandPos + RPC_COMMAND_PREAMBLE.length(),fullRx.length());
			}
			return command;
		}

	void BLE_RPC::execCommand(std::string command){
		auto iter = this->commandMap.find(command);
		if(iter != commandMap.end()){
//			(*iter)();
			Serial.println("We have that command");
			//todo:now execute :|
		}

	}

	void BLE_RPC::setRX(std::string fullRx){
		std::string command = this->isValidRPCPreamble(fullRx);
		this->execCommand(command);
}



//	 bool isValidRPCPreamble(){
//			  if (rxValue.find(RPC_COMMAND_PREAMBLE) != std::string::npos){
//
//			  	  return true;
//	            } else{
//	  	          return false;
//	            }
//
//		  }




//	std::string BLE_RPC::recoverAdddressFromSig(std::string sig){
//
//
//	}


