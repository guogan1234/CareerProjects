PUSHD .
SET count=1000000

mkdir qsub
mkdir tsub
mkdir qpsnd
mkdir tsnd
xcopy /Y test\testmqbench.exe qsub
xcopy /Y test\testmqbench.exe tsub
xcopy /Y test\testmqbench.exe qpsnd
xcopy /Y test\testmqbench.exe tsnd
cd qsub
REM start testmqbench.exe qsub %count% 1 0
cd ..\tsub
start testmqbench.exe tsub %count% 1 0
REM cd ..\qpsnd
REM start testmqbench.exe qpsnd %count% 1 0
cd ..\tsnd
PAUSE
REM start testmqbench.exe tsnd %count% 1 0
POPD

