#!/usr/bin/env bash

CWD=$(dirname $0)

#walletname=alloy
read -p "WALLET: " walletname 
read -p "HOST: " hostname 
#read -s -p "PASSWORD: " password 

WALLET_ADDRESS=$(cat $CWD/${walletname}.address)

echo "Wallet Address ${WALLET_ADDRESS}"

curl --location --request POST "http://${hostname}:9070/json_rpc" --header "Content-Type: application/json" --data-raw "{ \"jsonrpc\": \"2.0\", \"id\": \"0\", \"method\": \"getBalance\", \"params\": { \"address\": \"${WALLET_ADDRESS}\" } }"
echo ""
