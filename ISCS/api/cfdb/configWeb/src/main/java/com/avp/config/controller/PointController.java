package com.avp.config.controller;
import com.avp.config.entity.*;
import com.avp.config.entity.vo.DevCopyVo;
import com.avp.config.service.DioTpTblService;
import com.avp.config.service.PointService;
import com.avp.config.util.AjaxPara;
import com.avp.config.util.ResponseCode;
import com.avp.config.util.UniversalStatus;
import com.google.gson.Gson;
import net.sf.json.JSONArray;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import javax.servlet.http.HttpServletRequest;
import java.sql.Timestamp;
import java.util.List;

/**
 * 点模版
 */
@Controller
@RequestMapping(value = "/point")
public class PointController {
    private Logger logger= LoggerFactory.getLogger(PointController.class);

    @Autowired
    private PointService pointService;

    @RequestMapping(value = "/pointList",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara pointList(HttpServletRequest request, Model model){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        String devId=request.getParameter("devId");
        try {
            if (StringUtils.isNotBlank(devId)) {
                List<PointEntity> list = this.pointService.getList(devId);
                if (list != null && list.size() > 0) {
                    ajaxPara.setObj( new Gson().toJson(list));
                }
            }
        }catch (Exception e){
            logger.error("devList查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("查询设备列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveDioPoint")
    @ResponseBody
    public AjaxPara saveDioPoint(HttpServletRequest request,DioTpTbl dioTpTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            dioTpTbl.setDioTpLabel(dioTpTbl.getDevTpLabel()+":"+dioTpTbl.getDioTpLabel());
            ajaxPara.setObj(new Gson().toJson(this.pointService.save(dioTpTbl, UniversalStatus.POINT_DIO)));
        }catch (Exception e){
            logger.error("保存出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return  ajaxPara;
    }

    @RequestMapping("/updateDioPoint")
    @ResponseBody
    public AjaxPara updateDioPoint(HttpServletRequest request,DioTpTbl dioTpTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            ajaxPara.setObj(new Gson().toJson(this.pointService.update(dioTpTbl, UniversalStatus.POINT_DIO)));
        }catch (Exception e){
            logger.error("修改出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return  ajaxPara;
    }

    @RequestMapping("/updateAioPoint")
    @ResponseBody
    public AjaxPara updateAioPoint(HttpServletRequest request,AioTpTbl aioTpTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            ajaxPara.setObj(new Gson().toJson(this.pointService.update(aioTpTbl, UniversalStatus.POINT_AIO)));
        }catch (Exception e){
            logger.error("修改出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return  ajaxPara;
    }

    @RequestMapping("/updateAccPoint")
    @ResponseBody
    public AjaxPara updateAccPoint(HttpServletRequest request,AccTpTbl accTpTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            ajaxPara.setObj(new Gson().toJson(this.pointService.update(accTpTbl, UniversalStatus.POINT_ACC)));
        }catch (Exception e){
            logger.error("修改出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return  ajaxPara;
    }



    @RequestMapping("/saveAioPoint")
    @ResponseBody
    public AjaxPara saveAioPoint(HttpServletRequest request,AioTpTbl aioTpTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            aioTpTbl.setAioTpLabel(aioTpTbl.getDevTpLabel()+":"+aioTpTbl.getAioTpLabel());
            ajaxPara.setObj(new Gson().toJson(this.pointService.save(aioTpTbl, UniversalStatus.POINT_AIO)));
        }catch (Exception e){
            logger.error("保存出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return  ajaxPara;
    }

    @RequestMapping("/saveAccPoint")
    @ResponseBody
    public AjaxPara saveAccPoint(HttpServletRequest request,AccTpTbl accTpTbl) throws Exception{
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            accTpTbl.setAccTpLabel(accTpTbl.getDevTpLabel()+":"+accTpTbl.getAccTpLabel());
            ajaxPara.setObj(new Gson().toJson(this.pointService.save(accTpTbl, UniversalStatus.POINT_ACC)));
        }catch (Exception e){
        	throw new Exception(e);
//            logger.error("保存出错"+e.getMessage());
//            ajaxPara.setRetFlag(false);
//            ajaxPara.setMsg(e.getMessage());
        }
        return  ajaxPara;
    }

    @RequestMapping(value = "/copy")
    @ResponseBody
    public AjaxPara copy(DevCopyVo vo){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            Object object = this.pointService.copy(vo);
            ajaxPara.setObj(new Gson().toJson(object));
        }catch (Exception e){
            logger.error("复制出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return  ajaxPara;
    }




    @RequestMapping("/getDetail")
    @ResponseBody
    public AjaxPara getDetail(HttpServletRequest request){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        String id=request.getParameter("id");
        String type=request.getParameter("type");
        try{
            Object object = this.pointService.getDetail(id,type);
            ajaxPara.setObj(new Gson().toJson(object));
        }catch (Exception e){
            logger.error("保存出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return  ajaxPara;
    }


    @RequestMapping(value = "/delPointTemplate",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara delPointTemplate(HttpServletRequest request){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        String id=request.getParameter("id");
        try{
            if(StringUtils.isBlank(id)){
                ajaxPara.setMsg("标签不能为空!");
                ajaxPara.setRetFlag(false);
                return ajaxPara;
            }
            this.pointService.del(id);
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
