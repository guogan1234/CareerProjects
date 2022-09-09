CC=gcc
CFLAGS=-w -g
HEADERS=../include
LIBDIR=../lib
LIBS=-lpthread -lcmn -lconf_parser -L$(LIBDIR) -l:librdb_fes_client.a

all:
	make fesApiTimedTest1
	make fesApiChangedTest1
	make fesApiCmdTest1
	make fesApiTimedMultiTest1
	make fesApiChangedMultiTest1

fesApiTimedTest1:fesApiTimedTest1.o 
	$(CC) fesApiTimedTest1.o  -o fesApiTimedTest1 $(LIBS) 

fesApiChangedTest1:fesApiChangedTest1.o 
	$(CC) fesApiChangedTest1.o  -o fesApiChangedTest1 $(LIBS) 

fesApiCmdTest1:fesApiCmdTest1.o 
	$(CC) fesApiCmdTest1.o  -o fesApiCmdTest1 $(LIBS) 
    
fesApiTimedMultiTest1:fesApiTimedMultiTest1.o 
	$(CC) fesApiTimedMultiTest1.o  -o fesApiTimedMultiTest1 $(LIBS) 

fesApiChangedMultiTest1:fesApiChangedMultiTest1.o 
	$(CC) fesApiChangedMultiTest1.o  -o fesApiChangedMultiTest1 $(LIBS) 
        
.c.o:
	$(CC) $(CFLAGS) -I$(HEADERS) -c -o $*.o $<

clean :
	rm -rf *.o
	rm -rf fesApiTimedTest1
	rm -rf fesApiChangedTest1
	rm -rf fesApiCmdTest1
	rm -rf fesApiTimedMultiTest1
	rm -rf fesApiChangedMultiTest1



