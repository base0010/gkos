

#include "LocalWeb3.h"
//  #include <config.h>
using namespace std;




WiFiClient noSSLclient;
HTTPClient http;

string hip;
int host_port;

Contract *acontract;

LocalWeb3::LocalWeb3(string _hip, int _hport, Contract *_contract){
    hip = _hip;
    host_port = _hport;
    acontract = _contract;

}
std::string SendTxNoSSL(const string *data){

    stringstream ss;
    ss << *data;

    Serial.printf("\nRecieved data to POST\n %s", ss.str().c_str());


    ESPHTTPClient::web3_rawTx(ss.str().c_str());
    	return "a";
}
// string SendTxNoSSL(const string *data)
// {

//     std::string result;

//     stringstream dataS;
//     dataS << *data;

//     int connected = noSSLclient.connect(hip.c_str(), host_port);
//     if (!connected)
//     {
//         Serial.println("Unable to connect to Host");
//         Serial.println(noSSLclient);
//         return "";
//     }

//     // Make a HTTP request:
//     int l = data->size();
//     stringstream ss;
//     ss << l;
//     string lstr = ss.str();

//     string strPost = "POST " + string("/") + " HTTP/1.1";

//     //todo: possible bug here
//     string strHost = "Host: " + string("url");
//     string strContentLen = "Content-Length: " + lstr;

//     noSSLclient.println(strPost.c_str());
//     noSSLclient.println(strHost.c_str());
//     noSSLclient.println("Content-Type: application/json");
//     noSSLclient.println(strContentLen.c_str());
//     noSSLclient.println("Connection: close");
//     noSSLclient.println();
//     noSSLclient.println(data->c_str());

//     while (noSSLclient.connected())
//     {

//         String line = noSSLclient.readStringUntil('\n');
//         if (line == "\r")
//         {
//             break;
//         }
//     }
//     // if there are incoming bytes available
//     // from the server, read them and print them:
//     while (noSSLclient.available())
//         {
//             char c = noSSLclient.read();
//             result += c;
//         }
//     noSSLclient.stop();

//     return result;
// }
string generateJson(const string *method, const string *params)
{
    return "{\"jsonrpc\":\"2.0\",\"method\":\"" + *method + "\",\"params\":" + *params + ",\"id\":0}";
}

string EthSendSignedTransaction(const string *data, const uint32_t dataLen)
{
    string m = "eth_sendRawTransaction";
    string p = "[\"" + *data + "\"]";
    string input = generateJson(&m, &p);
#if 0
    LOG(input);
#endif
    return SendTxNoSSL(&input);
}

#define SIGNATURE_LENGTH 64

string LocalWeb3::SendTransaction(uint32_t nonceVal, unsigned long long gasPriceVal, uint32_t gasLimitVal,
                       string *toStr, string *valueStr, string *dataStr)
{
    uint8_t signature[SIGNATURE_LENGTH];
    memset(signature, 0, SIGNATURE_LENGTH);
    int recid[1] = {0};
    acontract->GenerateSignature(signature, recid, nonceVal, gasPriceVal, gasLimitVal,toStr, valueStr, dataStr);

    vector<uint8_t> param = acontract->RlpEncodeForRawTransaction(nonceVal, gasPriceVal, gasLimitVal,
                                                                toStr, valueStr, dataStr,
                                                                signature, recid[0]);

#if 0
    Serial.println("RLP RawTrans Encode:");
    Serial.println(Util::ConvertBytesToHex(param.data(), param.size()).c_str());
#endif

    string paramStr = Util::VectorToString(param);
    return EthSendSignedTransaction(&paramStr, param.size());
}

int getInt(const string* json) {
    int ret = 0;
    cJSON *root, *value;
    root = cJSON_Parse(json->c_str());
    value = cJSON_GetObjectItem(root, "result");
    if (cJSON_IsString(value)) {
        ret = strtol(value->valuestring, nullptr, 16);
    }

    cJSON_free(root);
    return ret;
}

//TODO: Not working
int LocalWeb3::EthGetTransactionCount(const string *address)
{
    std::string m = "eth_getTransactionCount";
    std::string p = "[\"" + *address + "\",\"latest\"]";
    std::string input = generateJson(&m, &p);
    std::string output = SendTxNoSSL(&input);

    return getInt(&output);
}

    //Probably a sloppy way to manipulate JSON
//
//    int b = output.find("{");
//    int e = output.find("}");
//
//    int diff = e - b;
//    std::string retStr = output.substr(b,diff);
//
//    Serial.printf("Return String, %s", retStr.c_str());

 //   return retStr;
    //todo: uncomment below, that worked

//    std::string parsedOut = output.substr(b, diff);
//
//    //Serial.println(parsedOut.c_str());
//
//    return parsedOut;
