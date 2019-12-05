#ifndef StateStore_H
#define StateStore_H
#include <atomic>
#include <string>
//#include "ConnextAPI.h"
#include <unordered_map>
#include "FlashStorage.h"
#include <WiFi.h>
#include <FreeRTOS.h>

using namespace std;

class StateStore
{
  public:
    StateStore(string userAddress, ConnextAPI* connext, int pollLambda = 1500);
    string recipient;
    void PrintBalance(longState_t state);

    string user;
    vector<string> asyncState;
    vector<hubUpdate_t> aupdates;

    longState_t *latestHubChannel;

    longState_t *latestChannel;

    //todo: mk private
    void Start();
    longState_t *GetLatestHubChannel();

    void DoDeviceDeposit(string aToken, string aWei);

    void ConfirmPending(int updateId);

    string DepositRequest(string aToken, string aWei);

    void RefreshState();

    void SendUpdate(hubUpdate_t *latestUpdate, int updateID);
    void InititalDeviceDeposit(string aTokens, string aWei);
    void Collateralize();

    void ConfirmCollateralize();

    void Stop();

    std::atomic<bool> poll;


    ~StateStore();

  private:
    FlashStorage* storage;
    ConnextAPI *aconnext;

    bool hubAuthed = false;

    int pollMs;

    //UserAddress

    vector<string> pendingPayments;

    void Init(string userAddress, int pollLambda);
    // std::unordered_map<std::string,longState_t> stateHashes;
    // longState_t latestHubChannel;

    void PollChannel();

    void StoreChannel(const char *data);
    void HandleNewChannel(longState_t, string);

    void FlashToMap();

    //Returns true on positive change false on negative change;
    bool CalcStateDelta(longState_t *newState);

    void AuthWithHub(bool retry = false);
    //void HandleNewUpdate();
  };

#endif //ARDUINO_WEB3_CRYP
