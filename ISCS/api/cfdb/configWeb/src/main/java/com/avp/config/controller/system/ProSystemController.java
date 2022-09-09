package com.avp.config.controller.system;

import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.system.ProSystemTbl;
import com.avp.config.service.system.ProSystemTblService;
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
@RequestMapping("/proSystem")
public class ProSystemController {
    private Logger logger= LoggerFactory.getLogger(ProSystemController.class);

    @Autowired
    private ProSystemTblService proSystemTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(ProSystemTbl proSystemTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<ProSystemTbl> list=this.proSystemTblService.getList(proSystemTbl);
            if (list != null && list.size() > 0) {
                ajaxPara.setObj(new Gson().toJson(list));
            }
        }catch (Exception e){
            logger.error("proSystem查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("proSystem查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(ProSystemTbl proSystemTbl, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            ProSystemTbl proSystemTbl2=this.proSystemTblService.saveOrUpdate(proSystemTbl,operate);
            if (proSystemTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(proSystemTbl2));
            }
        }catch (Exception e){
            logger.error("proSystem保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("proSystem保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(ProSystemTbl ProSystemTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            ProSystemTbl proSystemTbl2=this.proSystemTblService.detail(ProSystemTbl.getProSystemId());
            if (proSystemTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(proSystemTbl2));
            }
        }catch (Exception e){
            logger.error("proSystem查询详情出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("proSystem查询详情出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(ProSystemTbl proSystemTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.proSystemTblService.del(proSystemTbl.getProSystemId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }















}
