
PROJECT(mq_api)

SET(JNIMQ_SRC_LIST jni_InnerNativeMQ.c)

# jni.h jni_md.h使用了oracle jdk 1.8版本下的
INCLUDE_DIRECTORIES(.)

# 生成动态库，JNI只能依赖dll/so
ADD_LIBRARY(mq_api_jni SHARED ${JNIMQ_SRC_LIST})

SET(INC_EXP_JNI_LIST 
    jni.h
    jni_md.h
    jni_InnerNativeMQ.h
    jni_InnerMQChannel.h
    jni_InnerMessageListener.h
)

# 指定动态库版本
SET_TARGET_PROPERTIES(mq_api_jni PROPERTIES VERSION 1.0 SOVERSION 1)

# 链接依赖libmq.so
IF(UNIX)
	TARGET_LINK_LIBRARIES(mq_api_jni dy_lib${PROJECT_NAME})
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(mq_api_jni dy_lib${PROJECT_NAME} -Wl,--kill-at)
endif()

#安装jni lib
INSTALL(TARGETS mq_api_jni DESTINATION lib)
#INSTALL(TARGETS mq_api_jni DESTINATION lib)
#INSTALL(FILES ${INC_EXP_JNI_LIST} DESTINATION include/${PROJECT_NAME}/)
