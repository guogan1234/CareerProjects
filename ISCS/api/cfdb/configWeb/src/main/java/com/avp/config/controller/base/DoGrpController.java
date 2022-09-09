package com.avp.config.controller.base;

import com.avp.config.entity.base.DiTextTbl;
import com.avp.config.entity.base.DoGrpTbl;
import com.avp.config.service.base.DoGrpTblService;
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
@RequestMapping("/doGrp")
public class DoGrpController {
    private Logger logger= LoggerFactory.getLogger(DoGrpController.class);

    @Autowired
    private DoGrpTblService doGrpTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara list(DoGrpTbl doGrpTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            List<DoGrpTbl> list=this.doGrpTblService.getList(doGrpTbl);
            if (list != null && list.size() > 0) {
                ajaxPara.setObj(new Gson().toJson(list));
            }
        }catch (Exception e){
            logger.error("doGrp查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("doGrp查询列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping("/saveDoGrpConfig")
    @ResponseBody
    public AjaxPara saveDoGrpConfig(DoGrpTbl doGrpTbl, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        String allows=request.getParameter("allows");
        try {
            DoGrpTbl doGrpTbl2=this.doGrpTblService.saveDoGrpConfig(doGrpTbl,allows);
            if (doGrpTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(doGrpTbl2));
            }
        }catch (Exception e){
            logger.error("保存出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("保存出错");
            }
        }
        return ajaxPara;
    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(DoGrpTbl doGrpTbl, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
            DoGrpTbl doGrpTbl2=this.doGrpTblService.saveOrUpdate(doGrpTbl,operate);
            if (doGrpTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(doGrpTbl2));
            }
        }catch (Exception e){
            logger.error("doGrp保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("doGrp保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(DoGrpTbl doGrpTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            DoGrpTbl doGrpTbl2=this.doGrpTblService.detail(doGrpTbl.getDoOrder(),doGrpTbl.getDoGrpLabel());
            if (doGrpTbl2 != null) {
                ajaxPara.setObj(new Gson().toJson(doGrpTbl2));
            }
        }catch (Exception e){
            logger.error("doGrp查询详情出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("doGrp查询详情出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(DoGrpTbl doGrpTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.doGrpTblService.del(doGrpTbl.getDoOrder(),doGrpTbl.getDoGrpLabel());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }
}
