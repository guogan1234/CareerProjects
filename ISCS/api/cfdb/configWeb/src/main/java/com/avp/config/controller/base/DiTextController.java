package com.avp.config.controller.base;

import com.avp.config.entity.base.DevTypeTbl;
import com.avp.config.entity.base.DiTextTbl;
import com.avp.config.entity.system.DomainTbl;
import com.avp.config.service.base.DiTextTblService;
import com.avp.config.service.system.DomainTblService;
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
@RequestMapping("/diText")
public class DiTextController {
    private Logger logger= LoggerFactory.getLogger(DiTextController.class);

    @Autowired
    private DiTextTblService diTextTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(DiTextTbl diTextTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<DiTextTbl> list=this.diTextTblService.getList(diTextTbl);
            if (list != null && list.size() > 0) {
                ajaxPara.setObj(new Gson().toJson(list));
            }
        }catch (Exception e){
            logger.error("DiText查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("DiText查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(DiTextTbl diTextTbl, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            DiTextTbl DiTextTbl2=this.diTextTblService.saveOrUpdate(diTextTbl,operate);
            if (DiTextTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(DiTextTbl2));
            }
        }catch (Exception e){
            logger.error("diText保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("diText保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(DiTextTbl diTextTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            DiTextTbl diTextTbl2=this.diTextTblService.detail(diTextTbl.getDiValue(),diTextTbl.getDiTextLabel());
            if (diTextTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(diTextTbl2));
            }
        }catch (Exception e){
            logger.error("diText查询详情出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("diText查询详情出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(DiTextTbl diTextTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.diTextTblService.del(diTextTbl.getDiValue(),diTextTbl.getDiTextLabel());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }














}
