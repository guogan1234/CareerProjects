package com.avp.cdai.web.entity;

import javax.persistence.*;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/8/16.
 */
@Entity
@Table(name = "ticket_type",schema = "afccd")
public class TicketType {
    @Id
    private Integer id;
    @Column(name = "ticket_id")
    private Integer ticketId;
    @Column(name = "ticket_cn_name")
    private String ticketName;
    @Column(name = "insert_time")
    private Date insertTime;
    @Column(name = "family_id")
    private Integer familyId;
    @Transient
    private List<List<Long>> dataList1;
    @Transient
    private List<List<Long>> dataList2;
    @Transient
    private List<List<Long>> dataList3;
    public List<List<Long>> getDataList1() {
        return dataList1;
    }

    public void setDataList1(List<List<Long>> dataList1) {
        this.dataList1 = dataList1;
    }

    public List<List<Long>> getDataList2() {
        return dataList2;
    }

    public void setDataList2(List<List<Long>> dataList2) {
        this.dataList2 = dataList2;
    }

    public List<List<Long>> getDataList3() {
        return dataList3;
    }

    public void setDataList3(List<List<Long>> dataList3) {
        this.dataList3 = dataList3;
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getTicketId() {
        return ticketId;
    }

    public void setTicketId(Integer ticketId) {
        this.ticketId = ticketId;
    }

    public String getTicketName() {
        return ticketName;
    }

    public void setTicketName(String ticketName) {
        this.ticketName = ticketName;
    }

    public Date getInsertTime() {
        return insertTime;
    }

    public void setInsertTime(Date insertTime) {
        this.insertTime = insertTime;
    }

    public Integer getFamilyId() {
        return familyId;
    }

    public void setFamilyId(Integer familyId) {
        this.familyId = familyId;
    }
}
