package com.avp.config.controller.alarm;

import com.avp.config.entity.alarm.AlarmSourceTbl;
import com.avp.config.entity.alarm.vw.AlarmSourceActTypeVw;
import com.avp.config.service.alarm.AlarmSourceTblService;
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
 * Created by zhoujs on 2018/6/14.
 */
@Controller
@RequestMapping(value = "/alarmSource")
public class AlarmSourceController {
    private Logger logger = LoggerFactory.getLogger(AlarmTypeController.class);

    @Autowired
    private AlarmSourceTblService alarmSourceTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(AlarmSourceTbl alarmSourceTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<AlarmSourceActTypeVw> list = this.alarmSourceTblService.getList(alarmSourceTbl);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询AlarmSourceTbl 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(AlarmSourceTbl alarmSourceTbl, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        if (StringUtils.isNotBlank(operate) && operate.equals("update")) {
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        } else {
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            AlarmSourceTbl alarmSourceTbl1 = this.alarmSourceTblService.saveOrUpdate(alarmSourceTbl, operate);

            ajaxPara.setObj(new Gson().toJson(alarmSourceTbl1));
        } catch (Exception e) {
            logger.error("alarmSourceTbl保存或修改出错");
            ajaxPara.setRetFlag(false);
            if (e.getMessage() != null) {
                ajaxPara.setMsg(e.getMessage());
            } else {
                ajaxPara.setMsg("alarmSource保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(AlarmSourceTbl alarmSourceTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            AlarmSourceTbl alarmSourceTbl1 = this.alarmSourceTblService.detail(alarmSourceTbl.getSourceId());

            ajaxPara.setObj(new Gson().toJson(alarmSourceTbl1));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询alarmSourceTbl1 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(AlarmSourceTbl alarmSourceTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.alarmSourceTblService.del(alarmSourceTbl.getSourceId());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
