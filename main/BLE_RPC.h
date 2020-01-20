#pragma once
#ifndef BLE_RPC_H
#define BLE_RPC_H

#include <unordered_map>
#include <string>
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
	std::unordered_map <std::string,rpc_command>commandMap;
private:
	std::string txData;
	std::string rxData;
	void sendNFTMetaTx(std::string address);
	void getCommand(std::string command);

public:
	BLE_RPC();
	void setTX(std::string tx);
	void setRX(std::string rx);
};

#endif
