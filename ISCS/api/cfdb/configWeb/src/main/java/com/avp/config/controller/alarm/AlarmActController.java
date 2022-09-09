package com.avp.config.controller.alarm;

import com.avp.config.entity.alarm.AlarmActTbl;
import com.avp.config.service.alarm.AlarmActTblService;
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
 * Created by zhoujs on 2018/6/12.
 */
@Controller
@RequestMapping(value = "/alarmAct")
public class AlarmActController {
    private Logger logger = LoggerFactory.getLogger(AlarmActController.class);

    @Autowired
    private AlarmActTblService alarmActTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(AlarmActTbl alarmActTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<AlarmActTbl> list = this.alarmActTblService.getList(alarmActTbl);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询alarmActTbls 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(AlarmActTbl alarmActTbl, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        if (StringUtils.isNotBlank(operate) && operate.equals("update")) {
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        } else {
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            AlarmActTbl alarmActTbl1 = this.alarmActTblService.saveOrUpdate(alarmActTbl, operate);

            ajaxPara.setObj(new Gson().toJson(alarmActTbl1));
        } catch (Exception e) {
            logger.error("alarmAct保存或修改出错");
            ajaxPara.setRetFlag(false);
            if (e.getMessage() != null) {
                ajaxPara.setMsg(e.getMessage());
            } else {
                ajaxPara.setMsg("alarmAct保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(AlarmActTbl alarmActTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            AlarmActTbl alarmActTbl1 = this.alarmActTblService.detail(alarmActTbl.getActId());

            ajaxPara.setObj(new Gson().toJson(alarmActTbl1));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询alarmActTbl 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(AlarmActTbl alarmActTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.alarmActTblService.del(alarmActTbl.getActId());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }


}
