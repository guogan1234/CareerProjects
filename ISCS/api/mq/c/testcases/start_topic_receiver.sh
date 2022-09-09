#!/bin/sh
#

tqname=
echo -n "Enter a value for topic name, default name is[topic://domain:test_topic_recv]: "
read tqname

if [ -z "$tqname" ]; then
    tqname="topic://domain:test_topic_recv"
fi

./mqtc 2 "$tqname"
