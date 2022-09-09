package com.avp.config.controller.base;


import com.avp.config.entity.base.DevTypeTbl;
import com.avp.config.entity.system.StationTbl;
import com.avp.config.service.base.DevTypeTblService;
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

/**
 * Created by zhoujs on 2018/5/17.
 */
@Controller
@RequestMapping("/devType")
public class DevTypeController {

    private Logger logger = LoggerFactory.getLogger(DevTypeController.class);

    @Autowired
    private DevTypeTblService devTypeTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(DevTypeTbl devTypeTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<DevTypeTbl> list=this.devTypeTblService.getList(devTypeTbl);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询devTypes 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(DevTypeTbl devTypeTbl, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
           DevTypeTbl devTypeTbl1=this.devTypeTblService.saveOrUpdate(devTypeTbl,operate);

            ajaxPara.setObj(new Gson().toJson(devTypeTbl1));
        } catch (Exception e) {
            logger.error("devType保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("devType保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(DevTypeTbl devTypeTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            DevTypeTbl devTypeTbl1=this.devTypeTblService.detail(devTypeTbl.getDevTypeLabel());

            ajaxPara.setObj(new Gson().toJson(devTypeTbl1));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询devTypes 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(DevTypeTbl devTypeTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.devTypeTblService.del(devTypeTbl.getDevTypeLabel());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

}
