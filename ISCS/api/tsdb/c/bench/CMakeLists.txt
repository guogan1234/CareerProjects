project(tsdb_api)

#set(benchtsdb main.c)

# 生成可执行文件
add_executable(benchtsdb main.c)

# 链接
target_link_libraries(benchtsdb dy_lib${PROJECT_NAME})

set_target_properties(benchtsdb PROPERTIES C_STANDARD 99)

INSTALL(TARGETS benchtsdb DESTINATION test)

