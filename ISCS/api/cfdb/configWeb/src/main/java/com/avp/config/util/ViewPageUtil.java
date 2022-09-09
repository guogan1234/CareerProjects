package com.avp.config.util;
import org.springframework.util.StringUtils;
import javax.servlet.http.HttpServletRequest;
import java.util.ArrayList;
import java.util.List;

/**
 * 分页工具类
 */
public class ViewPageUtil<T> {
    private int pageNo = 1;

    private int pageSize = 10;

    private int totalElements = 0;

    private int totalPages = 0;

    private List<T> list=new ArrayList<T>();

    public int getPageNo() {
        return pageNo;
    }

    public void setPageNo(int pageNo) {
        this.pageNo = pageNo;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public int getTotalElements() {
        return totalElements;
    }

    public void setTotalElements(int totalElements) {
        this.totalElements = totalElements;
    }

    public int getTotalPages() {
        return totalPages;
    }

    public void setTotalPages(int totalPages) {
        this.totalPages = totalPages;
    }

    public List<T> getList() {
        return list;
    }

    public void setList(List<T> list) {
        this.list = list;
    }

    public ViewPageUtil(HttpServletRequest request){
        String no=request.getParameter("pageNo");
        if(!StringUtils.isEmpty(pageNo)){
            pageNo=Integer.valueOf(no);
        }
        String size=request.getParameter("pageSize");
        if(!StringUtils.isEmpty(size)){
            pageSize=Integer.valueOf(size);
        }
    }
}
