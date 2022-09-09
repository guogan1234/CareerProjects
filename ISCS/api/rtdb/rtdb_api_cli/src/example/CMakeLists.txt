
project(rtdb_api_cli)

#INCLUDE_DIRECTORIES(/usr/local/include/rdb_cli)
#LINK_DIRECTORIES(/usr/local/lib)

#set(LIBS_LIST -lrdb_cli -luuid -lconf_parser -lmxml -lcmn -lpthread)

# 生成可执行文件
add_executable(test_delete_record test_delete_record.c)
add_executable(test_insert_table test_insert_table.c)
add_executable(test_list_attrs test_list_attrs.c)
add_executable(test_list_children test_list_children.c)
add_executable(test_multi_query_attrs test_multi_query_attrs.c)
add_executable(test_query_node test_query_node.c)
add_executable(test_query_record test_query_record.c)
add_executable(test_query_table test_query_table.c)
add_executable(test_send_cmd test_send_cmd.c)
add_executable(test_send_msg test_send_msg.c)
add_executable(test_set_attr test_set_attr.c)
add_executable(test_sub_node test_sub_node.c)
add_executable(test_sub_table test_sub_table.c)
add_executable(test_unique_query_attr test_unique_query_attr.c)
add_executable(test_update_table test_update_table.c)
add_executable(test_get_node_type test_get_node_type.c)

# 链接
target_link_libraries(test_delete_record dy_lib${PROJECT_NAME})
target_link_libraries(test_insert_table dy_lib${PROJECT_NAME})
target_link_libraries(test_list_attrs dy_lib${PROJECT_NAME})
target_link_libraries(test_list_children dy_lib${PROJECT_NAME})
target_link_libraries(test_multi_query_attrs dy_lib${PROJECT_NAME})
target_link_libraries(test_query_node dy_lib${PROJECT_NAME})
target_link_libraries(test_query_record dy_lib${PROJECT_NAME})
target_link_libraries(test_query_table dy_lib${PROJECT_NAME})
target_link_libraries(test_send_cmd dy_lib${PROJECT_NAME})
target_link_libraries(test_send_msg dy_lib${PROJECT_NAME})
target_link_libraries(test_set_attr dy_lib${PROJECT_NAME})
target_link_libraries(test_sub_node dy_lib${PROJECT_NAME})
target_link_libraries(test_sub_table dy_lib${PROJECT_NAME})
target_link_libraries(test_unique_query_attr dy_lib${PROJECT_NAME})
target_link_libraries(test_update_table dy_lib${PROJECT_NAME})
target_link_libraries(test_get_node_type dy_lib${PROJECT_NAME})

set(TARGET_EXP_LIST test_delete_record test_insert_table test_list_attrs test_list_children test_multi_query_attrs
                    test_query_node test_query_record test_query_table test_send_cmd test_send_msg test_set_attr
                    test_sub_node test_sub_table test_unique_query_attr test_update_table 
                    test_get_node_type )
INSTALL(TARGETS ${TARGET_EXP_LIST} DESTINATION test)
