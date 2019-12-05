#pragma once

#ifndef FlashStorage_H
#define FlashStorage_H

#include <SPIFFS.h>
#include <FS.h>
#include "ConnextAPI.h"
#include <string>
#include <vector>

struct rawFile
{
  uint8_t* data;
  size_t size;
};

class FlashStorage
{
    public:
      FlashStorage();
      string GetHubSig(int index);
      string GetUserSig(int index);


      longState_t GetState(int);
      bool PutState(int i, longState_t value);
      bool PutHubSig(int i, string value);
      bool PutUserSig(int i, string value);

      void PutHubSig(string sig);
      void PutIndex(int index);
      void PutState(longState_t state);


      vector<string> hubSigs;
      vector<longState_t> states;


    private:
      bool PutIndex(int i, int value);

      bool InitUserSigs();
      bool InitHubSigs();
      bool InitStates();
      bool InitIndices();
      void PopulateStorage();
      void FirstRun();
      void Init();
      bool MountOrFormat();


      rawFile GetIndex(int i);
      void WriteFile(const char *path, rawFile);
      rawFile ReadFile(const char *path);


      vector<string> userSigs;

      bool FORMAT_ON_FAILED_MOUNT = true;
      bool partitionMounted;

      //Gets Head of the 3 different stores
      int stateIndex;
      int hubSigIndex;
      int userSigIndex;

      //0 = states 1 = hubSigs 2 = userSigs
      int indices[3];

      const char *stateRoot = "/states/";
      const char *hubSigRoot = "/hubSigs/";
      const char *userSigRoot = "/userSigs/";
      const char *indicesRoot = "/index/";
};

#endif
