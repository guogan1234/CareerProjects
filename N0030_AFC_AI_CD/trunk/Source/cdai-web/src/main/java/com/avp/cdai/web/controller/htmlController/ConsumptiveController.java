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
public class ConsumptiveController {

    private static final Logger LOGGER = LoggerFactory.getLogger(ConsumptiveController.class);

    public static final String TITLE_CONSUMPTIONFORECAST = "耗材预测";
    public static final String TITLE_CONSUMPTIONANALYSIS = "耗材分析";
    public static final String TITLE_CONSUMABLESTREND = "耗材去向分析";
    public static final String TITLE_DEVICEKEEP = "保养计划生成";


    @RequestMapping(value = {"/consumptive/consumptionAnalysis"})
    public ModelAndView consumptionAnalysis(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/consumptive/consumptionAnalysis");
        try {
            model.addObject("cdai_title", TITLE_CONSUMPTIONANALYSIS);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/consumptive/consumptionForecast"})
    public ModelAndView consumptionForecast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/consumptive/consumptionForecast");
        try {
            model.addObject("cdai_title", TITLE_CONSUMPTIONFORECAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/consumptive/consumablesTrend"})
    public ModelAndView consumablesTrend(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/consumptive/consumablesTrend");
        try {
            model.addObject("cdai_title", TITLE_CONSUMABLESTREND);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/consumptive/deviceKeep"})
    public ModelAndView deviceKeep(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/consumptive/deviceKeep");
        try {
            model.addObject("cdai_title", TITLE_DEVICEKEEP);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }
}
