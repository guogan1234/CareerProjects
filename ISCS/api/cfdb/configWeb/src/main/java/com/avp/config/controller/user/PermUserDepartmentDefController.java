package com.avp.config.controller.user;

import com.avp.config.entity.user.PermUserDef;
import com.avp.config.entity.user.PermUserDepartmentDef;
import com.avp.config.entity.user.vw.UserDepartmentVw;
import com.avp.config.service.user.PermUserDepartmentService;
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
@RequestMapping(value = "permUserDepartment")
public class PermUserDepartmentDefController {
    private Logger logger= LoggerFactory.getLogger(PermUserDepartmentDefController.class);

    @Autowired
    private PermUserDepartmentService permUserDepartmentService;

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(PermUserDepartmentDef permUserDepartmentDef, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
           this.permUserDepartmentService.saveOrUpdate(permUserDepartmentDef);
        }catch (Exception e){
            logger.error("保存出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("保存出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(PermUserDepartmentDef permUserDepartmentDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<UserDepartmentVw> list=this.permUserDepartmentService.getList(permUserDepartmentDef);
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
    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(PermUserDepartmentDef permUserDepartmentDef){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.permUserDepartmentService.del(permUserDepartmentDef.getRelationId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
