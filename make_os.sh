#!/bin/bash 

echo "Building GKOS" 

if ["$0" == "clean"]; then 
	echo "Cleaning before building"
	make clean
fi

make -j8 flash && make monitor
