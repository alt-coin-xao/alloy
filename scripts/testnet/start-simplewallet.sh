#!/usr/bin/env bash

CWD=$(dirname $0)

${CWD}/../../build/src/simplewallet --testnet --daemon-address localhost:1911 $*
