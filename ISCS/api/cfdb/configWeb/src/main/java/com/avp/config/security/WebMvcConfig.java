package com.avp.config.security;

import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.ViewControllerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;
@Configuration
public class WebMvcConfig extends WebMvcConfigurerAdapter {
    @Override
    public void addViewControllers(ViewControllerRegistry registry) {
        registry.addViewController("/login").setViewName("login");
        registry.addViewController("/").setViewName("config/index");
        registry.addViewController("/dev/devconf").setViewName("config/devconf");
        registry.addViewController("/domain/toList").setViewName("config/system/domainList");
        registry.addViewController("/station/toList").setViewName("config/system/stationList");
        registry.addViewController("/proSystem/toList").setViewName("config/system/proSystemList");
        registry.addViewController("/process/toList").setViewName("config/system/processList");
        registry.addViewController("/node/toList").setViewName("config/system/nodeList");


        registry.addViewController("/diText/toList").setViewName("config/base/diTextList");
        registry.addViewController("/doGrp/toList").setViewName("config/base/doGrpList");
        registry.addViewController("/devType/toList").setViewName("config/base/devTypeList");
        registry.addViewController("/unit/toList").setViewName("config/base/unitList");
        registry.addViewController("/region/toList").setViewName("config/base/regionList");

        registry.addViewController("/alarm/toAlarmActList").setViewName("config/alarm/alarmActList");
        registry.addViewController("/alarm/toAlarmTypeList").setViewName("config/alarm/alarmTypeList");
        registry.addViewController("/alarm/toAlarmLevelList").setViewName("config/alarm/alarmLevelList");
        registry.addViewController("/alarm/toAlarmSourceList").setViewName("config/alarm/alarmSourceList");
        registry.addViewController("/alarm/toAlarmInfoList").setViewName("config/alarm/alarmInfoList");

        registry.addViewController("/user/toList").setViewName("config/user/permUserDefList");
        registry.addViewController("/department/toList").setViewName("config/user/permDepartmentList");
        registry.addViewController("/userGroup/toList").setViewName("config/user/permGroupDefList");
        registry.addViewController("/permFunc/toList").setViewName("config/user/permFuncDefList");
        registry.addViewController("/user/toPermTypeDefList").setViewName("config/user/permTypeDefList");
        registry.addViewController("/permUserDepartment/toList").setViewName("config/user/permUserDepartmentList");
        registry.addViewController("/permSecurity/toList").setViewName("config/user/permSecurityDefList");
        registry.addViewController("/permUserGroup/toList").setViewName("config/user/permUserGroupList");
        registry.addViewController("/permGroupStation/toList").setViewName("config/user/permGroupStationList");
        registry.addViewController("/permGroupRealation/toList").setViewName("config/user/permGroupRealationList");
        registry.addViewController("/permSpecGroupRegionRealation/toList").setViewName("config/user/permSpecGroupRegionRealationList");
        registry.addViewController("/permSpecGroupStationRealation/toList").setViewName("config/user/permSpecGroupStationRealationList");
    }
}
