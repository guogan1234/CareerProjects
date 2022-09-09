package com.avp.config.entity.po;

import com.avp.config.entity.DiTbl;
import com.avp.config.entity.DioDefTblEntity;
import com.avp.config.entity.DoTbl;

/**
 * 
* @ClassName: DioPo
* @Description: Dio模板的实例
* @author Cookie
* @date 2018年3月29日 上午10:55:46
* @company 上海艾文普信息技术有限公司
*
 */
public class DioPo {
	private DoTbl doTbl;
	private DiTbl diTbl;
	private DioDefTblEntity dioDefTblEntity;
	
	public DoTbl getDoTbl() {
		return doTbl;
	}
	public void setDoTbl(DoTbl doTbl) {
		this.doTbl = doTbl;
	}
	public DiTbl getDiTbl() {
		return diTbl;
	}
	public void setDiTbl(DiTbl diTbl) {
		this.diTbl = diTbl;
	}
	public DioDefTblEntity getDioDefTblEntity() {
		return dioDefTblEntity;
	}
	public void setDioDefTblEntity(DioDefTblEntity dioDefTblEntity) {
		this.dioDefTblEntity = dioDefTblEntity;
	}
}
