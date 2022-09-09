package com.avp.config.controller;

import com.avp.config.entity.DevTpTbl;
import com.avp.config.entity.FormulaTpTbl;
import com.avp.config.entity.SelectEntity;
import com.avp.config.entity.alarm.AlarmActTbl;
import com.avp.config.entity.alarm.AlarmLevelTbl;
import com.avp.config.entity.alarm.AlarmTypeTbl;
import com.avp.config.entity.base.*;
import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.system.ProSystemTbl;
import com.avp.config.entity.system.StationTbl;
import com.avp.config.entity.user.*;
import com.avp.config.entity.vo.DevCopyVo;
import com.avp.config.service.DevTblService;
import com.avp.config.service.DevTpTblService;
import com.avp.config.service.FormulaTpTblService;
import com.avp.config.service.alarm.AlarmActTblService;
import com.avp.config.service.alarm.AlarmLevelTblService;
import com.avp.config.service.alarm.AlarmTypeTblService;
import com.avp.config.service.base.*;
import com.avp.config.service.system.DomainTblService;
import com.avp.config.service.system.ProSystemTblService;
import com.avp.config.service.system.StationTblService;
import com.avp.config.service.user.*;
import com.avp.config.util.AjaxPara;
import com.avp.config.util.ResponseCode;
import com.avp.config.util.ResultData;
import com.avp.config.util.UniversalStatus;
import com.google.gson.Gson;
import io.swagger.annotations.Api;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

/**
 * 设备模块
 */
@Api
@Controller
@RequestMapping("/dev")
public class DevController {
    private Logger logger = LoggerFactory.getLogger(DevController.class);

    @Autowired
    private ProSystemTblService proSystemTblService;

    @Autowired
    private DevTypeTblService devTypeTblService;

    @Autowired
    private DevTpTblService devTpTblService;

    @Autowired
    private DiTextTblService diTextTblService;

    @Autowired
    private AlarmActTblService alarmActTblService;

    @Autowired
    private FormulaTpTblService formulaTpTblService;

    @Autowired
    private DoGrpTblService doGrpTblService;

    @Autowired
    private StationTblService stationTblService;

    @Autowired
    private DomainTblService domainTblService;

    @Autowired
    private DevTblService devTblService;

    @Autowired
    private RegionTblService regionTblService;

    @Autowired
    private UnitTblService unitTblService;

    @Autowired
    private AlarmLevelTblService alarmLevelTblService;

    @Autowired
    private AlarmTypeTblService alarmTypeTblService;

    @Autowired
    private PermTypeDefService permTypeDefService;

    @Autowired
    private PermDepartmentDefService permDepartmentDefService;

    @Autowired
    private PermUserDefService permUserDefService;

    @Autowired
    private PermGroupDefService permGroupDefService;

    @Autowired
    private PermFuncDefService permFuncDefService;

