#!/bin/bash

INTERFACE=""
SIM=""
BITRATE=1000000

usage() {
    echo "Usage $0 -i INTERFACE -b BITRATE[default:1000000] -h help"
    echo "Use: ip -s -d link show to see wich can interface have ON"
    echo "Use $0 -s <n> to activate vcan0"
    exit 2
}

while getopts "i:b:s:h" arg; do
    case "$arg" in
        i) INTERFACE="$OPTARG"
            ;;
        b) BITRATE="$OPTARG"
            ;;
        s) SIM="vcan$OPTARG"
            ;;
        h) usage
            ;;
        *) usage
            ;;
    esac
done

if [ -z "$INTERFACE" -a -z "$SIM" ]; then
    usage
fi

if [ -n "$SIM" ]; then 
    sudo modprobe vcan
    sudo ip link add dev "$SIM" type vcan
    sudo ip link set "$SIM" up
    exit 0
fi

sudo ip link set $INTERFACE up type can bitrate $BITRATE
ip -s -d link show $INTERFACE