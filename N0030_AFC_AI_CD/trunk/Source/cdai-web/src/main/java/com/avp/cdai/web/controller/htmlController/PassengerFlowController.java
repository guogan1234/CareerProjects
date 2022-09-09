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
 * Created by len on 2017/9/18.
 */
@Controller
public class PassengerFlowController {
    private static final Logger LOGGER = LoggerFactory.getLogger(PassengerFlowController.class);

    public static final String TITLE_STATIONCONTRAST = "车站分时客流对比";
    public static final String TITLE_STATIONFORECAST = "车站分时客流预测";
    public static final String TITLE_STATIONTOTALCONTRAST = "车站累计客流对比";
    public static final String TITLE_STATIONTOTALFORECAST = "车站累计客流预测";
    public static final String TITLE_LINECONTRAST = "线路分时客流对比";
    public static final String TITLE_LINEFORECAST = "线路分时客流预测";
    public static final String TITLE_LINETOTALCONTRAST = "线路累计客流对比";
    public static final String TITLE_LINETOTALFORECAST = "线路累计客流预测";
    public static final String TITLE_TIKETCONTRAST = "票卡分时对比";
    public static final String TITLE_TICKETTOTAL = "票卡累计对比";


    @RequestMapping(value = {"/passengerFlow/stationContrast"})
    public ModelAndView stationContrast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/stationContrast");
        try {
            model.addObject("cdai_title", TITLE_STATIONCONTRAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/passengerFlow/stationForecast"})
    public ModelAndView stationForecast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/stationForecast");
        try {
            model.addObject("cdai_title", TITLE_STATIONFORECAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/passengerFlow/stationTotalContrast"})
    public ModelAndView stationTotalContrast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/stationTotalContrast");
        try {
            model.addObject("cdai_title", TITLE_STATIONTOTALCONTRAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/passengerFlow/stationTotalForecast"})
    public ModelAndView stationTotalForecast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/stationTotalForecast");
        try {

            model.addObject("cdai_title", TITLE_STATIONTOTALFORECAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/passengerFlow/lineContrast"})
    public ModelAndView lineContrast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/lineContrast");
        try {

            model.addObject("cdai_title", TITLE_LINECONTRAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/passengerFlow/lineForecast"})
    public ModelAndView lineForecast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/lineForecast");
        try {

            model.addObject("cdai_title", TITLE_LINEFORECAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/passengerFlow/lineTotalContrast"})
    public ModelAndView lineTotalContrast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/lineTotalContrast");
        try {

            model.addObject("cdai_title", TITLE_LINETOTALCONTRAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/passengerFlow/lineTotalForecast"})
    public ModelAndView lineTotalForecast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/lineTotalForecast");
        try {

            model.addObject("cdai_title", TITLE_LINETOTALFORECAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/passengerFlow/ticketContrast"})
    public ModelAndView ticketContrast(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/ticketContrast");
        try {

            model.addObject("cdai_title", TITLE_TIKETCONTRAST);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

    @RequestMapping(value = {"/passengerFlow/ticketTotal"})
    public ModelAndView ticketTotal(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/passengerFlow/ticketTotal");
        try {

            model.addObject("cdai_title", TITLE_TICKETTOTAL);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

}
