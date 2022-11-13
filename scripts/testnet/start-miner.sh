#!/usr/bin/env bash

CWD=$(dirname $0)

walletname=alloy
#read -s -p "WALLET: " walletname 
#read -s -p "PASSWORD: " password 

WALLET_ADDRESS=$(cat $CWD/${walletname}.address)

echo "Wallet Address ${WALLET_ADDRESS}"

${CWD}/../../build/src/miner --address ${WALLET_ADDRESS} --daemon-address localhost:1911 
