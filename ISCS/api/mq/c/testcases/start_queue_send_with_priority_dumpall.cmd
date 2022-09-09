ECHO OFF
SET tqname=
SET priority=
SET /P tqname=Enter a value for queue name, default name is[queue://domain:test_queue_recv_pri]:
IF [%tqname%]==[] SET tqname=queue://domain:test_queue_recv_pri

mqtc 6 "%tqname%"