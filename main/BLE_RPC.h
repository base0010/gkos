enum rpc_command {
	mintNFT=0,
	doSomethingElse=1,
};

enum rpc_mintStatus{
	error=0,
	step1=1,
	step2=2,
};


struct rpc_call {
	rpc_command command;
	int status;
	int timeStamp=0;
};


