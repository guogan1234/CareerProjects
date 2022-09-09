package com.avp.config.controller;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import com.avp.config.entity.vo.AccSignalInstanceVo;
import com.avp.config.entity.vo.AioSignalInstanceVo;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.avp.config.entity.dto.GetSignalInsatnceListDto;
import com.avp.config.entity.vo.DioSignalInstanceVo;
import com.avp.config.entity.vo.SignalInstanceVo;
import com.avp.config.service.SignalInstanceService;
import com.avp.config.util.ResultData;
import com.avp.config.util.UniversalStatus;

/**
 * 信号实例
 */
@Controller
@RequestMapping("/signalInstance")
public class SignalInstanceController {
	
    private Logger logger= LoggerFactory.getLogger(SignalInstanceController.class);
    
    @Autowired 
    private SignalInstanceService signalInstanceService;
    
	@RequestMapping(value="/getSignaInsatnceList",method=RequestMethod.POST)
	@ResponseBody
	public ResultData<List<GetSignalInsatnceListDto>> getSignaInsatnceList(HttpServletRequest request){
		ResultData<List<GetSignalInsatnceListDto>> res = new ResultData<>();;
		try {
            String devLabel=request.getParameter("devLabel");
			res.setObj(signalInstanceService.getSignalInstanceList(devLabel));
		} catch (Exception e) {
			logger.info("根据设备ID查询信号实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
	
	@RequestMapping(value="/getDetail",method=RequestMethod.POST)
	@ResponseBody
	public ResultData<Object> getDetail(SignalInstanceVo signalInstanceVo){
		ResultData<Object> res = new ResultData<>();;
		try {
			res.setObj(signalInstanceService.getSignalInstance(signalInstanceVo.getSignalType(), signalInstanceVo.getSignalLabel()));
		} catch (Exception e) {
			logger.info("查询信号实例详情失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
	
	@RequestMapping(value="/delSignalInstance",method=RequestMethod.POST)
	@ResponseBody
	public ResultData<Object> deleteSignaInstance(SignalInstanceVo getSignaInstanceVo){
		ResultData<Object> res = new ResultData<>();;
		try {
			signalInstanceService.deleteSignalInstance(getSignaInstanceVo.getSignalLabel());
		} catch (Exception e) {
			logger.info("删除信号实例详情失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}

	@RequestMapping(value = "/updateDioSignalInstance",method = RequestMethod.POST)
	@ResponseBody
	public ResultData<DioSignalInstanceVo> updateDioSignalInstance(DioSignalInstanceVo dioSignalInstanceVo){
		ResultData<DioSignalInstanceVo> res = new ResultData<>();;
		try {
			res.setObj(this.signalInstanceService.updateSignalInstance(UniversalStatus.SIGNAL_DIO,dioSignalInstanceVo));
		} catch (Exception e) {
			logger.info("修改信号实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}

    @RequestMapping(value = "/updateInheritDioSignalInstance",method = RequestMethod.POST)
    @ResponseBody
    public ResultData<DioSignalInstanceVo> updateInheritDioSignalInstance(DioSignalInstanceVo dioSignalInstanceVo){
        ResultData<DioSignalInstanceVo> res = new ResultData<>();;
        try {
            res.setObj(this.signalInstanceService.updateExtendsSignalInstance(UniversalStatus.SIGNAL_DIO,dioSignalInstanceVo));
        } catch (Exception e) {
            logger.info("修改信号实例失败："+e.getMessage());
            res.setRetFlag(false);
            res.setMsg(e.getMessage());
        }
        return res;
    }



	@RequestMapping(value = "/updateAioSignalInstance",method = RequestMethod.POST)
	@ResponseBody
	public ResultData<AioSignalInstanceVo> updateAioSignalInstance(AioSignalInstanceVo aioSignalInstanceVo){
		ResultData<AioSignalInstanceVo> res = new ResultData<>();;
		try {
			res.setObj(this.signalInstanceService.updateSignalInstance(UniversalStatus.SIGNAL_AIO,aioSignalInstanceVo));
		} catch (Exception e) {
			logger.info("修改信号实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}

	@RequestMapping(value = "/updateInheritAioSignalInstance",method = RequestMethod.POST)
	@ResponseBody
	public ResultData<AioSignalInstanceVo> updateInheritAioSignalInstance(AioSignalInstanceVo aioSignalInstanceVo){
		ResultData<AioSignalInstanceVo> res = new ResultData<>();;
		try {
			res.setObj(this.signalInstanceService.updateExtendsSignalInstance(UniversalStatus.SIGNAL_AIO,aioSignalInstanceVo));
		} catch (Exception e) {
			logger.info("修改信号实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}

	@RequestMapping(value = "/updateAccSignalInstance",method = RequestMethod.POST)
	@ResponseBody
	public ResultData<AccSignalInstanceVo> updateAccSignalInstance(AccSignalInstanceVo accSignalInstanceVo){
		ResultData<AccSignalInstanceVo> res = new ResultData<>();;
		try {
			res.setObj(this.signalInstanceService.updateSignalInstance(UniversalStatus.SIGNAL_ACC,accSignalInstanceVo));
		} catch (Exception e) {
			logger.info("修改信号实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}

	@RequestMapping(value = "/updateInheritAccSignalInstance",method = RequestMethod.POST)
	@ResponseBody
	public ResultData<AccSignalInstanceVo> updateInheritAccSignalInstance(AccSignalInstanceVo accSignalInstanceVo){
		ResultData<AccSignalInstanceVo> res = new ResultData<>();;
		try {
			res.setObj(this.signalInstanceService.updateExtendsSignalInstance(UniversalStatus.SIGNAL_ACC,accSignalInstanceVo));
		} catch (Exception e) {
			logger.info("修改信号实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}

	@RequestMapping(value = "/saveDioSignalInstance",method = RequestMethod.POST)
	@ResponseBody
	public ResultData<DioSignalInstanceVo> saveDioSignalInstance(HttpServletRequest request, DioSignalInstanceVo dioVo){
		ResultData<DioSignalInstanceVo> res = new ResultData<>();;
		try {
			res.setObj(this.signalInstanceService.saveSignalInstance(UniversalStatus.SIGNAL_DIO,dioVo));
		} catch (Exception e) {
			logger.info("增加信号实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}

	@RequestMapping(value = "/saveAioSignalInstance",method = RequestMethod.POST)
	@ResponseBody
	public ResultData<AioSignalInstanceVo> saveAioSignalInstance(HttpServletRequest request, AioSignalInstanceVo aioVo){
		ResultData<AioSignalInstanceVo> res = new ResultData<>();;
		try {
			res.setObj(this.signalInstanceService.saveSignalInstance(UniversalStatus.SIGNAL_AIO,aioVo));
		} catch (Exception e) {
			logger.info("增加信号实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}

	@RequestMapping(value = "/saveAccSignalInstance",method = RequestMethod.POST)
	@ResponseBody
	public ResultData<AccSignalInstanceVo> saveAccSignalInstance(HttpServletRequest request, AccSignalInstanceVo accVo){
		ResultData<AccSignalInstanceVo> res = new ResultData<>();;
		try {
			res.setObj(this.signalInstanceService.saveSignalInstance(UniversalStatus.SIGNAL_ACC,accVo));
		} catch (Exception e) {
			logger.info("增加信号实例失败："+e.getMessage());
			res.setRetFlag(false);
			res.setMsg(e.getMessage());
		}
		return res;
	}
}
