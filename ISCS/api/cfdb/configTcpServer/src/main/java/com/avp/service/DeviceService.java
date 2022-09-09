package com.avp.service;

import com.avp.entity.Device;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by guo on 2018/4/16.
 */
public class DeviceService {
    private static int page_size = 1000;

    public static List<Device> getDevice(){
        List<Device> list = new ArrayList<Device>();

        for(int i = 0;i<page_size;i++){
            String str = "_" + i;
            Device d = new Device();
            d.setCtrl_trans_grp_label("setCtrl_trans_grp_label" + str);
            d.setDev_label("setDev_label" + str);
            d.setDev_name("setDev_name" + str);
            d.setDev_tp_label("setDev_tp_label" + str);
            d.setDev_type_label("setDev_type_label" + str);
            d.setDev_status(i);
            d.setDomain_id(i);
            d.setIs_summary(i);
            d.setPro_system_id(i);
            d.setRegion_id(i);
            d.setRtu_id(i);
            d.setStation_id(i);
            d.setSummary_status(i);
            d.setSummary_value(i);

            list.add(d);
        }
        return list;
    }
}
