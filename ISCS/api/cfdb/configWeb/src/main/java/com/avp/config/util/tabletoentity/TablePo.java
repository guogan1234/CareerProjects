package com.avp.config.util.tabletoentity;

/**
 * 
* @ClassName: tablePo
* @Description: 表的实体类
* @author Cookie
* @date 2018年3月27日 上午8:57:41
* @company 上海艾文普信息技术有限公司
*
 */
public class TablePo {
	
	/*
	 * 字段英文名
	 */
	private String eName;
	
	/*
	 * 字段中文名
	 */
	private String cName;
	/*
	 * 字段类型
	 */
	private String type;
	/*
	 * 字段长度
	 */
	private String length;
	
	public TablePo(String eName,String cName,String type,String length) {
		this.eName = eName;
		this.cName = cName;
		this.type = type;
		this.length = length;
	}
	
	public String geteName() {
		return eName;
	}
	public void seteName(String eName) {
		this.eName = eName;
	}
	public String getcName() {
		return cName;
	}
	public void setcName(String cName) {
		this.cName = cName;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	public String getLength() {
		return length;
	}
	public void setLength(String length) {
		this.length = length;
	}
}
