package com.avp.configService.communicate;

import com.avp.configService.entity.User;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by guo on 2018/4/20.
 */
@Deprecated
public class GetDataController {
    private List data = null;
    private boolean listEnd;

    private int domainId;
    private String tableName;
    private int page;
    private int pageSize;

    public boolean isListEnd() {
        return listEnd;
    }

    public void setListEnd(boolean listEnd) {
        this.listEnd = listEnd;
    }

    public int getDomainId() {
        return domainId;
    }

    public void setDomainId(int domainId) {
        this.domainId = domainId;
    }

    public String getTableName() {
        return tableName;
    }

    public void setTableName(String tableName) {
        this.tableName = tableName;
    }

    public int getPage() {
        return page;
    }

    public void setPage(int page) {
        this.page = page;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public List getData() {
        return data;
    }

    public void setData(List data) {
        this.data = data;
    }

    public GetDataController() {
        listEnd = false;
        initTableDatas();
    }

    private void initTableDatas(){
        data = new ArrayList();
        for(int i = 0;i<2500;i++){
            User user = new User();
            Integer integer = new Integer(i);
            user.setId(integer.longValue());
            String name = "name_" + i;
            user.setName(name);
            user.setAge(integer);

            data.add(user);
        }
    }

    public List getTableDatas(){
        int start = page*pageSize;
        int end = (page+1)*pageSize;
        if(start >= data.size()){
            listEnd = true;
            return null;
        }
        else if(start < data.size() && end >= data.size()){
            listEnd = true;
            return data.subList(start,data.size());
        }else {
            return data.subList(start, end);
        }
    }
}
