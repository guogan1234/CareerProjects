package com.avp.configService.util;

public class Page {

    private  int number;

    private int totalPages;

    private int size;

    private Integer hasPrevious;

    private Integer hasNext;
    private Long totalElements;


    public int getNumber() {
        return number;
    }

    public void setNumber(int number) {
        this.number = number;
    }

    public int getTotalPages() {
        return totalPages;
    }

    public void setTotalPages(int totalPages) {
        this.totalPages = totalPages;
    }

    public int getSize() {
        return size;
    }

    public void setSize(int size) {
        this.size = size;
    }

    public Integer getHasPrevious() {
        return hasPrevious;
    }

    public void setHasPrevious(Integer hasPrevious) {
        this.hasPrevious = hasPrevious;
    }

    public Integer getHasNext() {
        return hasNext;
    }

    public void setHasNext(Integer hasNext) {
        this.hasNext = hasNext;
    }

    public Long getTotalElements() {
        return totalElements;
    }

    public void setTotalElements(Long totalElements) {
        this.totalElements = totalElements;
    }
}
