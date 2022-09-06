#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef TURNTABLE_EXPORTS
# define SCANNER_API __declspec(dllexport)
#else
# define SCANNER_API __declspec(dllimport )
#endif

extern "C"
{
    //建立通信连接
    //参数：设备号iDevice，iDevice=0；
    //返回值：0-通信成功，非0为通信失败。
    SCANNER_API int s_open_device(short iDevice);


    //断开通信连接
    //参数：设备号iDevice，iDevice=0；
    //返回值：0-通信成功，非0为通信失败。
    SCANNER_API int s_close_device(short iDevice);



    //移动设备
    //参数：设备号iDevice，iDevice=0；轴号iAxis，iAxis=1方位轴，iAxis=2俯仰轴；位置值fPos；速度值fVel；运动类型iType，iType=0绝对运动，iType!=0相对运动；
    //返回值：0-通信成功，非0为通信失败。
    SCANNER_API int s_move_device(short iDevice,short iAxis,double fPos,double fVel,short iType);

    //停止设备运动
    //参数：设备号iDevice，iDevice=0；轴号iAxis，iAxis=1方位轴，iAxis=2俯仰轴；
    //返回值：0-通信成功，非0为通信失败。
    SCANNER_API int s_stop_device(short iDevice,short iAxis);

    //获取设备当前位置值
    //参数：设备号iDevice，iDevice=0；轴号iAxis，iAxis=1方位轴，iAxis=2俯仰轴；cPos[20]用来存储位置值，ASIIC码形式存储，当前位置若为123°=123000ct，
    //返回值：0-通信成功，非0为通信失败。
    SCANNER_API int s_getpos(short iDevice,short iAxis,char cPos[20]);


    //移动设备过程中产生TTL电平
    /// <summary>
    ///Date           : 2020-04-24
    ///Autor          : xxt
    ///Description    : 转台单行扫描
    /// </summary>
    /// <param name="iDevice">设备号</param>
    /// <param name="iAxis">轴号</param>
    /// <param name="fStartPos">起始位置</param>
    /// <param name="fEndPos">终止位置</param>
    /// <param name="fStepPos">脉冲间隔</param>
    /// <param name="fSpeedToStart">至起始位置时的运行速度</param>
    /// <param name="fSpeedToEnd">至终止位置时的运行速度</param>
    /// <param name="fDeltaStep">偏移，恒定为0，代表不偏移</param>
    /// <param name="iTime">停顿时间，设为0，代表连续扫描；非0表示步进扫描</param>
    ///
    //返回值：0-通信成功，非0为通信失败。

    SCANNER_API int s_MoveByType(short iDevice, short iAxis, double fStartPos, double fEndPos, double fStepPos, double fSpeedToStart, double fSpeedToEnd, double fDeltaStep, int iTime);
}

#endif // INTERFACE_H
