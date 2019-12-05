#include "FlashStorage.h"
#include <FS.h>
#include <iostream>
#include <fstream>
#include "ConnextAPI.h"
#include <string>
#include <Util.h>
#include <vector>

using namespace std;
// vector<string> hubSigs;
FlashStorage::FlashStorage()
{
    partitionMounted = FlashStorage::MountOrFormat();
    if (!partitionMounted)
    {
        Serial.println("Couldn't mount SPIFFS...hmm");
        delete (this);
    }

    Serial.println("Initalized Flash");
    FlashStorage::Init();
}

void FlashStorage::Init()
{
    if (!SPIFFS.exists((String)indicesRoot + 0))
    {
        Serial.println("Setting up Storage");
        FlashStorage::FirstRun();
    }
    FlashStorage::PopulateStorage();
}

rawFile FlashStorage::GetIndex(int i)
{
    String apath = (String)indicesRoot + i;
    rawFile index = FlashStorage::ReadFile(apath.c_str());
    return index;
}

bool FlashStorage::InitIndices()
{

    File indicesDir = SPIFFS.open(indicesRoot);
    if (indicesDir.isDirectory())
    {
        Serial.println("Index directory found");
        for (int i = 0; i <= 2; i++)
        {
            //File file = i.openNextFile();
            // int *indexData = reinterpret_cast<int *>(FlashStorage::GetIndex(i).data);
            rawFile rawBoi = FlashStorage::GetIndex(i);
            //Only works for little index values
            //int indexData = static_cast<int>(*rawBoi.data);
            int indexData = static_cast<int>(*rawBoi.data);
            // int indexData = int(rawBoi.data[0]);
            Serial.println((String) "Got index@: " + i + "\nValue: " + indexData);
            indices[i] = indexData;

            // return true;
        }
        indicesDir.close();
    }
    return true;
}

longState_t FlashStorage::GetState(int i)
{
    String apath = (String)stateRoot + i;
    rawFile rawState = FlashStorage::ReadFile(apath.c_str());
    //Serial.write(rawState.data);

    longState_t *state = reinterpret_cast<struct longState_t *>(rawState.data);
    return *state;
}

bool FlashStorage::InitStates()
{
    File statesDir = SPIFFS.open(stateRoot);
    if (statesDir.isDirectory())
    {
        Serial.println("State directory found");
        Serial.println((String) "Getting " + indices[0] + " States from Storage");

        if (indices[0] > 0)
            for (int i = 1; i <= indices[0]; i++)
            {

                longState_t state = FlashStorage::GetState(i);
                Serial.println((String) "Got state@: " + i + "\nGlobal TX State #: " + (int)state.txCountGlobal);
                states.push_back(state);
                Serial.println("Pushed State to Vec");
                // std::ostream;
                Serial.printf("WeiUser test:  %llu", state.weiUser);
            }
    }
    statesDir.close();
    bool res = states.size() == indices[0] ? true : false;
    return res;
}

void FlashStorage::PutState(longState_t state)
{

    Serial.println((String) "Attempting to Store State:\n" + int(state.txCountGlobal));

    //typename std::vector<longState_t>::iterator it;

    // it = std::find(states.begin(), states.end(), state);

    // if (it != states.end())
    // {
    //found
    //int index = std::distance(states.begin(), it);
    // Serial.println("Already Have this State:\n" + index);
    // }
    // else
    // {
    //increment hubIndex in mem
    indices[0]++;

    String apath = (String)stateRoot + indices[0];
    uint8_t *pUState = reinterpret_cast<std::uint8_t *>(&state);

    size_t pUSLen = sizeof(state) / sizeof(uint8_t);

    Serial.println((String) "We think size of PTR is: " + pUSLen);

    FlashStorage::WriteFile(apath.c_str(), rawFile{pUState, pUSLen});

    states.push_back(state);

    //update hubIndex in storage
    FlashStorage::PutIndex(0);

    Serial.println("Saved New State");
}

void FlashStorage::PutIndex(int index)
{

    Serial.println((String) "Updating Index @ " + index + "With New Value" + indices[index]);
    String apath = (String)indicesRoot + index;

    uint8_t val = indices[index];

    FlashStorage::WriteFile(apath.c_str(), rawFile{&val, sizeof(val)});
}

void FlashStorage::PutHubSig(string sig)
{

    Serial.println((String) "Attempting to write Hub Sig:\n" + sig.c_str());

    std::vector<string>::iterator it = std::find(hubSigs.begin(), hubSigs.end(), sig);

    if (it != hubSigs.end())
    {
        //found
        int index = std::distance(hubSigs.begin(), it);
        Serial.println("Already Have this Hub Sig:\n" + index);
    }
    else
    {
        //increment hubIndex in mem
        indices[1]++;

        String apath = (String)hubSigRoot + indices[1];
        uint8_t s[65];
        //sig -> uint8
        uint32_t bLen = Util::ConvertCharStrToUintArray(s, (const uint8_t *)sig.c_str());
        string out = Util::ConvertBytesToHex(s, 65);
        //write uint8(sig)
        FlashStorage::WriteFile(apath.c_str(), rawFile{s, bLen});

        hubSigs.push_back(sig);

        //update hubIndex in storage
        FlashStorage::PutIndex(1);

        Serial.println("Saved New SigHub");
    }
}

