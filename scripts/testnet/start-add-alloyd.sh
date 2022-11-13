#!/usr/bin/env bash

CWD=$(dirname $0)

walletname=alloy
#read -s -p "WALLET: " walletname 
#read -s -p "PASSWORD: " password 
read -s "testnet host: " hostname 
echo ""

WALLET_ADDRESS=$(cat $CWD/${walletname}.address)

echo "Wallet Address ${WALLET_ADDRESS}"

$CWD/../../build/src/alloyd --log-level=4 --hide-my-port --testnet --data-dir=$CWD/.alloy-testnet --fee-address=${WALLET_ADDRESS} --genesis-block-reward-address=${WALLET_ADDRESS} --rpc-bind-port=1911 --p2p-bind-port=1910 --add-exclusive-node=${hostname}:1910 $*

