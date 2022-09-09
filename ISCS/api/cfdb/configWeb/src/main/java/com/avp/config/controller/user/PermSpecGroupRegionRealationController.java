package com.avp.config.controller.user;

import com.avp.config.entity.user.PermSpecGroupRegionRealation;
import com.avp.config.entity.user.vw.PermSpecGroupRegionVw;
import com.avp.config.service.user.PermSpecGroupRegionRealationService;
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
 * Created by zhoujs on 2018/6/28.
 */
@Controller
@RequestMapping(value = "/permSpecGroupRegionRealation")
public class PermSpecGroupRegionRealationController {

    private Logger logger = LoggerFactory.getLogger(PermSpecGroupRegionRealationController.class);
    @Autowired
    private PermSpecGroupRegionRealationService permSpecGroupRegionRealationService;

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(PermSpecGroupRegionRealation permSpecGroupRegionRealation, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.permSpecGroupRegionRealationService.saveOrUpdate(permSpecGroupRegionRealation);
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
    public AjaxPara list(PermSpecGroupRegionRealation permSpecGroupRegionRealation) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<PermSpecGroupRegionVw> list = this.permSpecGroupRegionRealationService.getList(permSpecGroupRegionRealation);
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
    public AjaxPara del(PermSpecGroupRegionRealation permSpecGroupRegionRealation) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.permSpecGroupRegionRealationService.del(permSpecGroupRegionRealation.getRelationId());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
