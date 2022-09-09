
project(tsdb_api)

#set(testtsdb main.c)

# 生成可执行文件
add_executable(testtsdb main.c)

# 链接
target_link_libraries(testtsdb dy_lib${PROJECT_NAME})

set_target_properties(testtsdb PROPERTIES C_STANDARD 99)

INSTALL(TARGETS testtsdb DESTINATION test)
