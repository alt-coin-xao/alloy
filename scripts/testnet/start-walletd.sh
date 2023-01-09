#!/usr/bin/env bash

CWD=$(dirname $0)

WALLET_ADDRESS=A4Zdbf7pzpFTkDUX63W8gk2S9ESjnWsQUPNTdD5zdAub8SJqf4u9zczCzbLsN8oawpM8mJt3745Af9jcXnmF66LG6LAVmFE

${CWD}/../../build/src/walletd --testnet --bind-address 0.0.0.0 --bind-port 9070 --daemon-address 127.0.0.1 --daemon-port 1911 -w ./test.wallet -p test
