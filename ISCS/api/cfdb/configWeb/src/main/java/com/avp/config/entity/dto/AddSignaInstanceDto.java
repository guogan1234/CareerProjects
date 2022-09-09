package com.avp.config.entity.dto;

import com.avp.config.entity.po.AccPo;
import com.avp.config.entity.po.AioPo;
import com.avp.config.entity.po.DioPo;

public class AddSignaInstanceDto {
	private AccPo accPo;
	private AioPo aioPo;
	private DioPo dioPo;
	public AccPo getAccPo() {
		return accPo;
	}
	public void setAccPo(AccPo accPo) {
		this.accPo = accPo;
	}
	public AioPo getAioPo() {
		return aioPo;
	}
	public void setAioPo(AioPo aioPo) {
		this.aioPo = aioPo;
	}
	public DioPo getDioPo() {
		return dioPo;
	}
	public void setDioPo(DioPo dioPo) {
		this.dioPo = dioPo;
	}
}
