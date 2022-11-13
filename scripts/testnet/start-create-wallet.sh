#!/usr/bin/env bash

CWD=$(dirname $0)

#walletname=alloy
#read -s -p "WALLET: " walletname 
#read -s -p "PASSWORD: " password 

${CWD}/../../build/src/simplewallet --testnet --daemon-address localhost:1911 

cp $CWD/test.wallet $CWD/testd.wallet