    /**
     * 得到下拉列表
     *
     * @return
     */
    @RequestMapping(value = "/getSelectList", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara getSelectList(HttpServletRequest request) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        String proSystemId = request.getParameter("proSystemId");
        String selectType = request.getParameter("selectType");
        List<ProSystemTbl> proSystemTblList = null;
        List<DevTypeTbl> devTypeTblList = null;
        List<DiTextTbl> diTextTblList = null;
        List<AlarmActTbl> alarmActTblList = null;
        List<FormulaTpTbl> formulaTpTblList = null;
        List<DoGrpTbl> doGrpTblList = null;
        List<StationTbl> stationTblList = null;
        List<DomainTbl> domainTblList = null;
        List<RegionTbl> regionTblList = null;
        List<UnitTbl> unitTblList = null;
        List<AlarmLevelTbl> alarmLevelTblList = null;
        List<AlarmTypeTbl> alarmTypeTblList = null;
        List<PermTypeDef> permTypeDefList = null;
        List<PermUserDef> permUserDefList = null;
        List<PermDepartmentDef> permDepartmentDefList = null;
        List<PermGroupDef> permGroupDefList = null;
        List<PermFuncDef> permFuncDefList=null;
        SelectEntity selectEntity = new SelectEntity();
        try {
            if (selectType.equals(UniversalStatus.SELECT_ZY)) {
                //专业
                proSystemTblList = this.proSystemTblService.getList(new ProSystemTbl());
                if (proSystemTblList != null) {
                    selectEntity.setProSystemTblList(proSystemTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_DEV_TYPE)) {
                //设备类型
                DevTypeTbl devTypeTbl = new DevTypeTbl();
                devTypeTbl.setProSystemId(Integer.valueOf(proSystemId));
                devTypeTblList = this.devTypeTblService.getList(devTypeTbl);
                if (devTypeTblList != null) {
                    selectEntity.setDevTypeTblList(devTypeTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_DI_TEXT)) {
//                   //数字量文本
                DiTextTbl diTextTbl = new DiTextTbl();
                diTextTbl.setProSystemId(Integer.valueOf(proSystemId));
                diTextTblList = this.diTextTblService.getList(diTextTbl);
                if (diTextTblList != null) {
                    selectEntity.setDiTextTblList(diTextTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_ALARM_LEVEL)) {
                //报警等级
                alarmLevelTblList = this.alarmLevelTblService.getList(new AlarmLevelTbl());
                if (alarmLevelTblList != null) {
                    selectEntity.setAlarmLevelTblList(alarmLevelTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_ALARM_TYPE)) {
                //报警类型
                alarmTypeTblList = this.alarmTypeTblService.getList(new AlarmTypeTbl());
                if (alarmTypeTblList != null) {
                    selectEntity.setAlarmTypeTblList(alarmTypeTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_ALARM_ACT)) {
                //报警动作
                alarmActTblList = this.alarmActTblService.getList(new AlarmActTbl());
                if (alarmActTblList != null) {
                    selectEntity.setAlarmActTblList(alarmActTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_FORMULA)) {
                //公式模板标签
                formulaTpTblList = this.formulaTpTblService.getList();
                if (formulaTpTblList != null) {
                    selectEntity.setFormulaTpTblList(formulaTpTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_DO_GRP)) {
                //控制动作组标签
                DoGrpTbl doGrpTbl = new DoGrpTbl();
                doGrpTbl.setProSystemId(Integer.valueOf(proSystemId));
                doGrpTblList = this.doGrpTblService.getList(doGrpTbl);
                if (doGrpTblList != null) {
                    selectEntity.setDoGrpTblList(doGrpTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_STATION)) {
                //车站
                stationTblList = this.stationTblService.getList(new StationTbl());
                if (stationTblList != null) {
                    selectEntity.setStationTblList(stationTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_DOMAIN)) {
                //域名
                domainTblList = this.domainTblService.getList(new DomainTbl());
                if (domainTblList != null) {
                    selectEntity.setDomainTblList(domainTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_REGION)) {
                //责任区
                RegionTbl regionTbl = new RegionTbl();
                if (StringUtils.isNotBlank(proSystemId)) {
                    regionTbl.setProSystemId(Integer.valueOf(proSystemId));
                }
                regionTblList = this.regionTblService.getList(regionTbl);
                if (regionTblList != null) {
                    selectEntity.setRegionTblList(regionTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_UNIT)) {
                UnitTbl unitTbl = new UnitTbl();
                unitTblList = this.unitTblService.getList(unitTbl);
                if (unitTblList != null) {
                    selectEntity.setUnitTblList(unitTblList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_PERM_TYPE)) {
                permTypeDefList = this.permTypeDefService.getList(new PermTypeDef());
                if (permTypeDefList != null) {
                    selectEntity.setPermTypeDefList(permTypeDefList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_DEP)) {
                permDepartmentDefList = this.permDepartmentDefService.getList(new PermDepartmentDef());
                if (permDepartmentDefList != null) {
                    selectEntity.setPermDepartmentDefList(permDepartmentDefList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_USER)) {
                permUserDefList = this.permUserDefService.getList(new PermUserDef());
                if (permUserDefList != null) {
                    selectEntity.setPermUserDefList(permUserDefList);
                }
            } else if (selectType.equals(UniversalStatus.SELECT_GROUP)) {
                permGroupDefList = this.permGroupDefService.getList(new PermGroupDef());
                if (permGroupDefList != null) {
                    selectEntity.setPermGroupDefList(permGroupDefList);
                }
            }else if(selectType.equals(UniversalStatus.SELECT_COMMON_FUNC)){
                //普通权限
                PermFuncDef permFuncDef=new PermFuncDef();
                permFuncDef.setPermTypeName("普通权限");
                permFuncDefList=this.permFuncDefService.getList(permFuncDef);
                if(permFuncDefList!=null){
                    selectEntity.setPermFuncDefList(permFuncDefList);
                }
            }else if(selectType.equals(UniversalStatus.SELECT_SPEC_FUNC)){
                //特殊权限
                PermFuncDef permFuncDef=new PermFuncDef();
                permFuncDef.setPermTypeName("特殊权限");
                permFuncDefList=this.permFuncDefService.getList(permFuncDef);
                if(permFuncDefList!=null){
                    selectEntity.setPermFuncDefList(permFuncDefList);
                }
            }
        } catch (Exception e) {
            logger.error("查询出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(ResponseCode.RETRIEVE_FAILED.phrase());
        }
        //对象转json
        JSONObject jsonObject = JSONObject.fromObject(selectEntity);
        ajaxPara.setObj(jsonObject.toString());
        return ajaxPara;
    }


    @RequestMapping(value = "/devList", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara devList(HttpServletRequest request, Model model) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        String proSystemId = request.getParameter("proSystemId");
        String deviceName = request.getParameter("deviceName");
        try {
            DevTpTbl devTpTbl = new DevTpTbl();
            if (StringUtils.isNotBlank(proSystemId)) {
                devTpTbl.setProSystemId(Integer.valueOf(proSystemId));
            }
            if (StringUtils.isNotBlank(deviceName)) {
                devTpTbl.setDevTpName(deviceName);
            }
            List<DevTpTbl> devTpTblList = this.devTpTblService.getList(devTpTbl);
            if (devTpTblList != null && devTpTblList.size() > 0) {
                JSONArray jsonArray = JSONArray.fromObject(devTpTblList);
                ajaxPara.setObj(jsonArray.toString());
            }
        } catch (Exception e) {
            logger.error("devList查询列表出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg("查询设备列表出错");
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/saveDevTemplate")
    @ResponseBody
    public AjaxPara saveDevTemplate(HttpServletRequest request, DevTpTbl devTpTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.CREATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            //String proSystemLabel=request.getParameter("proSystemLabel");
            String devTpLabel = devTpTbl.getDevTpLabel();
            if (StringUtils.isBlank(devTpLabel)) {
                ajaxPara.setMsg("设备标签不能为空!");
                ajaxPara.setRetFlag(false);
                return ajaxPara;
            }
            //devTpTbl.setDevTpLabel(proSystemLabel+"."+devTpLabel);
            ajaxPara.setObj(new Gson().toJson(this.devTpTblService.save(devTpTbl)));
        } catch (Exception e) {
            logger.error("保存出错" + e.getMessage());
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/updateDevTemplate")
    @ResponseBody
    public AjaxPara updateDevTemplate(HttpServletRequest request, DevTpTbl devTpTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.UPDATE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            String devTpLabel = devTpTbl.getDevTpLabel();
            if (StringUtils.isBlank(devTpLabel)) {
                ajaxPara.setMsg("设备标签不能为空!");
                ajaxPara.setRetFlag(false);
                return ajaxPara;
            }
            DevTpTbl devTpTbl1 = this.devTpTblService.update(devTpTbl);
            ajaxPara.setObj(new Gson().toJson(devTpTbl1));
        } catch (Exception e) {
            logger.error("修改出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/batchInstance", method = RequestMethod.POST)
    @ResponseBody
    public ResultData<Object> batchInstance(DevCopyVo vo) {
        ResultData<Object> res = new ResultData<>();
        ;
        try {
            res.setObj(devTblService.copyDevInstanceList(vo));
        } catch (Exception e) {
            logger.info("批量生成设备实例失败：" + e.getMessage());
            res.setRetFlag(false);
            res.setMsg(e.getMessage());
        }
        return res;
    }

    @RequestMapping(value = "/copy")
    @ResponseBody
    public AjaxPara copy(DevCopyVo vo) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            DevTpTbl devTpTbl2 = this.devTpTblService.copy(vo);
            ajaxPara.setObj(new Gson().toJson(devTpTbl2));
        } catch (Exception e) {
            logger.error("复制出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/getDetail")
    @ResponseBody
    public AjaxPara getDetail(HttpServletRequest request, DevTpTbl devTpTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.RETRIEVE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            String devTpLabel = devTpTbl.getDevTpLabel();
            if (StringUtils.isBlank(devTpLabel)) {
                ajaxPara.setMsg("设备标签不能为空!");
                ajaxPara.setRetFlag(false);
                return ajaxPara;
            }
            DevTpTbl devTpTbl2 = this.devTpTblService.getDetail(devTpLabel);
            ajaxPara.setObj(new Gson().toJson(devTpTbl2));
        } catch (Exception e) {
            logger.error("修改出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }

    @RequestMapping(value = "/delDevTemplate", method = RequestMethod.POST)
    @ResponseBody
    public AjaxPara delDevTemplate(HttpServletRequest request, DevTpTbl devTpTbl) {
        AjaxPara ajaxPara = new AjaxPara();
        ajaxPara.setMsg(ResponseCode.DELETE_SUCCEED.phrase());
        ajaxPara.setRetFlag(true);
        try {
            String devTpLabel = devTpTbl.getDevTpLabel();
            if (StringUtils.isBlank(devTpLabel)) {
                ajaxPara.setMsg("设备标签不能为空!");
                ajaxPara.setRetFlag(false);
                return ajaxPara;
            }
            this.devTpTblService.delete(devTpTbl);
        } catch (Exception e) {
            logger.error("删除出错");
            ajaxPara.setRetFlag(false);
            ajaxPara.setMsg(e.getMessage());
        }
        return ajaxPara;
    }


}
