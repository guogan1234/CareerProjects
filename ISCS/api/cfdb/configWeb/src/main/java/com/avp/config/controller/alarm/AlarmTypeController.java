package com.avp.config.controller.alarm;

import com.avp.config.entity.alarm.AlarmTypeTbl;
import com.avp.config.service.alarm.AlarmTypeTblService;
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
@RequestMapping("/alarmType")
public class AlarmTypeController {
    private Logger logger = LoggerFactory.getLogger(AlarmTypeController.class);

    @Autowired
    private AlarmTypeTblService alarmTypeTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(AlarmTypeTbl alarmTypeTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<AlarmTypeTbl> list = this.alarmTypeTblService.getList(alarmTypeTbl);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询alarmTypeTbl 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(AlarmTypeTbl alarmTypeTbl, HttpServletRequest request) {
        String operate = request.getParameter("operate");
        AjaxPara ajaxPara = new AjaxPara();
        if (StringUtils.isNotBlank(operate) && operate.equals("update")) {
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        } else {
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            AlarmTypeTbl alarmTypeTbl1 = this.alarmTypeTblService.saveOrUpdate(alarmTypeTbl, operate);

            ajaxPara.setObj(new Gson().toJson(alarmTypeTbl1));
        } catch (Exception e) {
            logger.error("alarmTypeTbl保存或修改出错");
            ajaxPara.setRetFlag(false);
            if (e.getMessage() != null) {
                ajaxPara.setMsg(e.getMessage());
            } else {
                ajaxPara.setMsg("alarmType保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(AlarmTypeTbl alarmTypeTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            AlarmTypeTbl alarmTypeTbl1 = this.alarmTypeTblService.detail(alarmTypeTbl.getTypeId());

            ajaxPara.setObj(new Gson().toJson(alarmTypeTbl1));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询alarmTypeTbl1 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(AlarmTypeTbl alarmTypeTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            this.alarmTypeTblService.del(alarmTypeTbl.getTypeId());
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

}
