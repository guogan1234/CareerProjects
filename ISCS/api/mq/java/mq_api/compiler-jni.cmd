call mvn clean package

javah -classpath target\classes -d tmp jni.InnerMessageListener

javah -classpath target\classes -d tmp jni.InnerNativeMQ

javah -classpath target\classes -d tmp jni.InnerMQChannel
