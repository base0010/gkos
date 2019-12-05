//
// Created by Okada, Takahiro on 2018/02/05.
//

#ifndef ARDUINO_WEB3_CONTRACT_H
#define ARDUINO_WEB3_CONTRACT_H

#include "Arduino.h"
#include "Log.h"
#include "Web3.h"
#include <vector>
#include <Crypto.h>

using namespace std;

class Contract {

public:
    typedef struct {
        char from[80];
        char to[80];
        char gasPrice[20];
        long gas;
    } Options;
    Options options;

public:
    Contract(Web3* _web3, const char* address);
    void SetPrivateKey(const char *key);
    string SetupContractData(const char* func, ...);
    string Call(const string* param);
    string ViewCall(const string *param);
    string SendTransaction(uint32_t nonceVal, unsigned long long gasPriceVal, uint32_t gasLimitVal,
                           string *toStr, string *valueStr, string *dataStr);
    string GenerateContractBytes(const char *func);
    string GenerateBytesForInt(const int32_t value);
    string GenerateBytesForUint(const uint32_t value);
    string GenerateBytesForAddress(const string *value);
    string GenerateBytesForString(const string *value);
    void GenerateSignature(uint8_t *signature, int *recid, uint32_t nonceVal, unsigned long long gasPriceVal, uint32_t gasLimitVal,
                           string *toStr, string *valueStr, string *dataStr);
    vector<uint8_t> RlpEncodeForRawTransaction(
        uint32_t nonceVal, unsigned long long gasPriceVal, uint32_t gasLimitVal,
        string *toStr, string *valueStr, string *dataStr, uint8_t *sig, uint8_t recid);

    string GenerateBytesForBytes(const char *value, const int len);
    

  private:
    Log Debug;
    #define LOG(x) Debug.println(x)

    Web3* web3;
    const char * contractAddress;
    Crypto* crypto;

private:

    string GenerateBytesForUIntArray(const vector<uint32_t> *v);

    
    vector<uint8_t> RlpEncode(
            uint32_t nonceVal, unsigned long long gasPriceVal, uint32_t  gasLimitVal,
            string* toStr, string* valueStr, string* dataStr);
    
    void Sign(uint8_t* hash, uint8_t* sig, int* recid);
};


#endif //ARDUINO_WEB3_CONTRACT_H
