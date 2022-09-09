package com.avp.config.service;

import java.util.List;

import com.avp.config.entity.dto.GetSignalInsatnceListDto;
import com.avp.config.entity.vo.DevCopyVo;

public interface SignalInstanceService<T> {
	
	/**
	 * 
	 * @Title: doPointInstance
	 * @Description: 删除信号实例
	 * @author Cookie
	 * @date 2018年3月29日 上午9:53:52
	 * @company 上海艾文普信息技术有限公司
	 * @param signalLabel 信号标签
	 * @throws Exception 
	 */
	public void deleteSignalInstance(String signalLabel) throws Exception;
	
	/**
	 * 
	 * @Title: addPointInstance
	 * @Description: 增加特殊信号实例
	 * @author Cookie
	 * @date 2018年3月29日 上午10:30:36
	 * @company 上海艾文普信息技术有限公司
	 * @throws Exception
	 */
	public T saveSignalInstance(int type,T t) throws Exception;
	
	/**
	 * 
	 * @Title: updatePointInstance
	 * @Description: 修改特殊信号实例
	 * @author Cookie
	 * @date 2018年3月29日 上午10:31:56
	 * @company 上海艾文普信息技术有限公司
	 * @throws Exception
	 */
	public Object updateSignalInstance(int type,Object o) throws Exception;
	
	/**
	 * 
	 * @Title: queryPointInstance
	 * @Description: 查询单个信号实例详情
	 * @author Cookie
	 * @date 2018年3月29日 上午10:34:24
	 * @company 上海艾文普信息技术有限公司
	 * @param pointType
	 * @param signalLabel
	 * @throws Exception 
	 */
	public Object getSignalInstance(int pointType,String signalLabel) throws Exception;
	
	/**
	 * 
	 * @Title: listPointInsatnceByDev
	 * @Description: 查询某个设备实例的所有信号实例
	 * @author Cookie
	 * @date 2018年3月29日 上午10:45:06
	 * @company 上海艾文普信息技术有限公司
	 * @param devLabel
	 * @return 
	 */
	public List<GetSignalInsatnceListDto> getSignalInstanceList(String devLabel);
	
	/**
	 * 
	 * @Title: updateExtendsSignalInstance
	 * @Description: 修改继承信号实例
	 * @author Cookie
	 * @date 2018年4月4日 上午9:46:06
	 * @company 上海艾文普信息技术有限公司
	 * @param type
	 * @param o
	 * @return
	 * @throws Exception 
	 */
	public Object updateExtendsSignalInstance(int type, Object o) throws Exception;
	
	
}
