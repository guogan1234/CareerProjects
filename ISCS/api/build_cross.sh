#!/bin/sh
#


clean_win() {
    echo "start to clean apis ... "
	$MSYS_HOME/bin/rm -rf build
	echo "apis clean done!"
}

build_win() {
    echo "start to build apis ... "
	$MSYS_HOME/bin/mkdir -p build
	
	cd build
	$CMAKE_HOME/bin/cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../sysconf_api
	$CMAKE_HOME/bin/cmake --build .
	$MINGW_HOME/bin/mingw32-make install
	$MSYS_HOME/bin/rm -rf *
	
	$CMAKE_HOME/bin/cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
	$CMAKE_HOME/bin/cmake --build .
	$MINGW_HOME/bin/mingw32-make install
	cd ..
	echo "apis build done!"
}

clean_lix() {
    echo "start to clean apis ... "
	rm -rf build
	rm -fr mq/java/mq_api/target
	echo "apis clean done!"
}

build_lix() {
    echo "start to build apis ... "
	mkdir -p build
	cd build
	cmake  -DCMAKE_C_FLAGS="-std=gnu99" -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../sysconf_api
	cmake --build .
	make install
	rm -rf *
	
	cmake  -DCMAKE_C_FLAGS="-std=gnu99" -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
	cmake --build .
	make install
	cd ..
	
	cd mq/java/mq_api
	mvn clean package install
	cd ../../..
	echo "apis build done!"
}

TARGET=rebuild
BUILD_TYPE=Release
PLATFORM=$('uname' 2>/dev/null)

if [ $# -eq 1 ]; then
	TARGET=$1
elif [ $# -eq 2 ]; then
	TARGET=$1
	BUILD_TYPE=$2
fi

echo "ready to build target[$TARGET], type[$BUILD_TYPE]"
if [ -z $PLATFORM ]; then
# Window
	case $TARGET in
		clean )
			clean_win
			break
			;;
		build )
			build_win
			break
			;;
		rebuild )
		    clean_win
			build_win
			;;
	esac
else
# Unix/Linx
	case $TARGET in
		clean )
			clean_lix
			break
			;;
		build )
			build_lix
			break
			;;
		rebuild )
		    clean_lix
			build_lix
			;;
	esac
fi
