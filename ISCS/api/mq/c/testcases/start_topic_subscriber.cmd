ECHO OFF
SET tqname=
SET /P tqname=Enter a value for topic name, default name is[topic://domain:test_topic]:
IF [%tqname%]==[] SET tqname=topic://domain:test_topic
mqtc 0 "%tqname%"