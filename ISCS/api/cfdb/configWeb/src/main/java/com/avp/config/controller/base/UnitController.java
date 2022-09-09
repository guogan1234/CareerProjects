package com.avp.config.controller.base;


import com.avp.config.entity.base.RegionTbl;
import com.avp.config.entity.base.UnitTbl;
import com.avp.config.service.base.UnitTblService;
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
 * Created by zhoujs on 2018/5/17.
 */
@Controller
@RequestMapping("/unit")
public class UnitController {

    private Logger logger = LoggerFactory.getLogger(UnitController.class);

    @Autowired
    private UnitTblService unitTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(UnitTbl unitTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<UnitTbl> list=this.unitTblService.getList(unitTbl);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询unit 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(UnitTbl unitTbl, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
           UnitTbl unitTbl1=this.unitTblService.saveOrUpdate(unitTbl,operate);
           ajaxPara.setObj(new Gson().toJson(unitTbl1));
        } catch (Exception e) {
            logger.error("unit保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("unit保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(UnitTbl unitTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            UnitTbl unitTbl1=this.unitTblService.detail(unitTbl.getUnitId());
            ajaxPara.setObj(new Gson().toJson(unitTbl1));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询unit 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(UnitTbl unitTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.unitTblService.del(unitTbl.getUnitId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

}
