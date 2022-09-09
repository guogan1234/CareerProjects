cmake_minimum_required(VERSION 3.5)

project(sys_dev_station)

# debug/release
#set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")

#set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})

aux_source_directory(. SRCS)

# 头文件引用目录
include_directories(. ../../../dependences/mxml-2.11 ../../../dependences/cmn ../../../api/sys/g_conf_parser/conf_parser ../../../dependences/tstl2cl)

# 引用依赖库（静态引用）
find_library(MXML_LIB mxml ../../../dependences/mxml-2.11 NO_DEFAULT_PATH)
find_library(CONF_PARSER_LIB confparser ../../../api/sys/g_conf_parser NO_DEFAULT_PATH)
find_library(TSTL2CL_LIB tstl2cl ../../../dependences/tstl2cl NO_DEFAULT_PATH)
#find_library(CMN_LIB cmn ../../../dependences/cmn NO_DEFAULT_PATH)
link_libraries(${CONF_PARSER_LIB} ${MXML_LIB} pthread)

set(AppName "sys_dev_staion")
add_executable(${AppName} ${SRCS})

# 使动态库和静态库同时存在
# cmake在构建一个新的target时，会尝试清理掉其他使用这个名字的库，
# 为了回避这个问题，比如再次使用SET_TARGET_PROPERTIES定义 CLEAN_DIRECT_OUTPUT属性。
#set_target_properties(libconfparser PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#set_target_properties(libconfparser_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 添加子目录（每个子目录下都必须有自已的CMakeLists.txt文件
#add_subdirectory(test)

# 将libconfparser的共享库安装到<prefix>/lib目录；
# 将conf_manager 安装<prefix>/include/sys目录。
#INSTALL (TARGETS libconfparser libconfparser_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
#INSTALL (FILES conf_parser.h DESTINATION include/sys)


