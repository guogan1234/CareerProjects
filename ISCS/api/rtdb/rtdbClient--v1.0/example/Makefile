CC=gcc
CFLAGS=-w -g
HEADERS=../include
LIBS=-L../lib -l:librdb_cli.a  -luuid -lconf_parser -lmxml -lcmn -lpthread
	
DSTBIN=test_unique_query_attr test_multi_query_attrs test_query_node test_list_children test_list_attrs test_set_attr test_query_table test_query_record test_delete_record test_update_table test_insert_table test_send_cmd test_send_msg test_sub_table test_sub_node 

all:test_unique_query_attr.o test_multi_query_attrs.o test_query_node.o test_list_children.o test_list_attrs.o test_set_attr.o test_query_table.o test_query_record.o test_delete_record.o test_update_table.o test_insert_table.o test_send_cmd.o test_send_msg.o test_sub_table.o test_sub_node.o 
	$(CC) test_unique_query_attr.o -o test_unique_query_attr $(LIBS)
	$(CC) test_multi_query_attrs.o -o test_multi_query_attrs $(LIBS)
	$(CC) test_query_node.o -o test_query_node $(LIBS)
	$(CC) test_list_children.o -o test_list_children $(LIBS)
	$(CC) test_list_attrs.o -o test_list_attrs $(LIBS)
	$(CC) test_set_attr.o -o test_set_attr $(LIBS)
	$(CC) test_query_table.o -o test_query_table $(LIBS)
	$(CC) test_query_record.o -o test_query_record $(LIBS)
	$(CC) test_delete_record.o -o test_delete_record $(LIBS)
	$(CC) test_update_table.o -o test_update_table $(LIBS)
	$(CC) test_insert_table.o -o test_insert_table $(LIBS)
	$(CC) test_send_cmd.o -o test_send_cmd $(LIBS)
	$(CC) test_send_msg.o -o test_send_msg $(LIBS)
	$(CC) test_sub_table.o -o test_sub_table $(LIBS)
	$(CC) test_sub_node.o -o test_sub_node $(LIBS)
	
		
.c.o:
	$(CC) $(CFLAGS) -I$(HEADERS) -c -o $*.o $<

clean :
	rm -rf *.o
	rm -rf $(DSTBIN)
