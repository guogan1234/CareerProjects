ECHO OFF
SET tqname=
SET /P tqname=Enter a value for topic name, default name is[topic://domain:test_topic]:
IF [%tqname%]==[] SET tqname=topic://domain:test_topic

SET payload=
SET /P payload=Enter a message to be sent, default message is[test message hello]:
IF "%payload%"=="" SET payload=test message hello

mqtc 1 "%tqname%" "%payload%"