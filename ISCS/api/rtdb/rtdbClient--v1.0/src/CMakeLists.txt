CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# 工程名称（必填项)
PROJECT(rdb_cli)

# 编译器统一在执行cmake的时候指定。
# 指定编译器（默认）。Window平台下编译器用环境变量 "MINGW_GCC_HOME"。
#if (WIN32)
#SET(CMAKE_C_COMPILER $ENV{MINGW_GCC_HOME}/bin/gcc)
#SET(CMAKE_CXX_COMPILER $ENV{MINGW_GCC_HOME}/bin/g++)
#endif()

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
endif()
IF(USE_32BITS)
MESSAGE("OS is 32bits")
ELSE()
MESSAGE("OS is 64bits")
endif()

# 指定默认安装路径前缀(默认)。
# 默认的安装路径 Unix/Linux/Window都为/usr/local，其中windows为当前磁盘分区的X:/usr/local
# 由编译脚本指定。
# SET(CMAKE_INSTALL_PREFIX /usr/local)

IF(UNIX)

# 指定依赖的三方库路径（默认）
LINK_DIRECTORIES(/usr/local/lib)

# 指定功能的头文件路径(根据实际情况修改)。这里给出了一些默认路径。
INCLUDE_DIRECTORIES(
.
..
src
include
/usr/local/include
/usr/local/include/conf_parser
/usr/local/include/cmn
)
# 指定依赖的三方库名称（根据实际情况修改）。例如libtest.so
SET( LIB_LIST -lconf_parser -lcmn -luuid)

ELSE(WIN32)

# 指定依赖的三方库路径（默认）
LINK_DIRECTORIES($ENV{MSYS_HOME}/local/lib)

# 指定功能的头文件路径(根据实际情况修改)。这里给出了一些默认路径。
INCLUDE_DIRECTORIES(
.
..
src
include
$ENV{MSYS_HOME}/local/include
$ENV{MSYS_HOME}/local/include/conf_parser
$ENV{MSYS_HOME}/local/include/cmn
)

# 指定依赖的三方库名称（根据实际情况修改）。例如libtest.so
SET( LIB_LIST Ws2_32.lib -lconf_parser -lcmn -lole32)

endif()

# 设置结果输出路径（默认）.
# 动态库、静态库用 LIBRARY_OUTPUT_PATH，可执行程序用　EXECUTABLE_OUTPUT_PATH
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# 设置编译类型 Debug|Release，外部编译时 CMAKE_BUILD_TYPE 用（根据实际情况修改）。
# C设置 CMAKE_C_FLAGS_DEBUG|CMAKE_C_FLAGS_RELEASE
# C++设置 CMAKE_CXX_FLAGS_DEBUG|CMAKE_CXX_FLAGS_RELEASE
# C++示例：
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fPIC -Wall")
SET(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -fPIC -Wall -g -ggdb")
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -fPIC -Wall")

# 设置宏定义（根据实际情况修改）。例如-DENABLE_DEBUG -DSHOW_TRACE
ADD_DEFINITIONS()

# 指定源码目录（根据实际情况修改）。
# 用实际的源码目录替换“your_source_dir”
# 不推荐使用AUX_SOURCE_DIRECTORY指令，建议使用手动填写方式。
# AUX_SOURCE_DIRECTORY(your_source_dir SRC_LIST)
SET( SRC_LIST 
rtdb_cli_base.c
rtdb_cli_data.c
rtdb_cli_list.c
rtdb_cli_net.c
rtdb_cli_path.c
rtdb_cli_sds.c
rtdb_cli_protocol.c
rtdb_client.c
)

# 指定要导出的头文件（根据实际情况修改）
SET( INC_EXP_LIST 
rtdb_cli_export.h
rtdb_cli_data.h
rtdb_cli_data_public.h
rtdb_cli_list.h
rtdb_client.h
)

# 添加编译结果（默认）。动态库、静态库用 ADD_LIBRARY，可执行程序用 ADD_EXECUTABLE 。
ADD_LIBRARY(dy_lib${PROJECT_NAME} SHARED ${SRC_LIST})
ADD_LIBRARY(st_lib${PROJECT_NAME} STATIC ${SRC_LIST})

# 设置动态库、静态库编译属性（默认）。
SET_TARGET_PROPERTIES(dy_lib${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
SET_TARGET_PROPERTIES(st_lib${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
SET_TARGET_PROPERTIES(dy_lib${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(st_lib${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(dy_lib${PROJECT_NAME} PROPERTIES VERSION 1.0 SOVERSION 1)

# 设置外部动态连接库（根据实际情况修改）。
# 例如， TARGET_LINK_LIBRARIES(dy_lib ${LIB_LIST} -lpthread)
TARGET_LINK_LIBRARIES(dy_lib${PROJECT_NAME} ${LIB_LIST})

# 设置安装（根据实际情况修改）。
INSTALL(TARGETS dy_lib${PROJECT_NAME} DESTINATION lib)
INSTALL(TARGETS st_lib${PROJECT_NAME} DESTINATION lib)
INSTALL(FILES ${INC_EXP_LIST} DESTINATION include/${PROJECT_NAME}/)
