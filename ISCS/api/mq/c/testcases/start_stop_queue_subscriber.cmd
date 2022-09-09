ECHO OFF
SET tqname=
SET /P tqname=Enter a value for queue name, default name is[queue://domain:test_queue]:
IF [%tqname%]==[] SET tqname=queue://domain:test_queue
mqtc 3 "%tqname%"