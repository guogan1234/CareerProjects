package com.avp.config.util;

/**
 * 通用状态枚举值
 */
public class UniversalStatus {
    //删除状态
    public final static int DELETE_STATUS=1;
    //未删除状态
    public final static int NOT_DELETE_STATUS=0;
    //车站
    public final static  String SELECT_STATION="ST";
    //专业下拉框
    public final  static  String SELECT_ZY="ZY";
    //设备类型下拉框
    public final static  String SELECT_DEV_TYPE="DT";
    //数字量文本下拉框
    public final static  String SELECT_DI_TEXT="DTL";
    //报警等级
    public final static  String SELECT_ALARM_LEVEL="AL";
    //报警类型
    public final static String SELECT_ALARM_TYPE="AT";
    //报警动作
    public final static String SELECT_ALARM_ACT="AC";
    //公式模版标签
    public final static String  SELECT_FORMULA="FM";
    //控制动作组标签
    public final static String  SELECT_DO_GRP="DG";
    //域
    public final static  String SELECT_DOMAIN="DM";
    //责任区
    public final static  String SELECT_REGION="RG";
    //单位
    public final static  String SELECT_UNIT="DW";
    //权限类型
    public final static  String SELECT_PERM_TYPE="PT";
    //部门
    public final  static  String SELECT_DEP="DEP";
    //用户
    public final static  String SELECT_USER="USER";
   //用户组
    public final static String SELECT_GROUP="GROUP";
    //普通权限
    public final static String SELECT_COMMON_FUNC="CF";
    //特殊权限
    public final static  String SELECT_SPEC_FUNC="SF";
    //ACC点模板
    public final static String POINT_ACC="ACC";
    //AIO点模板
    public final static  String POINT_AIO="AIO";
    //DIO点模板
    public final static String POINT_DIO="DIO";
    
	//DIO模板
	public static final int SIGNAL_DIO = 1;
	//AIO模板
	public static final int SIGNAL_AIO = 2;
	//ACC模板
	public static final int SIGNAL_ACC = 3;
	//MIX模板
	public static final int SIGNAL_MIX = 4;
	
	//不为特殊信号
	public static final int noSpecial = 0;
	//为特殊信号
	public static final int ifSpecial = 1;
	
	//数字自增类型
	public static final Integer numberAdd = 0;
	//字母自增类型
	public static final Integer letterAdd = 1;
}
