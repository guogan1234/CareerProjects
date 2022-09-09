ECHO OFF
SET tqname=
SET /P tqname=Enter a value for queue name, default name is[queue://domain:test_queue]:
IF [%tqname%]==[] SET tqname=queue://domain:test_queue

SET payload=
SET /P payload=Enter a message to be sent, default message is[test message hello]:
IF "%payload%"=="" SET payload=test message hello

mqtc 1 "%tqname%" "%payload%"
