CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# 工程名称（必填项)
PROJECT(cfdb_api_cmn)

# 输出提示信息
MESSAGE("Project name=${PROJECT_NAME}")
MESSAGE("OS name=${CMAKE_SYSTEM_NAME}")
MESSAGE("OS version=${CMAKE_SYSTEM_VERSION}")
MESSAGE("GCC=${CMAKE_C_COMPILER}")
MESSAGE("GCC=${CMAKE_CXX_COMPILER}")
IF(UNIX)
    MESSAGE("Platform Unix Like=${UNIX}")
ELSE(WIN32)
    MESSAGE("Platform Win=${WIN32}")
    MESSAGE("Install location=$ENV{MSYS_HOME}")
endif()
IF(USE_32BITS)
    MESSAGE("OS is 32bits")
ELSE()
    MESSAGE("OS is 64bits")
endif()

# 指定默认安装路径前缀(默认)。
# 默认的安装路径 src/out
SET(CMAKE_INSTALL_PREFIX $ENV{ISCS_SRC_HOME}/out)

# 指定依赖的三方库路径（默认）
LINK_DIRECTORIES($ENV{ISCS_SRC_HOME}/out/lib)

# 指定功能的头文件路径(根据实际情况修改)。这里给出了一些默认路径。
INCLUDE_DIRECTORIES(
    .
    ..
    include
    $ENV{ISCS_SRC_HOME}/out/include
    $ENV{ISCS_SRC_HOME}/out/include/cmn
    $ENV{ISCS_SRC_HOME}/out/include/curl
    $ENV{ISCS_SRC_HOME}/out/include/parson
	$ENV{ISCS_SRC_HOME}/out/include/sysmgr_api
)
	
 # 指定依赖的三方库名称（根据实际情况修改）。例如libtest.so
SET( LIB_LIST -lcmn -lcurl -lparson -lsysmgr_api_conf )

# 设置结果输出路径（默认）.
# 动态库、静态库用 LIBRARY_OUTPUT_PATH，可执行程序用　EXECUTABLE_OUTPUT_PATH
MESSAGE("project output dir = ${PROJECT_BINARY_DIR}")
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# 设置编译类型 Debug|Release，外部编译时 CMAKE_BUILD_TYPE 用（根据实际情况修改）。
# C设置 CMAKE_C_FLAGS_DEBUG|CMAKE_C_FLAGS_RELEASE
# C++设置 CMAKE_CXX_FLAGS_DEBUG|CMAKE_CXX_FLAGS_RELEASE
# C++示例：
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
SET(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")

# 设置宏定义（根据实际情况修改）。例如-DENABLE_DEBUG -DSHOW_TRACE
ADD_DEFINITIONS()

# 指定源码目录（根据实际情况修改）。
# 用实际的源码目录替换“your_source_dir”
# 不推荐使用AUX_SOURCE_DIRECTORY指令，建议使用手动填写方式。
# AUX_SOURCE_DIRECTORY(your_source_dir SRC_LIST)
SET(SRC_LIST
    confcmn.c
    recordset.c
)

# 指定要导出的头文件（根据实际情况修改）
SET( INC_EXP_LIST 
    confcmn.h
)

# 添加编译结果（默认）。动态库、静态库用 ADD_LIBRARY，可执行程序用 ADD_EXECUTABLE 。
ADD_LIBRARY(dy_lib${PROJECT_NAME} SHARED ${SRC_LIST})
ADD_LIBRARY(st_lib${PROJECT_NAME} STATIC ${SRC_LIST})

# 设置动态库、静态库编译属性（默认）。
SET_TARGET_PROPERTIES(dy_lib${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
SET_TARGET_PROPERTIES(st_lib${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
SET_TARGET_PROPERTIES(dy_lib${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(st_lib${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(dy_lib${PROJECT_NAME} PROPERTIES VERSION 2.0 SOVERSION 2)

# 设置外部动态连接库（根据实际情况修改）。
# 例如， TARGET_LINK_LIBRARIES(dy_lib ${LIB_LIST} -lpthread)
TARGET_LINK_LIBRARIES(dy_lib${PROJECT_NAME} ${LIB_LIST})

ADD_SUBDIRECTORY(test)

# 设置安装（根据实际情况修改）。
INSTALL(TARGETS dy_lib${PROJECT_NAME} DESTINATION lib)
INSTALL(TARGETS st_lib${PROJECT_NAME} DESTINATION lib)
INSTALL(FILES ${INC_EXP_LIST} DESTINATION include/cfdb_api/)
