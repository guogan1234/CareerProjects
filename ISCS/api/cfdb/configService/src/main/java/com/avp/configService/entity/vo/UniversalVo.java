package com.avp.configService.entity.vo;

import com.avp.configService.util.Page;

import java.util.ArrayList;
import java.util.List;

/**
 * 通用接口 http
 */
public class UniversalVo {

    private  List<String> columnList=new ArrayList<>();

    private  Integer columnSize=0;

    private List<List<String>> dataList=new ArrayList<>();

    private  Integer dataCurrentSize=0;

    private Page page;

    public List<String> getColumnList() {
        return columnList;
    }

    public void setColumnList(List<String> columnList) {
        this.columnList = columnList;
    }

    public Integer getColumnSize() {
        return columnSize;
    }

    public void setColumnSize(Integer columnSize) {
        this.columnSize = columnSize;
    }

    public List<List<String>> getDataList() {
        return dataList;
    }

    public void setDataList(List<List<String>> dataList) {
        this.dataList = dataList;
    }

    public Integer getDataCurrentSize() {
        return dataCurrentSize;
    }

    public void setDataCurrentSize(Integer dataCurrentSize) {
        this.dataCurrentSize = dataCurrentSize;
    }

    public Page getPage() {
        return page;
    }

    public void setPage(Page page) {
        this.page = page;
    }

    public static  void  main(String args[]){
        String sql="select * from conf.dev_tbl";
        StringBuilder builder=new StringBuilder(sql);
        int index=builder.indexOf("from");
        char tableName=builder.charAt(index+1);
        System.out.println(tableName);
    }
}
