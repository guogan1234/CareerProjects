
project(libconfdb)

set(testconfdb main.c)

# 生成可执行文件
add_executable(testconfdb main.c)

# 链接
#target_link_libraries(testmq libmq_static)
target_link_libraries(testconfdb libconfdb)
