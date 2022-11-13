#!/usr/bin/env bash

CWD=$(dirname $0)

walletname=alloy
#read -s "WALLET: " walletname 
#read -s -p "PASSWORD: " password 

WALLET_ADDRESS=$(cat $CWD/${walletname}.address)

echo "Wallet Address ${WALLET_ADDRESS}"

$CWD/../../build/src/alloyd --testnet --fee-address=${WALLET_ADDRESS} --genesis-block-reward-address=${WALLET_ADDRESS} --rpc-bind-port=1911 --p2p-bind-port=1910 $* 
