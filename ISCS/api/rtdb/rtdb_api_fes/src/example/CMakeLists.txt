
project(rtdb_api_fes)

#INCLUDE_DIRECTORIES(/usr/local/include/rdb_fes_client)
#LINK_DIRECTORIES(/usr/local/lib)

#set(LIBS_LIST -lrdb_fes_client -luuid -lconf_parser -lmxml -lcmn -lpthread)

# 生成可执行文件
add_executable(fesApiTimedTest1 fesApiTimedTest1.c)
add_executable(fesApiChangedTest1 fesApiChangedTest1.c)
add_executable(fesApiCmdTest1 fesApiCmdTest1.c)
add_executable(fesApiTimedMultiTest1 fesApiTimedMultiTest1.c)
add_executable(fesApiChangedMultiTest1 fesApiChangedMultiTest1.c)
add_executable(fesApiUploadSOETest1 fesApiUploadSOETest1.c)


# 链接
target_link_libraries(fesApiTimedTest1 dy_lib${PROJECT_NAME})
target_link_libraries(fesApiChangedTest1 dy_lib${PROJECT_NAME})
target_link_libraries(fesApiCmdTest1 dy_lib${PROJECT_NAME})
target_link_libraries(fesApiTimedMultiTest1 dy_lib${PROJECT_NAME})
target_link_libraries(fesApiChangedMultiTest1 dy_lib${PROJECT_NAME})
target_link_libraries(fesApiUploadSOETest1 dy_lib${PROJECT_NAME})

set(TARGET_EXP_LIST fesApiTimedTest1 fesApiChangedTest1 fesApiCmdTest1 fesApiTimedMultiTest1 fesApiChangedMultiTest1 fesApiUploadSOETest1)
INSTALL(TARGETS ${TARGET_EXP_LIST} DESTINATION test)

