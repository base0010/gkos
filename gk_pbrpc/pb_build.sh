#!/bin/bash

export PROTOC_PBNANO="~/Development/nanopb-0.3.9.4-linux-x86/"; 

echo "set protoc to hardcoded directory "; 
echo "${PROTOC_PBNANO}"; 

exec make; 
#exec "${PROTOC_PBNANO}/generator-bin/protoc" --nanopb_out=./nanopb_cpp/ ./protos/*.proto; 
echo "compiled .proto files" ; 

