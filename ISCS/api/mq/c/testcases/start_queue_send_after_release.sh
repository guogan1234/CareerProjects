#!/bin/sh
#

tqname=
echo -n "Enter a value for queue name, default name is[queue://domain:test_queue_recv]: "
read tqname

if [ -z "$tqname" ]; then
    tqname="queue://domain:test_queue_recv"
fi

payload=
echo -n "Enter a message to be sent, default message is[queue test message hello]: "
read payload

if [ -z "$payload" ]; then
    payload="test message hello"
fi

./mqtc 4 "$tqname" "$payload"
