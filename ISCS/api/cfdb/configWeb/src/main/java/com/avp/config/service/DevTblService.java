package com.avp.config.service;

import java.util.List;

import com.avp.config.entity.DevTbl;
import com.avp.config.entity.dto.AddDevDto;
import com.avp.config.entity.vo.DevCopyVo;

public interface DevTblService {
	
	/**
	 * 
	 * @Title: addDev
	 * @Description: 增加设备实例
	 * @author Cookie
	 * @date 2018年3月28日 上午10:00:29
	 * @company 上海艾文普信息技术有限公司
	 * @param param
	 * @throws Exception 
	 */
	public AddDevDto addDev(DevTbl param) throws Exception;
	
	/**
	 * 
	 * @Title: delDev
	 * @Description: 删除模板实例
	 * @author Cookie
	 * @date 2018年3月28日 下午1:35:22
	 * @company 上海艾文普信息技术有限公司
	 * @param devLabel
	 */
	public void  delDev(String devLabel);
	
	/**
	 * 
	 * @Title: update
	 * @Description: 跟新设备实例
	 * @author Cookie
	 * @date 2018年3月28日 下午3:45:22
	 * @company 上海艾文普信息技术有限公司
	 * @param param
	 * @return
	 */
	public DevTbl updateDevTbl(DevTbl param);
	
	/**
	 * 
	 * @Title: queryDevTbl
	 * @Description: 查询设备实例
	 * @author Cookie
	 * @date 2018年3月28日 下午3:48:01
	 * @company 上海艾文普信息技术有限公司
	 * @param devLabel
	 * @return
	 */
	public DevTbl queryDevTbl(String devLabel);
	
	/**
	 * 
	 * @Title: getDevList
	 * @Description: 查询某个某设备模板下的（车站）设备实例集合
	 * @author Cookie
	 * @date 2018年3月29日 下午2:32:47
	 * @company 上海艾文普信息技术有限公司
	 * @param devTpLabel
	 * @param stationId
	 * @return
	 * @throws Exception 
	 */
	public List<DevTbl> getDevList(String devTpLabel,Integer stationId) throws Exception;
	
	/**
	 * 
	 * @Title: copySignalInstance
	 * @Description: 复制设备实例
	 * @author Cookie
	 * @date 2018年4月10日 上午11:27:13
	 * @company 上海艾文普信息技术有限公司
	 * @param vo
	 * @return
	 * @throws Exception
	 */
	public Object copyDevInstance(DevCopyVo vo) throws Exception;
	
	/**
	 * 
	 * @Title: copyStation
	 * @Description: 复制车站
	 * @author Cookie
	 * @date 2018年4月10日 下午3:32:03
	 * @company 上海艾文普信息技术有限公司
	 * @param vo
	 * @return
	 * @throws Exception
	 */
	public Object copyStation(DevCopyVo vo) throws Exception;
	
	/**
	 * 
	 * @Title: copyDevInstanceList
	 * @Description: 批量生成设备实例
	 * @author Cookie
	 * @date 2018年4月11日 上午10:49:40
	 * @company 上海艾文普信息技术有限公司
	 * @param vo
	 * @return
	 * @throws Exception
	 */
	public Object copyDevInstanceList(DevCopyVo vo) throws Exception;
}
