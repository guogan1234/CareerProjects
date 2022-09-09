
project(sysconf_api)

# 生成可执行文件
add_executable(test_sysconf test.c)

# 链接
set_target_properties(test_sysconf PROPERTIES C_STANDARD 99)
target_link_libraries(test_sysconf dy_lib${PROJECT_NAME})

INSTALL(TARGETS test_sysconf DESTINATION test)
