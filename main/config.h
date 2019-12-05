
#pragma once
#ifndef _CONFIG_H
#define _CONFIG_H

#include <string>

// socat tcp-listen:8081,reuseaddr,fork tcp:localhost:8080
// /socat tcp-listen:8000,reuseaddr,fork tcp:localhost:8545
//0xb7a525847d25cbe89b875a91dbacab4f3adf8dd870b44ba4cf2918e2ec242fce
// extern std::string ssid = "Cats";
// extern std::string pass = "Ilovethebeach!";
// //****Nancys
// extern std::string host_ip = "10.0.0.152";
// extern std::string host = "http://10.0.0.152";

// //*****Hotspot
extern std::string host_ip;
extern std::string host;
//**Ports & Addresses
extern std::string url;
extern std::string web3url;
extern std::string userAddress;
extern std::string contractAddress;

extern std::string hubContractAddress;
extern int web3_port;

extern unsigned long long gasPriceVal;
extern uint32_t gasLimitVal;

//Wallet Address
extern std::string hubAddress;

extern std::string tokenContract;

extern const char *pk;
#endif
