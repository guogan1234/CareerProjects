#!/bin/sh
#

tqname=
echo -n "Enter a value for queue name, default name is[queue://domain:test_queue_recv]: "
read tqname

if [ -z "$tqname" ]; then
    tqname="queue://domain:test_queue_recv"
fi

./mqtc 2 "$tqname"
