package com.avp.config.controller;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.avp.config.entity.DevTbl;
import com.avp.config.entity.dto.AddDevDto;
import com.avp.config.entity.vo.DevCopyVo;
import com.avp.config.service.DevTblService;
import com.avp.config.util.ResultData;

@Controller
@RequestMapping("/devInstance")
public class DevInstanceController {

    private Logger logger= LoggerFactory.getLogger(PointController.class);
    
    @Autowired
    private DevTblService devTblService;
    
	@RequestMapping(value="/addDev",method=RequestMethod.POST)
	@ResponseBody
	public ResultData<AddDevDto> addDev(HttpServletRequest request,DevTbl devTbl){
		ResultData<AddDevDto> res = new ResultData<>();;
		try {
			res.setObj(devTblService.addDev(devTbl));
		} catch (Exception e) {
			logger.info("增加设备实例出错："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
	
	@RequestMapping(value="/delDev",method=RequestMethod.POST)
	@ResponseBody
	public ResultData<DevTbl> delDev(DevTbl devTbl){
		ResultData<DevTbl> res = new ResultData<>();
		try {
			devTblService.delDev(devTbl.getDevLabel());
		} catch (Exception e) {
			logger.info("删除设备实例出错："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
	
	@RequestMapping(value="/updateDev",method=RequestMethod.POST)
	@ResponseBody
	public ResultData<DevTbl> updateDev(HttpServletRequest request,DevTbl devTbl){
		ResultData<DevTbl> res = new ResultData<>();
		try {
			String json=request.getParameter("json");
			res.setObj(devTblService.updateDevTbl(devTbl));
		} catch (Exception e) {
			logger.info("修改设备实例出错："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
	
	@RequestMapping(value="/getDevDetail",method=RequestMethod.POST)
	@ResponseBody
	public ResultData<DevTbl> getDevDetail(DevTbl devTbl){
		ResultData<DevTbl> res = new ResultData<>();
		try {
			res.setObj(devTblService.queryDevTbl(devTbl.getDevLabel()));
		} catch (Exception e) {
			logger.info("查询设备实例出错："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
	
	@RequestMapping(value="/getDevList",method=RequestMethod.POST)
    @ResponseBody
	public ResultData<List<DevTbl>> getDevList(DevTbl devTbl){
		ResultData<List<DevTbl>> res = new ResultData<>();;
		try {
			res.setObj(devTblService.getDevList(devTbl.getDevTpLabel(), devTbl.getStationId()));
		} catch (Exception e) {
			logger.info("查询所有设备实例出错："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
	
	@RequestMapping(value="/copyDevInstance",method=RequestMethod.POST)
    @ResponseBody
	public ResultData<Object> copyDevInstance(DevCopyVo vo){
		ResultData<Object> res = new ResultData<>();;
		try {
			res.setObj(devTblService.copyDevInstance(vo));
		} catch (Exception e) {
			logger.info("复制设备实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
	
	@RequestMapping(value="/copyStation",method=RequestMethod.POST)
    @ResponseBody
	public ResultData<Object> copyStation(DevCopyVo vo){
		ResultData<Object> res = new ResultData<>();;
		try {
			res.setObj(devTblService.copyStation(vo));
		} catch (Exception e) {
			logger.info("复制车站失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
	

}
