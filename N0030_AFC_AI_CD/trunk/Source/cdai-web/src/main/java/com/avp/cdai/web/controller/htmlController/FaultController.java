package com.avp.cdai.web.controller.htmlController;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.security.Principal;

/**
 * Created by len on 2017/9/19.
 */
@Controller
public class FaultController {

    private static final Logger LOGGER = LoggerFactory.getLogger(FaultController.class);

    public static final String TITLE_TYPICALFAULT = "典型故障分析";
    public static final String TITLE_MODULEFAULT = "模块故障分析";
    public static final String TITLE_DEVICEFAULT = "设备故障分析";
    public static final String TITLE_DEVICERATE = "设备宕机分析";
    public static final String TITLE_FAULTFACTOR = "故障因子分析";
    public static final String TITLE_FAULTREPAIRTIME = "故障修复时间分析";
    public static final String TITLE_STATIONMAP = "车站地图报警";
    public static final String TITLE_TYPICAL = "故障分布";
    public static final String TITLE_MODULE = "故障分布";
    public static final String TITLE_DEVICE = "设备模块分布";

    @RequestMapping(value = {"/fault/typicalFault"})
    public ModelAndView typicalFault(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/typicalFault");
        try {
            model.addObject("cdai_title", TITLE_TYPICALFAULT);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }
    @RequestMapping(value = {"/fault/moduleFault"})
    public ModelAndView moduleFault(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/moduleFault");
        try {
            model.addObject("cdai_title", TITLE_MODULEFAULT);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/fault/deviceFault"})
    public ModelAndView deviceFault(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/deviceFault");
        try {
            model.addObject("cdai_title", TITLE_DEVICEFAULT);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/fault/deviceRate"})
    public ModelAndView deviceRate(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/deviceRate");
        try {
            model.addObject("cdai_title", TITLE_DEVICERATE);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/fault/faultFactor"})
    public ModelAndView faultFactor(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/faultFactor");
        try {
            model.addObject("cdai_title", TITLE_FAULTFACTOR);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/fault/faultRepairTime"})
    public ModelAndView faultRepairTime(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/faultRepairTime");
        try {
            model.addObject("cdai_title", TITLE_FAULTREPAIRTIME);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/fault/stationMap"})
    public ModelAndView stationMap(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/stationMap");
        try {
            model.addObject("cdai_title", TITLE_STATIONMAP);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }
    @RequestMapping(value = {"/fault/typical"})
    public ModelAndView typical(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/typical");
        try {
            model.addObject("cdai_title", TITLE_TYPICAL);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/fault/module"})
    public ModelAndView module(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/module");
        try {
            model.addObject("cdai_title", TITLE_MODULE);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/fault/device"})
    public ModelAndView device(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/fault/device");
        try {
            model.addObject("cdai_title", TITLE_DEVICE);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

}
