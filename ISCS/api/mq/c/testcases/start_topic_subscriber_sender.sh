#!/bin/sh
#

tqname=
echo -n "Enter a value for topic name, default name is[topic://domain:test_topic]: "
read tqname

if [ -z "$tqname" ]; then
    tqname="topic://domain:test_topic"
fi

payload=
echo -n "Enter a message to be sent, default message is[test message hello]: "
read payload

if [ -z "$payload" ]; then
    payload="test message hello"
fi

./mqtc 1 "$tqname" "$payload"
