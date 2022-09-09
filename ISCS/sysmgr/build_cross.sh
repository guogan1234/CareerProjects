#!/bin/sh
#


clean_win() {
    echo "start to clean device magangers ... "
	
    cd device_mgr/sysmgr_device_agent
    $QT_HOME/Tools/mingw530_32/bin/mingw32-make -f Makefile.$BUILD_TYPE clean
    cd ../..

    cd device_mgr/sysmgr_device_server
    $QT_HOME/Tools/mingw530_32/bin/mingw32-make -f Makefile.$BUILD_TYPE clean
    cd ../..

    cd device_mgr/sysmgr_device_viewer
    $QT_HOME/Tools/mingw530_32/bin/mingw32-make -f Makefile.$BUILD_TYPE clean
    cd ../..
	
    echo "device magangers clean done!"
}

build_win() {
    echo "start to build device magangers ... "
	
    cd device_mgr/sysmgr_device_agent
    $QT_HOME/5.9.3/mingw53_32/bin/qmake  SysDevInfoCollector.pro -o Makefile.$BUILD_TYPE
	$QT_HOME/Tools/mingw530_32/bin/mingw32-make -f Makefile.$BUILD_TYPE
	$QT_HOME/Tools/mingw530_32/bin/mingw32-make -f Makefile.$BUILD_TYPE install 
    cd ../..
    
    cd device_mgr/sysmgr_device_server
    $QT_HOME/5.9.3/mingw53_32/bin/qmake  SysDevInfoService.pro -o Makefile.$BUILD_TYPE
	$QT_HOME/Tools/mingw530_32/bin/mingw32-make -f Makefile.$BUILD_TYPE
	$QT_HOME/Tools/mingw530_32/bin/mingw32-make -f Makefile.$BUILD_TYPE install 
    cd ../..

    cd device_mgr/sysmgr_device_viewer
    $QT_HOME/5.9.3/mingw53_32/bin/qmake  sys_dev_info_viewer.pro -o Makefile.$BUILD_TYPE
	$QT_HOME/Tools/mingw530_32/bin/mingw32-make -f Makefile.$BUILD_TYPE
	$QT_HOME/Tools/mingw530_32/bin/mingw32-make -f Makefile.$BUILD_TYPE install 
    cd ../..
    
    echo "device magangers build done!"
}

clean_lix() {
    echo "start to clean device magangers ... "
	
    cd device_mgr/sysmgr_device_agent
    make -f Makefile.$BUILD_TYPE clean
    cd ../..

    cd device_mgr/sysmgr_device_server
    make -f Makefile.$BUILD_TYPE clean
    cd ../..

    cd device_mgr/sysmgr_device_viewer
    make -f Makefile.$BUILD_TYPE clean
    cd ../..
	
    echo "device magangers clean done!"
}

build_lix() {
    echo "start to build device magangers ... "
	
    cd device_mgr/sysmgr_device_agent
    qmake  SysDevInfoCollector.pro -o Makefile.$BUILD_TYPE
	make -f Makefile.$BUILD_TYPE
	make -f Makefile.$BUILD_TYPE install 
    cd ../..
    
    cd device_mgr/sysmgr_device_server
    qmake  SysDevInfoService.pro -o Makefile.$BUILD_TYPE
	make -f Makefile.$BUILD_TYPE
	make -f Makefile.$BUILD_TYPE install 
    cd ../..

    cd device_mgr/sysmgr_device_viewer
    qmake  sys_dev_info_viewer.pro -o Makefile.$BUILD_TYPE
	make -f Makefile.$BUILD_TYPE
	make -f Makefile.$BUILD_TYPE install 
    cd ../..
    
    echo "device magangers build done!"
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
