#pragma once
#ifndef BLE_RPC_H
#define BLE_RPC_H

#include <unordered_map>
#include <string>
#include <functional>
#include <Arduino.h>




enum rpc_command {
	mintNFT=0,
	doSomethingElse=1,
};

enum rpc_mintStatus{
	error=0,
	step1=1,
	step2=2,
};


struct rpc_call {
	rpc_command command;
	int status;
	int timeStamp=0;
};


//data from the tx & rx attributes

class BLE_RPC{


protected:
	//std::any in C++17 would let us store functions with arbitrary signatures.
	std::unordered_map <std::string,std::function<void()>>commandMap;
	std::string command;
	std::string isValidRPCPreamble(std::string fullRx);
	std::string RPC_COMMAND_PREAMBLE = "GK_RPC:";

	void execCommand(std::string command);

private:
	std::string txData;
	std::string rxData;
	void sendNFTMetaTx(std::string address);
	void e(std::string command);


public:
	BLE_RPC();
	void setTX(std::string tx);
	void setRX(std::string rx);
};

#endif
