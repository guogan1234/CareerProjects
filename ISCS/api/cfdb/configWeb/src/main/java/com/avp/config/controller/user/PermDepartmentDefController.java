package com.avp.config.controller.user;

import com.avp.config.entity.user.PermDepartmentDef;
import com.avp.config.service.user.PermDepartmentDefService;
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
 * Created by zhoujs on 2018/6/21.
 */
@Controller
@RequestMapping(value = "/permDepartment")
public class PermDepartmentDefController {

    private Logger logger = LoggerFactory.getLogger(PermDepartmentDefController.class);

    @Autowired
    private PermDepartmentDefService permDepartmentDefService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(PermDepartmentDef permDepartmentDef) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<PermDepartmentDef> list = this.permDepartmentDefService.getList(permDepartmentDef);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询PermDepartment 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(PermDepartmentDef permDepartmentDef, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        if (StringUtils.isNotBlank(operate) && operate.equals("update")) {
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        } else {
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            PermDepartmentDef permDepartmentDef1= this.permDepartmentDefService.saveOrUpdate(permDepartmentDef, operate);

            ajaxPara.setObj(new Gson().toJson(permDepartmentDef1));
        } catch (Exception e) {
            logger.error("permDepartment保存或修改出错");
            ajaxPara.setRetFlag(false);
            if (e.getMessage() != null) {
                ajaxPara.setMsg(e.getMessage());
            } else {
                ajaxPara.setMsg("permDepartment保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(PermDepartmentDef permDepartmentDef) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            PermDepartmentDef permDepartmentDef1 = this.permDepartmentDefService.detail(permDepartmentDef.getDeptId());

            ajaxPara.setObj(new Gson().toJson(permDepartmentDef1));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询permDepartmentDef 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(PermDepartmentDef permDepartmentDef) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.permDepartmentDefService.del(permDepartmentDef.getDeptId());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
