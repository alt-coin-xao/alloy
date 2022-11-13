#!/usr/bin/env bash

CWD=$(dirname $0)

#walletname=alloy
read -p "WALLET: " walletname 
read -s -p "PASSWORD: " password 
echo " "

WALLET_ADDRESS=$(cat $CWD/${walletname}.address)

echo "Wallet Address ${WALLET_ADDRESS}"

${CWD}/../../build/src/simplewallet --testnet --daemon-address localhost:1911 --wallet-file $CWD/${walletname}.wallet --password $password
