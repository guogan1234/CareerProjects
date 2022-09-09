package com.avp.config.controller.system;


import com.avp.config.entity.system.SysProcessInfo;
import com.avp.config.service.system.SysProcessInfoService;
import com.avp.config.util.AjaxPara;
import com.avp.config.util.ResponseCode;
import com.google.gson.Gson;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

/**
 * Created by zhoujs on 2018/5/17.
 */
@Controller
@RequestMapping("/process")
public class SysProcessInfoController {

    private Logger logger = LoggerFactory.getLogger(SysProcessInfoController.class);

    @Autowired
    private SysProcessInfoService sysProcessInfoService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(SysProcessInfo sysProcessInfo){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<SysProcessInfo> list=this.sysProcessInfoService.getList(sysProcessInfo);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            logger.error("查询sysProcessInfo 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(SysProcessInfo sysProcessInfo, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);

        try {
           SysProcessInfo sysProcessInfo1=this.sysProcessInfoService.saveOrUpdate(sysProcessInfo,operate);

            ajaxPara.setObj(new Gson().toJson(sysProcessInfo1));
        } catch (Exception e) {
            logger.error("保存sysProcessInfo 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(SysProcessInfo sysProcessInfo){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            SysProcessInfo sysProcessInfo1=this.sysProcessInfoService.detail(sysProcessInfo.getProcId());

            ajaxPara.setObj(new Gson().toJson(sysProcessInfo1));
        } catch (Exception e) {
            logger.error("查询sysProcessInfo 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping("/del")
    @ResponseBody
    public AjaxPara del(SysProcessInfo sysProcessInfo){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            this.sysProcessInfoService.del(sysProcessInfo.getProcId());
        } catch (Exception e) {
            logger.error("删除sysProcessInfo 报错!");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

}
