#!/bin/bash

export PROTOC_PBNANO="~/Development/nanopb-0.3.9.4-linux-x86/generator-bin"; 
echo "${PROTOC_PBNANO}"; 


exec "${PROTOC_PBNANO}/protoc" --nanopb_out=./nanopb_out/ ./protos/*.proto; 


