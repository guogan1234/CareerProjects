#!/bin/sh
#

tqname=
echo -n "Enter a value for queue name, default name is[queue://domain:test_queue_recv_pri]: "
read tqname

if [ -z "$tqname" ]; then
    tqname="queue://domain:test_queue_recv_pri"
fi

priority=
echo -n "Enter a value for priority [0-9], default is[0]: "
read priority

if [ -z "$priority" ]; then
    priority="0"
fi

payload=
echo -n "Enter a message to be sent, default message is[queue test message hello]: "
read payload

if [ -z "$payload" ]; then
    payload="queue test message hello"
fi

./mqtc 5 "$tqname" "$payload" "$priority"
