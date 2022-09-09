package com.avp.config.controller.user;

import com.avp.config.entity.user.PermTypeDef;
import com.avp.config.service.user.PermTypeDefService;
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
 * Created by zhoujs on 2018/6/22.
 */
@Controller
@RequestMapping(value = "/permType")
public class PermTypeDefController {

    private Logger logger = LoggerFactory.getLogger(PermTypeDef.class);

    @Autowired
    private PermTypeDefService permTypeDefService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(PermTypeDef permTypeDef) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<PermTypeDef> list = this.permTypeDefService.getList(permTypeDef);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询PermTypeDef 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(PermTypeDef permTypeDef, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        if (StringUtils.isNotBlank(operate) && operate.equals("update")) {
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        } else {
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            PermTypeDef permTypeDef1= this.permTypeDefService.saveOrUpdate(permTypeDef, operate);

            ajaxPara.setObj(new Gson().toJson(permTypeDef1));
        } catch (Exception e) {
            logger.error("permTypeDef1保存或修改出错");
            ajaxPara.setRetFlag(false);
            if (e.getMessage() != null) {
                ajaxPara.setMsg(e.getMessage());
            } else {
                ajaxPara.setMsg("permTypeDef1保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(PermTypeDef permTypeDef) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            PermTypeDef permTypeDef1 = this.permTypeDefService.detail(permTypeDef.getPermTypeId());

            ajaxPara.setObj(new Gson().toJson(permTypeDef1));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询permTypeDef 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(PermTypeDef permTypeDef) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.permTypeDefService.del(permTypeDef.getPermTypeId());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
