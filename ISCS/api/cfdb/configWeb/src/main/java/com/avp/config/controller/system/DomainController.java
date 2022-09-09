package com.avp.config.controller.system;

import com.avp.config.entity.DevTpTbl;
import com.avp.config.entity.system.DomainTbl;
import com.avp.config.service.system.DomainTblService;
import com.avp.config.util.AjaxPara;
import com.avp.config.util.ResponseCode;
import com.google.gson.Gson;
import net.sf.json.JSONArray;
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
@RequestMapping("/domain")
public class DomainController {
    private Logger logger= LoggerFactory.getLogger(DomainController.class);

    @Autowired
    private DomainTblService domainTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(DomainTbl domainTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<DomainTbl> list=this.domainTblService.getList(domainTbl);
            if (list != null && list.size() > 0) {
                ajaxPara.setObj(new Gson().toJson(list));
            }
        }catch (Exception e){
            logger.error("domain查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("domain查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(DomainTbl domainTbl, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            DomainTbl domainTbl2=this.domainTblService.saveOrUpdate(domainTbl,operate);
            if (domainTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(domainTbl2));
            }
        }catch (Exception e){
            logger.error("domain保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("domain保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(DomainTbl domainTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            DomainTbl domainTbl2=this.domainTblService.detail(domainTbl.getDomainId());
            if (domainTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(domainTbl2));
            }
        }catch (Exception e){
            logger.error("domain查询详情出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("domain查询详情出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(DomainTbl domainTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.domainTblService.del(domainTbl.getDomainId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

}
