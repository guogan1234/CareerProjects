package com.avp.config.controller.user;

import com.avp.config.entity.user.PermSecurityDef;
import com.avp.config.service.user.PermSecurityDefService;
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
@RequestMapping(value = "permSecurity")
public class PermSecurityDefController {
    private Logger logger= LoggerFactory.getLogger(PermSecurityDefController.class);
    @Autowired
    private PermSecurityDefService permSecurityDefService;


    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(PermSecurityDef permSecurityDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<PermSecurityDef> list=this.permSecurityDefService.getList(permSecurityDef);
            if (list != null && list.size() > 0) {
                ajaxPara.setObj(new Gson().toJson(list));
            }
        }catch (Exception e){
            logger.error("查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(PermSecurityDef permSecurityDef, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            PermSecurityDef permSecurityDef2=this.permSecurityDefService.saveOrUpdate(permSecurityDef,operate);
            if (permSecurityDef2 != null) {
                ajaxPara.setObj(new Gson().toJson(permSecurityDef2));
            }
        }catch (Exception e){
            logger.error("保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(PermSecurityDef permSecurityDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            PermSecurityDef permSecurityDef2=this.permSecurityDefService.detail(permSecurityDef.getPermSecurityId());
            if (permSecurityDef2 != null) {
                ajaxPara.setObj(new Gson().toJson(permSecurityDef2));
            }
        }catch (Exception e){
            logger.error("查询详情出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("查询详情出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(PermSecurityDef permSecurityDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.permSecurityDefService.del(permSecurityDef.getPermSecurityId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}

