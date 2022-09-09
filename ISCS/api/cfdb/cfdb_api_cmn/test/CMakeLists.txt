
PROJECT(cfdb_api_cmn)

#SET(test_cfdb_cmn test.c)

# 生成可执行文件
ADD_EXECUTABLE(test_cfdb_cmn test.c)

# 链接
TARGET_LINK_LIBRARIES(test_cfdb_cmn dy_lib${PROJECT_NAME})

SET_TARGET_PROPERTIES(test_cfdb_cmn PROPERTIES C_STANDARD 99)

INSTALL(TARGETS test_cfdb_cmn DESTINATION test)
