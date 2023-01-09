#!/usr/bin/env bash

CWD=$(dirname $0)

WALLET_ADDRESS=A4Zdbf7pzpFTkDUX63W8gk2S9ESjnWsQUPNTdD5zdAub8SJqf4u9zczCzbLsN8oawpM8mJt3745Af9jcXnmF66LG6LAVmFE
echo "$1"
curl --location --request POST "http://$1:9070/json_rpc" --header "Content-Type: application/json" --data-raw "{ \"jsonrpc\": \"2.0\", \"id\": \"0\", \"method\": \"getBalance\", \"params\": { \"address\": \"${WALLET_ADDRESS}\" } }"
echo ""
