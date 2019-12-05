#include "StateStore.h"
#include "ConnextAPI.h"
#include <unordered_map>
#include <sstream>
#include "LCD.h"
#include "FlashStorage.h"
#include <algorithm>
#include <map>


#include "FreeRTOS.h"

#define SignAllPositiveStates true
#define SignAllNegativeStates true


LCD *alcd;
longState_t nullChannel = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "", ""};

int stateIndex = 1;
int vHSig = 0;



vector<string> asyncState;
vector<hubUpdate_t> aupdates;

vector<string> pendingPayments;

string hAddress = "0xfb482f8f779fd96a857f1486471524808b97452d";

std::map<longState_t, std::string> stateHashes;
std::map<std::string, longState_t> hashesState;

longState_t *latestHubChannel;


longState_t *latestChannel;

StateStore::StateStore(string userAddress, ConnextAPI *connext, int pollLambda)
{
    this->aconnext = connext;
    this->Init(userAddress, pollLambda);
    // StateStore::Start();
}

void StateStore::AuthWithHub(bool retry)
{
	Serial.println("Auth With Hub");
    while (hubAuthed == false)
    {
        aconnext->AuthorizeWithHub();
        bool isAuthed = (bool)aconnext->GetStatus().c_str();
        if (isAuthed)
        {
            Serial.println((String) "is authed?\n" + isAuthed);
//            alcd->Print("Authed With Hub!", none);
            hubAuthed = true;
        }
        if (retry)
        {
            delay(500);
        }
        else
        {
            return;
        }
    }
}

longState_t* StateStore::GetLatestHubChannel(){
    return latestChannel;

}

void StateStore::FlashToMap()
{
    //Puts map of <longState_t: state, string:HashOf(state)> from Permanant Storage & for Quick access later (because hashing things is expensive op) AFTER THE STORAGE VECTORS GET MOUNTED

    //todo: hubSigs are +1 off of state indexes
    for (auto &state : storage->states)
    {
        // longState_t *tempState;
        //Set temp to whats inside of state vector
        // tempState = &state;
        //hash it
        string hash = aconnext->GenerateLongSateHash(&state, &user, &user);
        //store in map

        //sig to check
        string recAddr;
        string hSig;
        try
        {
            hSig = storage->hubSigs.at(stateIndex);
            recAddr = aconnext->ECRecoverState(&state, hSig);
            Serial.println((String) "Rcovered\n" + recAddr.c_str());

        }catch(...){
            Serial.println((String)"HSig @ fs vec index " + stateIndex + "dosnen't exist");
        }


        if (recAddr == hAddress)
        {
            //Hub Sig Matches State
            Serial.println("State Matches Sig Stored");
            state.sigHub = hSig;
            vHSig++;
        }

        //Push State to Map regardless of sig or naw
        hashesState.insert(std::make_pair(hash, state));
        stateIndex++;



        //Set Global latest state
        *latestChannel = hashesState.at(hash);
        Serial.printf("Latest Global State: %llu" , latestChannel->txCountGlobal);
    }

    Serial.println((String) "Added " + hashesState.size() + " State(s) to map with "+ vHSig + " validated Hub Signatures");

}

void StateStore::PrintBalance(longState_t state)
{
    //should point to current global agreed state
    std::string number;
    std::stringstream strstream;
    strstream << state.weiUser;
    strstream >> number;
//    alcd->Print((String) "Current Balance\n" + number.c_str(), wink);
}

void StateStore::Init(string userAddress, int pollLambda)
{
    recipient = user;
    pollMs = pollLambda;
    user = userAddress;


    storage = new FlashStorage();

    this->AuthWithHub();
    this->FlashToMap();

    if(this->GetLatestHubChannel()){
        //effectively start from scratch
        Serial.println("Latest Channel is Null");
        latestHubChannel = &nullChannel;
        asyncState = aconnext->Sync(0);
        aupdates = ConnextAPI::ParseUpdateFromHub(asyncState);
    }
    asyncState = aconnext->Sync(latestHubChannel->txCountGlobal);
    aupdates = ConnextAPI::ParseUpdateFromHub(asyncState);

    //change for purchases etc.
}
// TaskHandle_t PollChannelTask;
string StateStore::DepositRequest(string aToken, string aWei)
{
    signedDepositReq_t signedDep = aconnext->CreateUserDeposit(aToken, aWei);
    string res = aconnext->SendRequestDeposit(signedDep, latestHubChannel->txCountGlobal, 0);
    //sketchy ThreadId
    return res;
}
void StateStore::ConfirmPending(int updateId)
{

    Serial.println((String) "AUpdates is " + aupdates.size() + " long");

    this->RefreshState();

    hubUpdate_t updateOne = aupdates.at(updateId);


    Serial.printf("%llu", latestHubChannel->weiUser);


    string stateHash = aconnext->GenerateLongSateHash(latestHubChannel, &user, &user);

    string signedZeroState = aconnext->SignState(stateHash);

    //  Serial.println((String)zeroHash.c_str() + "\n" + signedZeroState.c_str());


    Serial.println((String) "Update one" + updateOne.args.depositTokenUser);
    updateOne.sigUser = (char *)signedZeroState.c_str();

    Serial.println("Pushing Update");
    aconnext->Update(updateId, &updateOne);
    //********************************************************//
}

