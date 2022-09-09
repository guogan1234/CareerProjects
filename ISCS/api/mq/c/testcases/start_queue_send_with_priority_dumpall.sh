#!/bin/sh
#

tqname=
echo -n "Enter a value for queue name, default name is[queue://domain:test_queue_recv_pri]: "
read tqname

if [ -z "$tqname" ]; then
    tqname="queue://domain:test_queue_recv_pri"
fi

./mqtc 6 "$tqname"
