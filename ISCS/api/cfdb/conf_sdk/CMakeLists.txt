cmake_minimum_required(VERSION 3.5)

project(libconfdb)

# debug/release
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 输出target
set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})

# 代码目录
aux_source_directory(. srcs)

# 头文件引用目录
include_directories(. ../../../dependences/cmn ../../../dependences/tstl2cl/include ../../../dependences/mxml-2.11)

# 引用依赖库
find_library(CMN_LIB cmn ../../../dependences/cmn NO_DEFAULT_PATH)
find_library(TSTL2CL_LIB tstl2cl ../../../dependences/tstl2cl NO_DEFAULT_PATH)
find_library(MXML_LIB mxml ../../../dependences/mxml-2.11 NO_DEFAULT_PATH)
link_libraries(${CMN_LIB} ${TSTL2CL_LIB} ${MXML_LIB} -lpthread)

# 生成动态库目标
add_library(libconfdb SHARED ${srcs})
# 生成静态库目标
add_library(libconfdb_static STATIC ${srcs})

# 指定动态库的输出名称
set_target_properties(libconfdb PROPERTIES OUTPUT_NAME "confdb")
# 指定静态库的输出名称
set_target_properties(libconfdb_static PROPERTIES OUTPUT_NAME "confdb")

# 使动态库和静态库同时存在
# cmake在构建一个新的target时，会尝试清理掉其他使用这个名字的库，
# 为了回避这个问题，比如再次使用SET_TARGET_PROPERTIES定义 CLEAN_DIRECT_OUTPUT属性。
set_target_properties(libconfdb PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(libconfdb_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 指定动态库版本
# VERSION 动态库版本
# SOVERSION API版本
set_target_properties(libconfdb PROPERTIES VERSION 1.0 SOVERSION 1)

# 添加子目录（每个子目录下都必须有自已的CMakeLists.txt文件
add_subdirectory(test)

# 将libconfdb的共享库安装到<prefix>/lib目录；
# 将sdk.h安装<prefix>/include/confdb目录。
INSTALL (TARGETS libconfdb libconfdb_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
INSTALL (FILES sdk.h DESTINATION include/confdb)
