package com.avp.config.controller.user;

import com.avp.config.entity.user.PermSpecGroupStationRealation;
import com.avp.config.entity.user.vw.PermSpecGroupStationVw;
import com.avp.config.service.user.PermSpecGroupStationRealationService;
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
@RequestMapping(value = "/permSpecGroupStationRealation")
public class PermSpecGroupStationRealationController {

    private Logger logger = LoggerFactory.getLogger(PermSpecGroupStationRealationController.class);

    @Autowired
    private PermSpecGroupStationRealationService permSpecGroupStationRealationService;

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(PermSpecGroupStationRealation permSpecGroupStationRealation, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.permSpecGroupStationRealationService.saveOrUpdate(permSpecGroupStationRealation);
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
    public AjaxPara list(PermSpecGroupStationRealation permSpecGroupStationRealation) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<PermSpecGroupStationVw> list = this.permSpecGroupStationRealationService.getList(permSpecGroupStationRealation);
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
    public AjaxPara del(PermSpecGroupStationRealation permSpecGroupStationRealation) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.permSpecGroupStationRealationService.del(permSpecGroupStationRealation.getRelationId());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

}
