#pragma once
#ifndef FPWeb3_H
#define FPWeb3_H

#include <string>
#include <Arduino.h>
#include <Util.h>
#include <Crypto.h>
#include <Contract.h>
#include <Web3.h>



class FPWeb3 {
	private:
		Web3 *web3;
		Crypto *crypto;

	public:
		FPWeb3();
		string challengeNonce;
		string challengeHash;
		string recoveredAddress;


		string recoverAddressFromChallenge(string signedChallenge);



};

#endif
