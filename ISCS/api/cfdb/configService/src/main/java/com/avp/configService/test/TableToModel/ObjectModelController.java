package com.avp.configService.test.TableToModel;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by guo on 2018/4/27.
 */
public class ObjectModelController {
    public List GetList(){
        List list = new ArrayList();

        for(int j = 0;j<2;j++) {
            List subList = new ArrayList();
            for (int i = 0; i < 3; i++) {
                if (i == 0) {
                    ObjectModel<Integer> section_1 = new ObjectModel<>();
                    section_1.setType(0);
                    section_1.setValue(18);

                    subList.add(section_1);
                } else if (i == 1) {
                    ObjectModel<String> section_2 = new ObjectModel<>();
                    section_2.setType(1);
                    section_2.setValue("name");

                    subList.add(section_2);
                } else if (i == 2) {
                    ObjectModel<Double> section_3 = new ObjectModel<>();
                    section_3.setType(2);
                    section_3.setValue(12.22);

                    subList.add(section_3);
                }
            }

            list.add(subList);
        }

        System.out.println("Show List..." + list.size());
//        for(int i = 0;i<list.size();i++){
//            Object obj = list.get(i);
//            ObjectModel<Integer> model = (ObjectModel<Integer>)obj;
//            int type = model.getType();
//            String str = null;
//            System.out.println("list:type -- " + type);
//            System.out.println("list:value -- " + model.getValue());
//        }
        int a = list.size();
        System.out.println("list:a -- " + a);
        for(int i = 0;i<a;i++){
            Object objList = list.get(i);
            List subList = (List)objList;
            int b = subList.size();
            System.out.println("list:b -- " + b + " " + i);
            for(int j = 0;j<b;j++){
                Object obj = subList.get(j);
                ObjectModel<Integer> model = (ObjectModel<Integer>)obj;
                int type = model.getType();
                String str = null;
                System.out.println("list:type -- " + type);
                System.out.println("list:value -- " + model.getValue());
            }
        }
        return list;
    }
}
