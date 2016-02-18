#! /bin/bash

rm -rf System_Init
rm -rf *~
gcc *.c -o System_Init
./System_Init
#dmesg |grep vdma
