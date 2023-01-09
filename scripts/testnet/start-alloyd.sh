#!/usr/bin/env bash

CWD=$(dirname $0)
WALLET_ADDRESS=A4Zdbf7pzpFTkDUX63W8gk2S9ESjnWsQUPNTdD5zdAub8SJqf4u9zczCzbLsN8oawpM8mJt3745Af9jcXnmF66LG6LAVmFE

$CWD/../../build/src/alloyd --testnet --fee-address ${WALLET_ADDRESS} --genesis-block-reward-address ${WALLET_ADDRESS} --rpc-bind-port 1911 --p2p-bind-port 1910 
