#include "FPWeb3.h"
using namespace std;


std::string ETH_PREAMBLE = "19457468657265756d205369676e6564204d6573736167653a0a3332";

FPWeb3::FPWeb3(){
	web3 = new Web3("http://nonetest.com:8545", "/");
	crypto = new Crypto(web3);
}

