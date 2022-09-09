CC=gcc
CFLAGS=-w -g

DSTLIBNAME=librdb_fes_client
DSTSTATICLIB=$(DSTLIBNAME).a
DSTDYNAMICLIB=$(DSTLIBNAME).so
HEADERS=-I/usr/local/include/cmn -I/usr/local/include/conf_parser
LIBS=-lpthread -L/usr/local/lib -lcmn -lconf_parser
SRCOBJFILES=irtdb_fes_data.o irtdb_fes_privatecommon.o cmn_list.o irtdb_fes_privatedata.o irtdb_fes_interface.o 

all:$(SRCOBJFILES) 
	ar rcs $(DSTSTATICLIB) $(SRCOBJFILES)  
	$(CC) -shared -fPIC -o $(DSTDYNAMICLIB)	 $(SRCOBJFILES) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -fPIC $(HEADERS) -o $*.o $<

clean :
	rm -rf *.o
	rm -rf $(DSTSTATICLIB)
	rm -rf $(DSTDYNAMICLIB)