void StateStore::RefreshState(){
    *latestHubChannel = aconnext->GetChannelByUserLong("0x2925fd5333bb4c23edfeeb75dea316c31adc9c36");
    asyncState = aconnext->Sync(latestHubChannel->txCountGlobal);
    aupdates = ConnextAPI::ParseUpdateFromHub(asyncState);
}

void StateStore::SendUpdate(hubUpdate_t *latestUpdate, int updateID){


    state_t stateOne = aconnext->UpdateToState(latestUpdate);

    string stateHashOne = aconnext->GenerateSateHash(stateOne);

    string signedZeroState = aconnext->SignState(stateHashOne);

    Serial.println((String)stateHashOne.c_str() + "\n" + signedZeroState.c_str());

    latestUpdate->sigUser = (char *)signedZeroState.c_str();
    //Maybe no
    aconnext->Update(updateID, latestUpdate);
}

void StateStore::DoDeviceDeposit(string aToken, string aWei)
{

    string depRes = this->DepositRequest(aToken, aWei);
    Serial.println((String) "Request Deposit\n" + depRes.c_str());
    delay(3000);


    this->RefreshState();
    string hChannelHash = aconnext->GenerateLongSateHash(latestHubChannel, &user, &user);

    // //Sign that fucker.
    string signedStateOne = aconnext->SignState(hChannelHash);

    int syncSize = asyncState.size();

    // // //Maybe off by one
    hubUpdate_t latestUpdate = aupdates.at(0);

    this->SendUpdate(&latestUpdate,0);

    *latestHubChannel = aconnext->GetChannelByUserLong("0x2925fd5333bb4c23edfeeb75dea316c31adc9c36");

}

void StateStore::InititalDeviceDeposit(string aTokens, string aWei){
    //**************************************************
    // //Step One
    Serial.println("Doing Initial Deposit");
    this->DoDeviceDeposit(aTokens, aWei);
    vTaskDelay(1000);

    this->RefreshState();

    hubUpdate_t updateOne = aupdates.at(0);

    Serial.printf("UpdateOne(in Deposit) HSig: %s", updateOne.sigHub);


    //See what happens?
    longState_t stateTwo = aconnext->UpdateToLongState(updateOne, *latestHubChannel);

    this->RefreshState();

    std::string sig = latestHubChannel->sigHub;

    Serial.printf("State Getting passed to UAU()\n User Deposit Wei: %llu \nUser Deposit Tokens: %llu", stateTwo.pendingDepositUser, stateTwo.pendingTokenDepositUser);

    //From PendingDeposit State Hub Sig

    string res = aconnext->UserAuthorizedUpdate(&stateTwo, sig);

    Serial.printf("Sent TX to the blockchain, result:\n, %s", res.c_str());

    vTaskDelay(30000);
    this->RefreshState();

    this->ConfirmPending(2);

}

std::string uAddy = "0x2925fd5333bb4c23edfeeb75dea316c31adc9c36";

void StateStore::Collateralize(){
    Serial.printf("Correct StateThree:\n txGlobal %llu \n txChain %llu", latestHubChannel->txCountGlobal, latestHubChannel->txCountChain);
    delay(5000);
    aconnext->RequestCollateral(uAddy, 3);
    delay(3000);
    this->RefreshState();

    int upSize = aupdates.size();
    Serial.println("Doing Collateralize");
    hubUpdate_t collateralUpdate = aupdates.at(upSize - 1);
    Serial.println(collateralUpdate.args.depositTokenHub);

    longState_t stateThree = aconnext->UpdateToLongState(collateralUpdate, *latestHubChannel);
     Serial.printf("Correct StateThree:\n txGlobal %llu \n txChain %llu", stateThree.txCountGlobal, stateThree.txCountChain);

    stateThree.txCountGlobal = 3;
    stateThree.txCountChain = 2;

    Serial.printf("The Long Sate's timeout: %llu", stateThree.pendingTokenDepositHub);
    std::string stateThreeHash = aconnext->GenerateLongSateHash(&stateThree, &uAddy, &uAddy);

    std::string signedSateThree = aconnext->SignState(stateThreeHash);

    Serial.println((String)stateThreeHash.c_str() + "\n" + signedSateThree.c_str());

    collateralUpdate.sigUser = (char *)signedSateThree.c_str();
    collateralUpdate.txCount = 3;
    //Maybe no
    delay(3000);
    aconnext->Update(2, &collateralUpdate);
}
void StateStore::ConfirmCollateralize(){
    // delay(5000);
    Serial.printf("Starting Confirm Collateralize");
    this->RefreshState();
    int upSize = aupdates.size();
    hubUpdate_t latestUpdate = aupdates.at(2);

    Serial.printf("Size %i TH: %s\n TxCount: \n", upSize, latestUpdate.args.depositTokenHub);

    longState_t stateFour = aconnext->UpdateToLongState(latestUpdate);

    // stateFour.txCountGlobal = 3;

    stateFour.txCountChain = 2;

    stateFour.tokenUser = 10000000;


    string stateFourHash = aconnext->GenerateLongSateHash(&stateFour, &uAddy, &uAddy);

    string signedStateFour = aconnext->SignState(stateFourHash);

    Serial.printf("Hash, %s", signedStateFour.c_str());


    latestUpdate.sigUser = (char *)signedStateFour.c_str();
    latestUpdate.txCount = 4;
    //
    Serial.println("Confirming Collateral from Hub");
    aconnext->Update(4, &latestUpdate);
}
void StateStore::Start()
{
    this->PollChannel();
}

