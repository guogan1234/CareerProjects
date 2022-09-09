
project(sysmgr_api_conf)

#set(test_api_conf test.c)

# 生成可执行文件
add_executable(test_api_conf test.c)

# 链接
target_link_libraries(test_api_conf dy_lib${PROJECT_NAME} pthread)

INSTALL(TARGETS test_api_conf DESTINATION test)
