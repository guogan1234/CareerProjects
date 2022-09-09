package com.avp.config.controller.user;

import com.avp.config.entity.user.PermGroupStationDef;
import com.avp.config.entity.user.vw.GroupStationVw;
import com.avp.config.service.user.PermGroupStationDefService;
import com.avp.config.util.AjaxPara;
import com.avp.config.util.ResponseCode;
import com.google.gson.Gson;
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
 * Created by zhoujs on 2018/6/26.
 */
@Controller
@RequestMapping(value = "/permGroupStation")
public class PermGroupStationDefController {

    private Logger logger = LoggerFactory.getLogger(PermGroupStationDefController.class);

    @Autowired
    private PermGroupStationDefService permGroupStationDefService;

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(PermGroupStationDef permGroupStationDef, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.permGroupStationDefService.saveOrUpdate(permGroupStationDef);
        } catch (Exception e) {
            logger.error("保存出错");
            ajaxPara.setRetFlag(false);
            if (e.getMessage() != null) {
                ajaxPara.setMsg(e.getMessage());
            } else {
                ajaxPara.setMsg("保存出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(PermGroupStationDef permGroupStationDef) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<GroupStationVw> list = this.permGroupStationDefService.getList(permGroupStationDef);
            if (list != null && list.size() > 0) {
                ajaxPara.setObj(new Gson().toJson(list));
            }
        } catch (Exception e) {
            logger.error("查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(PermGroupStationDef permGroupStationDef) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.permGroupStationDefService.del(permGroupStationDef.getRelationId());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
