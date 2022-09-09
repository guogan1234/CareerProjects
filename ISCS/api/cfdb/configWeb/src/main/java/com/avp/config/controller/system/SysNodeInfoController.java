package com.avp.config.controller.system;

import com.avp.config.entity.system.StationTbl;
import com.avp.config.entity.system.SysNodeInfo;
import com.avp.config.service.system.StationTblService;
import com.avp.config.service.system.SysNodeInfoService;
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

@Controller
@RequestMapping(value = "/node")
public class SysNodeInfoController {
    private Logger logger= LoggerFactory.getLogger(SysNodeInfoController.class);

    @Autowired
    private SysNodeInfoService sysNodeInfoService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(SysNodeInfo sysNodeInfo){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<SysNodeInfo> list=this.sysNodeInfoService.getList(sysNodeInfo);
            if (list != null && list.size() > 0) {
                ajaxPara.setObj(new Gson().toJson(list));
            }
        }catch (Exception e){
            logger.error("sysNodeInfo查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("sysNodeInfo查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(SysNodeInfo sysNodeInfo, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            SysNodeInfo sysNodeInfo2=this.sysNodeInfoService.saveOrUpdate(sysNodeInfo,operate);
            if (sysNodeInfo2 != null) {
                ajaxPara.setObj(new Gson().toJson(sysNodeInfo2));
            }
        }catch (Exception e){
            logger.error("sysNodeInfo保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("sysNodeInfo保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(SysNodeInfo sysNodeInfo){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            SysNodeInfo sysNodeInfo2=this.sysNodeInfoService.detail(sysNodeInfo.getNodeId());
            if (sysNodeInfo2 != null) {
                ajaxPara.setObj(new Gson().toJson(sysNodeInfo2));
            }
        }catch (Exception e){
            logger.error("sysNodeInfo查询详情出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("sysNodeInfo查询详情出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(SysNodeInfo sysNodeInfo){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.sysNodeInfoService.del(sysNodeInfo.getNodeId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
