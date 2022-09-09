package com.avp.config.controller.user;

import com.avp.config.controller.system.DomainController;
import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.user.PermUserDef;
import com.avp.config.service.user.PermUserDefService;
import com.avp.config.util.AjaxPara;
import com.avp.config.util.ResponseCode;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
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
@RequestMapping(value = "/user")
public class PermUserDefController {
    private Logger logger= LoggerFactory.getLogger(PermUserDefController.class);
    @Autowired
    private PermUserDefService permUserDefService;


    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(PermUserDef permUserDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<PermUserDef> list=this.permUserDefService.getList(permUserDef);
            if (list != null && list.size() > 0) {
                Gson gson = new GsonBuilder().setDateFormat("yyyy-MM-dd HH:mm:ss").create();
                ajaxPara.setObj(gson.toJson(list));
            }
        }catch (Exception e){
            logger.error("permUserDef查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("permUserDef查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(PermUserDef permUserDef, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            PermUserDef permUserDef2=this.permUserDefService.saveOrUpdate(permUserDef,operate);
            if (permUserDef2 != null) {
                ajaxPara.setObj(new Gson().toJson(permUserDef2));
            }
        }catch (Exception e){
            logger.error("permUserDef保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("permUserDef保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(PermUserDef permUserDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            PermUserDef permUserDef2=this.permUserDefService.detail(permUserDef.getUserId());
            if (permUserDef2 != null) {
                ajaxPara.setObj(new Gson().toJson(permUserDef2));
            }
        }catch (Exception e){
            logger.error("permUserDef查询详情出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("permUserDef查询详情出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(PermUserDef permUserDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.permUserDefService.del(permUserDef.getUserId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/forbid",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara forbid(PermUserDef permUserDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.permUserDefService.forbid(permUserDef.getUserId());
        }catch (Exception e){
            logger.error("失效出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/lock",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara lock(PermUserDef permUserDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.permUserDefService.lock(permUserDef.getUserId());
        }catch (Exception e){
            logger.error("锁定出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
    @RequestMapping(value = "/cancelLock",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara cancelLock(PermUserDef permUserDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.permUserDefService.cancelLock(permUserDef.getUserId());
        }catch (Exception e){
            logger.error("解除锁定出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
