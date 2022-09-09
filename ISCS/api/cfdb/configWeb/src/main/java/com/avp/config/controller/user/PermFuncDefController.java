package com.avp.config.controller.user;

import com.avp.config.entity.user.PermFuncDef;
import com.avp.config.service.user.PermFuncDefService;
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
@RequestMapping(value ="permFunc")
public class PermFuncDefController {
    private Logger logger= LoggerFactory.getLogger(PermFuncDefController.class);
    @Autowired
    private PermFuncDefService permFuncDefService;


    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(PermFuncDef permFuncDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<PermFuncDef> list=this.permFuncDefService.getList(permFuncDef);
            if (list != null && list.size() > 0) {
                ajaxPara.setObj(new Gson().toJson(list));
            }
        }catch (Exception e){
            logger.error("permFuncDef查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("permFuncDef查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(PermFuncDef permFuncDef, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            PermFuncDef permFuncDef2=this.permFuncDefService.saveOrUpdate(permFuncDef,operate);
            if (permFuncDef2 != null) {
                ajaxPara.setObj(new Gson().toJson(permFuncDef2));
            }
        }catch (Exception e){
            logger.error("permFuncDef保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("permFuncDef保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(PermFuncDef permFuncDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            PermFuncDef permFuncDef2=this.permFuncDefService.detail(permFuncDef.getPermId());
            if (permFuncDef2 != null) {
                ajaxPara.setObj(new Gson().toJson(permFuncDef2));
            }
        }catch (Exception e){
            logger.error("permFuncDef查询详情出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("permFuncDef查询详情出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(PermFuncDef permFuncDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.permFuncDefService.del(permFuncDef.getPermId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
