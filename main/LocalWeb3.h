
#pragma once
#ifndef _LocalWeb3_H
#define _LocalWeb3_H

#include <string>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <sstream>
#include <Contract.h>
#include <Web3.h>
#include <Util.h>
#include <iomanip>
#include "ESPHTTPClient.h"
#include <cJSON.h>

class LocalWeb3
{
public:
  LocalWeb3(string host_ip, int host_port, Contract *contract);
  string SendTransaction(uint32_t nonceVal, unsigned long long gasPriceVal, uint32_t gasLimitVal, string *toStr, string *valueStr, string *dataStr);
  int EthGetTransactionCount(const string *address);

//   std::string NativeHTTP(const char *rawTx);
};

#endif
