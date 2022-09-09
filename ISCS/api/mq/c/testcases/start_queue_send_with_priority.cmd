ECHO OFF
SET tqname=
SET priority=
SET /P tqname=Enter a value for queue name, default name is[queue://domain:test_queue_recv_pri]:
IF [%tqname%]==[] SET tqname=queue://domain:test_queue_recv_pri

SET /P priority=Enter a value for priority [0-9], default is[0]:
IF [%priority%]==[] SET priority=0

SET payload=
SET /P payload=Enter a message to be sent, default message is[queue test message hello]:
IF "%payload%"=="" SET payload=queue test message hello
mqtc 5 "%tqname%" "%payload%" "%priority%"