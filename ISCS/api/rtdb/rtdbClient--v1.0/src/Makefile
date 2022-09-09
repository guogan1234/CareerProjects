# Makefile RTDB client
# This file is released under the BSD license, see the COPYING file

OBJ=rtdb_cli_base.o  rtdb_cli_data.o  rtdb_cli_list.o  rtdb_cli_net.o  \
    rtdb_cli_path.o  rtdb_cli_sds.o  rtdb_client.o rtdb_cli_protocol.o
    
LIBNAME=librdb_cli

# Fallback to gcc when $CC is not in $PATH.
CC:=$(shell sh -c 'type $(CC) >/dev/null 2>/dev/null && echo $(CC) || echo gcc')
OPTIMIZATION?=-O3
WARNINGS=-Wall -W 
DEBUG?= -g -ggdb
REAL_CFLAGS=$(OPTIMIZATION) -fPIC $(CFLAGS) $(WARNINGS) $(DEBUG) $(ARCH)

DYLIBSUFFIX=so
STLIBSUFFIX=a
STLIBNAME=$(LIBNAME).$(STLIBSUFFIX)
STLIB_MAKE_CMD=ar rcs $(STLIBNAME)
DYLIBNAME=$(LIBNAME).$(DYLIBSUFFIX)
DYLIB_MAKE_CMD=$(CC) -shared -fPIC -o $(DYLIBNAME) -DRTDB_CLIENT_EXPORT


HEADERS=-I../../../dependences/cmn -I../../../dependences/mxml-2.11  -I../../sys/g_conf_parser 
DEPENDLIBS=-I../../../dependences/cmn -lcmn -L../../../dependences/mxml-2.11 -lmxml \
    -L../../sys/g_conf_parser -lconf_parser -lpthread -luuid

OS_NAME = $(shell uname -o)
ifeq ($(OS_NAME), Msys)
WINLIBS=-lwsock32 -lWS2_32
endif

all:$(DYLIBNAME) $(STLIBNAME)

$(DYLIBNAME): $(OBJ)
	$(DYLIB_MAKE_CMD) $(OBJ) $(DEPENDLIBS) $(WINLIBS) 

$(STLIBNAME): $(OBJ)
	$(STLIB_MAKE_CMD) $(OBJ) 

dynamic: $(DYLIBNAME)
static: $(STLIBNAME)

# Binaries:

.c.o:
	$(CC) -pedantic -c $(REAL_CFLAGS) $(HEADERS)  $(DEPENDLIBS) $(WINLIBS) $<

clean:
	rm -rf $(DYLIBNAME) $(STLIBNAME) $(TESTS) examples/example* *.o *.gcda *.gcno *.gcov

dep:
	$(CC) -MM *.c

