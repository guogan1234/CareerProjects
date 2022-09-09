#! /bin/sh
#

tqname=
echo -n "Enter a value for topic name, default name is[topic://domain:test_topic]: "
read tqname

if [ -z "$tqname" ]; then
    tqname="topic://domain:test_topic"
fi

./mqtc 0 "$tqname"

