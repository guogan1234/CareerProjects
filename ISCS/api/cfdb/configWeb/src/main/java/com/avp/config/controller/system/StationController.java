package com.avp.config.controller.system;

import com.avp.config.entity.system.ProSystemTbl;
import com.avp.config.entity.system.StationTbl;
import com.avp.config.service.system.StationTblService;
import com.avp.config.util.AjaxPara;
import com.avp.config.util.ResponseCode;
import com.google.gson.Gson;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

@Controller
@RequestMapping("/station")
public class StationController {
    private Logger logger= LoggerFactory.getLogger(StationController.class);

    @Autowired
    private StationTblService stationTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(StationTbl stationTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<StationTbl> list=this.stationTblService.getList(stationTbl);
            if (list != null && list.size() > 0) {
                ajaxPara.setObj(new Gson().toJson(list));
            }
        }catch (Exception e){
            logger.error("station查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("station查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(StationTbl stationTbl, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            StationTbl stationTbl2=this.stationTblService.saveOrUpdate(stationTbl,operate);
            if (stationTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(stationTbl2));
            }
        }catch (Exception e){
            logger.error("station保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("station保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(StationTbl stationTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            StationTbl stationTbl2=this.stationTblService.detail(stationTbl.getStationId());
            if (stationTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(stationTbl2));
            }
        }catch (Exception e){
            logger.error("station查询详情出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("station查询详情出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(StationTbl stationTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.stationTblService.del(stationTbl.getStationId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }














}
