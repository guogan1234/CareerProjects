package com.avp.configService.entity.vo;

import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;

import java.util.List;

/**
 * tcp
 */
public class ZyEntity {
    public List<String> headerList;

    public List<List<CellVo>> lists;

    public boolean hasNext;

    public List<String> getHeaderList() {
        return headerList;
    }

    public List<List<CellVo>> getLists() {
        return lists;
    }

    public void setHeaderList(List<String> headerList) {
        this.headerList = headerList;
    }

    public void setLists(List<List<CellVo>> lists) {
        this.lists = lists;
    }

    public boolean isHasNext() {
        return hasNext;
    }

    public void setHasNext(boolean hasNext) {
        this.hasNext = hasNext;
    }
}
