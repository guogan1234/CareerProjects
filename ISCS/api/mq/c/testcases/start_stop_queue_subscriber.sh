#!/bin/sh
#

tqname=
echo -n "Enter a value for queue name, default name is[queue://domain:test_queue]: "
read tqname

if [ -z "$tqname" ]; then
    tqname="queue://domain:test_queue"
fi

./mqtc 3 "$tqname"
