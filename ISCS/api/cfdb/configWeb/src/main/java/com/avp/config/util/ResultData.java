package com.avp.config.util;

import com.google.gson.Gson;

public class ResultData<T> {
	/*
	 * 结果
	 */
	private boolean retFlag = true;
	/*
	 * 数据
	 */
	private Object obj;
	/*
	 * 信息
	 */
	private String msg = "调用成功！";
	
	public ResultData() {
		
	}
	
	public ResultData(Object obj) {
		this.obj = new Gson().toJson(obj);
	}
	
	public ResultData(boolean retFlag,Object obj) {
		this.retFlag = retFlag;
		this.obj = new Gson().toJson(obj);
	} 
	
	public ResultData(boolean retFlag,Object obj,String msg) {
		this.retFlag = retFlag;
		this.obj = new Gson().toJson(obj);
		this.msg = msg;
	} 
	
	public boolean isRetFlag() {
		return retFlag;
	}
	public void setRetFlag(boolean retFlag) {
		this.retFlag = retFlag;
	}
	public Object getObj() {
		return obj;
	}
	public void setObj(Object obj) {
		this.obj = new Gson().toJson(obj);
	}
	public String getMsg() {
		return msg;
	}
	public void setMsg(String msg) {
		this.msg = msg;
	}
}
