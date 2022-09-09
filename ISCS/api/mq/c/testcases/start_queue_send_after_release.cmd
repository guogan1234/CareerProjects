ECHO OFF
SET tqname=
SET /P tqname=Enter a value for queue name, default name is[queue://domain:test_queue_recv]:
IF [%tqname%]==[] SET tqname=queue://domain:test_queue_recv

SET payload=
SET /P payload=Enter a message to be sent, default message is[queue test message hello]:
IF "%payload%"=="" SET payload=queue test message hello
mqtc 4 "%tqname%" "%payload%"