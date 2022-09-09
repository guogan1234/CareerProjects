package com.avp.config.controller.base;


import com.avp.config.entity.base.DoGrpTbl;
import com.avp.config.entity.base.RegionTbl;
import com.avp.config.service.base.RegionTblService;
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
@RequestMapping("/region")
public class RegionController {

    private Logger logger = LoggerFactory.getLogger(RegionController.class);

    @Autowired
    private RegionTblService regionTblService;

    @RequestMapping("/list")
    @ResponseBody
    public AjaxPara getList(RegionTbl regionTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            List<RegionTbl> list=this.regionTblService.getList(regionTbl);
            ajaxPara.setObj(new Gson().toJson(list));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询region 报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;

    }


    @RequestMapping("/saveOrUpdate")
    @ResponseBody
    public AjaxPara saveOrUpdate(RegionTbl regionTbl, HttpServletRequest request){
        String operate=request.getParameter("operate");
        AjaxPara ajaxPara=new AjaxPara();
        if(StringUtils.isNotBlank(operate) && operate.equals("update")){
            ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        }else{
            ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        }
        ajaxPara.setRetFlag(true);
        try {
           RegionTbl regionTbl1=this.regionTblService.saveOrUpdate(regionTbl,operate);

            ajaxPara.setObj(new Gson().toJson(regionTbl1));
        } catch (Exception e) {
            logger.error("region保存或修改出错");
            ajaxPara.setRetFlag(false);
            if(e.getMessage()!=null){
                ajaxPara.setMsg(e.getMessage());
            }else{
                ajaxPara.setMsg("region保存或修改出错");
            }
        }
        return ajaxPara;
    }

    @RequestMapping("/detail")
    @ResponseBody
    public AjaxPara detail(RegionTbl regionTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);

        try {
            RegionTbl regionTbl1=this.regionTblService.detail(regionTbl.getRegionId());
            ajaxPara.setObj(new Gson().toJson(regionTbl1));
        } catch (Exception e) {
            e.printStackTrace();
            logger.error("查询region 详情报错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/del",method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara del(RegionTbl regionTbl){
        AjaxPara ajaxPara=new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try{
            this.regionTblService.del(regionTbl.getRegionId());
        }catch (Exception e){
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

}
