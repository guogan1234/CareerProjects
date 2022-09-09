ECHO OFF
SET tqname=
SET /P tqname=Enter a value for topic name, default name is[topic://domain:test_topic_recv]:
IF [%tqname%]==[] SET tqname=topic://domain:test_topic_recv
mqtc 2 "%tqname%"