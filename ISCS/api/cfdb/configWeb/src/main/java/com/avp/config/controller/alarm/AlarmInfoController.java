package com.avp.config.controller.alarm;

import com.avp.config.entity.alarm.AlarmInfoTbl;
import com.avp.config.entity.alarm.vw.AlarmInfoDomainStationRegionTypeLevelVw;
import com.avp.config.service.alarm.AlarmInfoTblService;
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
@RequestMapping(value = "/alarmInfo")
public class AlarmInfoController {

    private Logger logger = LoggerFactory.getLogger(AlarmInfoController.class);

    @Autowired
    private AlarmInfoTblService alarmInfoTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(AlarmInfoTbl alarmInfoTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<AlarmInfoDomainStationRegionTypeLevelVw> list = this.alarmInfoTblService.getList(alarmInfoTbl);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询AlarmInfo 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(AlarmInfoTbl alarmInfoTbl, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        if (StringUtils.isNotBlank(operate) && operate.equals("update")) {
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        } else {
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            AlarmInfoTbl alarmInfoTbl2 = this.alarmInfoTblService.saveOrUpdate(alarmInfoTbl, operate);

            ajaxPara.setObj(new Gson().toJson(alarmInfoTbl2));
        } catch (Exception e) {
            logger.error("alarmInfo保存或修改出错");
            ajaxPara.setRetFlag(false);
            if (e.getMessage() != null) {
                ajaxPara.setMsg(e.getMessage());
            } else {
                ajaxPara.setMsg("alarmInfo保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(AlarmInfoTbl alarmInfoTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            AlarmInfoTbl alarmInfoTbl2 = this.alarmInfoTblService.detail(alarmInfoTbl.getLabel(),alarmInfoTbl.getSecond(),alarmInfoTbl.getMsecond());

            ajaxPara.setObj(new Gson().toJson(alarmInfoTbl2));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询alarmInfo 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(AlarmInfoTbl alarmInfoTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.alarmInfoTblService.del(alarmInfoTbl.getLabel(),alarmInfoTbl.getSecond(),alarmInfoTbl.getMsecond());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
