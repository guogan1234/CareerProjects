
project(cfdb_api_rtdb)

#set(test_cfdb_rtdb test.c)

# 生成可执行文件
add_executable(test_cfdb_rtdb test.c)

# 链接
target_link_libraries(test_cfdb_rtdb dy_lib${PROJECT_NAME})

INSTALL(TARGETS test_cfdb_rtdb DESTINATION test)
