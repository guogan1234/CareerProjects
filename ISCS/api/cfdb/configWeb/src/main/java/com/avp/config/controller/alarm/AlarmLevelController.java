package com.avp.config.controller.alarm;

import com.avp.config.entity.alarm.AlarmLevelTbl;
import com.avp.config.service.alarm.AlarmLevelTblService;
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
 * Created by zhoujs on 2018/6/13.
 */
@Controller
@RequestMapping(value = "/alarmLevel")
public class AlarmLevelController {

    private Logger logger = LoggerFactory.getLogger(AlarmLevelController.class);

    @Autowired
    private AlarmLevelTblService alarmLevelTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(AlarmLevelTbl alarmLevelTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<AlarmLevelTbl> list = this.alarmLevelTblService.getList(alarmLevelTbl);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询alarmLevelTbls 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(AlarmLevelTbl alarmLevelTbl, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        if (StringUtils.isNotBlank(operate) && operate.equals("update")) {
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        } else {
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            AlarmLevelTbl alarmLevelTbl1 = this.alarmLevelTblService.saveOrUpdate(alarmLevelTbl, operate);

            ajaxPara.setObj(new Gson().toJson(alarmLevelTbl));
        } catch (Exception e) {
            logger.error("alarmLevelTbl保存或修改出错");
            ajaxPara.setRetFlag(false);
            if (e.getMessage() != null) {
                ajaxPara.setMsg(e.getMessage());
            } else {
                ajaxPara.setMsg("alarmLevelTbl保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(AlarmLevelTbl alarmLevelTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            AlarmLevelTbl alarmLevelTbl1 = this.alarmLevelTblService.detail(alarmLevelTbl.getLevelId());

            ajaxPara.setObj(new Gson().toJson(alarmLevelTbl1));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询alarmLevelTbl1 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(AlarmLevelTbl alarmLevelTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.alarmLevelTblService.del(alarmLevelTbl.getLevelId());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