string FlashStorage::GetHubSig(int i)
{
    String apath = (String)hubSigRoot + i;

    rawFile rawState = FlashStorage::ReadFile(apath.c_str());

    string hSig = Util::ConvertBytesToHex(rawState.data, rawState.size).c_str();
    Serial.println((String) "Got HubSig @ " + i + "\nValue:" + hSig.c_str());
    return hSig;
}

string FlashStorage::GetUserSig(int i)
{
        String apath = (String)userSigRoot + i;
        rawFile rawState = FlashStorage::ReadFile(apath.c_str());
        string *sig = reinterpret_cast<string *>(rawState.data);
        return *sig;
}

    bool FlashStorage::InitHubSigs()
    {
        File hSigDir = SPIFFS.open(hubSigRoot);

        if (hSigDir.isDirectory())
        {
            Serial.println("Hub Sig directory found");
            Serial.println((String) "Getting " + indices[1] + "HSigs from Storage");
            if (indices[1] > 0)
            {
                for (int i = 0; i <= indices[1]; i++)
                {

                    string hSig = FlashStorage::GetHubSig(i);

                    Serial.println((String) "Got hSig@: " + i + "\nGlobal TX State #: ");

                    if (!hSig.empty())
                    {
                        hubSigs.push_back((string)hSig.c_str());
                        Serial.println((String) "PushedBack\n" + hubSigs.at(i).c_str());
                    }
                    // hubSigs.size();
                }
                // }
            }
        }
        return true;
    }

    bool FlashStorage::InitUserSigs()
    {
        File uSigDir = SPIFFS.open(userSigRoot);

        if (uSigDir.isDirectory())
        {
            Serial.println("User Sig directory found");
            Serial.println((String) "Getting " + indices[2] + "USigs from Storage");

            if (indices[2] > 0)
                for (int i = 0; i <= indices[2]; i++)
                {
                    //File file = i.openNextFile();

                    string uSig = FlashStorage::GetUserSig(i);
                    Serial.println((String) "Got uSig@: " + i + "\nGlobal TX State #: ");

                    userSigs.push_back(uSig);
                }
        }

        bool res = userSigs.size() == indices[2] ? true : false;
        return res;
    }

    void FlashStorage::PopulateStorage()
    {

        if (FlashStorage::InitIndices() && FlashStorage::InitStates() && FlashStorage::InitHubSigs() && FlashStorage::InitUserSigs())
        {
            Serial.println("Initalized Storage In Memory!");
        }
        else
        {
            Serial.println("Couldn't Initalize Storage In Memory");
        }
    }

    void FlashStorage::FirstRun()
    {
        SPIFFS.mkdir(indicesRoot);

        Serial.println("Couldn't find old indicies , creating new");

        String stateIndex = (String)indicesRoot + "0";
        String hsIndex = (String)indicesRoot + "1";
        String usIndex = (String)indicesRoot + "2";

        indices[0] = 0;
        indices[1] = 0;
        indices[2] = 0;

        FlashStorage::PutIndex(0);
        FlashStorage::PutIndex(1);
        FlashStorage::PutIndex(2);

        uint8_t z = 0;

        FlashStorage::WriteFile(stateIndex.c_str(), rawFile{&z, 1});
        FlashStorage::WriteFile(hsIndex.c_str(), rawFile{&z, 1});
        FlashStorage::WriteFile(usIndex.c_str(), rawFile{&z, 1});

        SPIFFS.mkdir(stateRoot);

        SPIFFS.mkdir(hubSigRoot);
        SPIFFS.mkdir(userSigRoot);
    }

    bool FlashStorage::MountOrFormat()
    {
        Serial.println("Mounting Storage");
        //return SPIFFS.begin(FORMAT_ON_FAILED_MOUNT);

        SPIFFS.format();
        SPIFFS.begin();
        return true;
    }

    rawFile FlashStorage::ReadFile(const char *path)
    {

        Serial.printf("Reading file @: %s\r\n", path);

        File file = SPIFFS.open(path, FILE_READ);
        //file.m
        size_t fSize = file.size();

        Serial.println((String) "RFile Size\n" + fSize);

        rawFile raw;
        // uint8_t readData;
        char *buffer = (char *)malloc(fSize);

        if (!file)
        {
            Serial.println("Failed to open file for rriting");
        }

        while (file.available())
        {
            size_t rSize = file.readBytes(buffer, fSize);

            Serial.println((String) "File Read @: " + path + "\nSize(b): " + rSize);

            file.close();
            //Serial.write(file.read());
        }
        file.close();
        Serial.println((String) "this is RBuf in Straight Char\n" + buffer);
        uint8_t *pBuf = reinterpret_cast<uint8_t *>(buffer);
        Serial.println(Util::ConvertBytesToHex(pBuf, fSize).c_str());
        raw.data = pBuf;
        raw.size = size_t(fSize);
        return raw;
    }

    void FlashStorage::WriteFile(const char *path, rawFile data)
    {
        Serial.printf("Writing file @: %s\r\n", path);

        File file = SPIFFS.open(path, FILE_WRITE);
        //file.m

        if (!file)
        {
            Serial.println("Failed to open file for writing");
            return;
        }

        size_t fLen = file.write(data.data, data.size);
        file.close();

        Serial.println("Reading the file we just wrote");

        rawFile rFile = FlashStorage::ReadFile(path);
        Serial.println(*rFile.data);
    }
