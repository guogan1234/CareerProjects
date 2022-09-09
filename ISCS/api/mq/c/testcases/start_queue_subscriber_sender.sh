#!/bin/sh
#

tqname=
echo -n "Enter a value for queue name, default name is[queue://domain:test_queue]: "
read tqname

if [ -z "$tqname" ]; then
    tqname="queue://domain:test_queue"
fi

payload=
echo -n "Enter a message to be sent, default message is[test message hello]: "
read payload

if [ -z "$payload" ]; then
    payload="test message hello"
fi

./mqtc 1 "$tqname" "$payload"
