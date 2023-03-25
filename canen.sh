#!/bin/bash

INTERFACE=""
BITRATE=1000000

usage() {
    echo "Usage $0 -i INTERFACE -b BITRATE[default:1000000] -h help"
    echo "Use: ip -s -d link show to see wich can interface have ON"
    exit 2
}

while getopts "i:b:h" arg; do
    case "$arg" in
        i) INTERFACE="$OPTARG"
            ;;
        b) BITRATE="$OPTARG"
            ;;
        h) usage
            ;;
        *) usage
            ;;
    esac
done

if [ -z "$INTERFACE" ]; then
    usage
fi

sudo ip link set $INTERFACE up type can bitrate $BITRATE
ip -s -d link show $INTERFACE