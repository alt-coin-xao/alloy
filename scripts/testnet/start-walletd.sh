#!/usr/bin/env bash

CWD=$(dirname $0)

#walletname=alloy
read -p "WALLET: " walletname 
read -s -p "PASSWORD: " password 
echo ""

WALLET_ADDRESS=$(cat $CWD/${walletname}.address)

echo "Wallet Address ${WALLET_ADDRESS}"

${CWD}/../../build/src/walletd --testnet --bind-address 0.0.0.0 --bind-port 9070 --daemon-address 127.0.0.1 --daemon-port 1911 -w $CWD/${walletname}d.wallet -p $password