void StateStore::Stop(){
    this->poll = false;
    Serial.printf("\n\n\npoll variable, %d", int(poll));
}

bool StateStore::CalcStateDelta(longState_t  *newState){

    //Does incoming channel add more to user bal?
    bool isWeiPositive = ((newState->weiUser > latestChannel->weiUser) ? true : false);

    bool isTokenPositive = ((newState->tokenUser > latestChannel->tokenUser) ? true : false);

    bool isPositive =  (isWeiPositive || isTokenPositive);

    if (SignAllNegativeStates){
        return true;
    } else if (isPositive){
        return true;
    }else{
        return false;
    }
}

    void StateStore::HandleNewChannel(longState_t state, string stateHash){
        string sigHub;
        string sigUser;

        string recHub;
        string recUser;

        //verify hubSig if exists & store
        if (!state.sigHub.empty())
        {
            sigHub = state.sigHub;
            Serial.println((String) "Got Sig Hub\n" + sigHub.c_str());

            recHub = aconnext->ECRecoverState(&state, sigHub);

            Serial.println((String) "Recovered Hub Address:\n" + "\n" + "\n" + recHub.c_str());

            if(recHub == hAddress){
            Serial.println("Adding Valid Hub Sig");

            storage->PutHubSig(sigHub);

            Serial.println((String)"Latest SigHub" + storage->hubSigs.back().c_str());

            }else{
                Serial.println("This is not the hub sig you're looking for");
            }

            //if(recHub == hubAddress)
        }
        //verify userSig if exists
        if (!state.sigUser.empty())
        {
            sigUser = state.sigUser;
            Serial.println((String) "Got Sig User\n" + sigUser.c_str());

            recUser = aconnext->ECRecoverState(&state, sigUser);

            Serial.println((String) "Recovered User Address:\n" + "\n" + "\n" + recUser.c_str());
        }else{
            Serial.println("Bad Hub sig");
            state.sigHub = "";
        }
    //may not need to check this, user can sign whatever tf they want
    if (!state.sigUser.empty())
    {
        Serial.println((String) "Got Sig User\n:" + state.sigUser.c_str() + "*");
    }
    //save channel with or without hubSig
    //****WORKS****************
    storage->PutState(state);
    //*************************

    //Should User Sign New State?
    bool shouldSign = StateStore::CalcStateDelta(&state);

    if(shouldSign){
        Serial.println("Device Signing State");
        string vHash = aconnext->GenerateLongSateHash(&state, &user, &user);
        aconnext->SignState(vHash);
    }
}


void StateStore::PollChannel()
{
    int numAsks = 0;
    for(;;)
    {
        Serial.printf("Poll is: \n\n\n\n, %i", int(poll));

        *latestHubChannel = aconnext->GetChannelByUserLong("0x2925fd5333bb4c23edfeeb75dea316c31adc9c36");
        string stateHash = aconnext->GenerateLongSateHash(latestHubChannel, &user, &user);

        if (hashesState.find(stateHash) == hashesState.end())
        {
            try
            {

                Serial.println("Didnt find that State, adding");
                StateStore::PrintBalance(*latestHubChannel);

                hashesState.insert(std::make_pair(stateHash, *latestHubChannel));

                Serial.println("Getting Payment History");
                aconnext->PaymentHistory();

                Serial.println("Getting Sync");
                aconnext->Sync(latestHubChannel->txCountGlobal + 1);

                delay(3000);
                //TODO: ENABLE
                //    StateStore::HandleNewChannel(*latestHubChannel, stateHash);

                numAsks++;
            }
            catch (...)
            {
                Serial.println("Hub probably dosen't like being asked alot");
                throw;
            }
        }
        else
        {
            Serial.println("We already have that state");
            numAsks++;
            Serial.println(numAsks);
            //handleChannel
        }

        Serial.println((String) "Free heap left\n" + ESP.getFreeHeap());
        if(numAsks == 0){


        }
        vTaskDelay(500/portTICK_RATE_MS);
    }
}

StateStore::~StateStore(){
    delete this;
}
