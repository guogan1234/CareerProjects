package com.avp.config.util;

/**
 * 
* @ClassName: BeanTools
* @Description: Bean拷贝工具
* @author Cookie
* @date 2018年3月28日 上午10:15:21
* @company 上海艾文普信息技术有限公司
*
 */
public  class BeanTools {

	/**
	 * 
	 * @Title: copy
	 * @Description: bean拷贝
	 * @author Cookie
	 * @date 2018年3月28日 上午10:15:37
	 * @company 上海艾文普信息技术有限公司
	 * @param source
	 * @param target
	 */
	public static void copy(Object source,Object target){
		org.springframework.beans.BeanUtils.copyProperties(source, target);
	}
}
