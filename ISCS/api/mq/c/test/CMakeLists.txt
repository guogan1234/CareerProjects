
PROJECT(mq_api)

#SET(testmq test.c)

# 生成可执行文件
ADD_EXECUTABLE(testmq test.c)

# 链接
#target_link_libraries(testmq libmq_static pthread)
TARGET_LINK_LIBRARIES(testmq dy_lib${PROJECT_NAME} pthread)
SET_TARGET_PROPERTIES(testmq PROPERTIES C_STANDARD 99)

INSTALL(TARGETS testmq DESTINATION test)

ADD_EXECUTABLE(testmqbench testbench.c)

# 链接
#target_link_libraries(testmq libmq_static pthread)
TARGET_LINK_LIBRARIES(testmqbench dy_lib${PROJECT_NAME} pthread)
SET_TARGET_PROPERTIES(testmqbench PROPERTIES C_STANDARD 99)

ADD_EXECUTABLE(mqtc mq_testcase.c)

# 链接
TARGET_LINK_LIBRARIES(mqtc dy_lib${PROJECT_NAME} pthread)
SET_TARGET_PROPERTIES(mqtc PROPERTIES C_STANDARD 99)

INSTALL(TARGETS testmqbench mqtc DESTINATION test)