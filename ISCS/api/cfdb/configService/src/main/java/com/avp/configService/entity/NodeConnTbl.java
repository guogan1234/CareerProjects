package com.avp.configService.entity;

import javax.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "node_conn_tbl", schema = "conf")
@IdClass(NodeConnTblPK.class)
public class NodeConnTbl {
    private String tagName;
    private Integer nodeNo;
    private String targetTagname;
    private Integer targetNodeno;
    private Integer para1;
    private Integer para2;

    @Id
    @Column(name = "tag_name", nullable = false, length = 32)
    public String getTagName() {
        return tagName;
    }

    public void setTagName(String tagName) {
        this.tagName = tagName;
    }

    @Id
    @Column(name = "node_no", nullable = false)
    public int getNodeNo() {
        return nodeNo;
    }

    public void setNodeNo(int nodeNo) {
        this.nodeNo = nodeNo;
    }

    @Basic
    @Column(name = "target_tagname", nullable = false, length = 32)
    public String getTargetTagname() {
        return targetTagname;
    }

    public void setTargetTagname(String targetTagname) {
        this.targetTagname = targetTagname;
    }

    @Basic
    @Column(name = "target_nodeno", nullable = false)
    public int getTargetNodeno() {
        return targetNodeno;
    }

    public void setTargetNodeno(int targetNodeno) {
        this.targetNodeno = targetNodeno;
    }

    @Basic
    @Column(name = "para1", nullable = true)
    public Integer getPara1() {
        return para1;
    }

    public void setPara1(Integer para1) {
        this.para1 = para1;
    }

    @Basic
    @Column(name = "para2", nullable = true)
    public Integer getPara2() {
        return para2;
    }

    public void setPara2(Integer para2) {
        this.para2 = para2;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        NodeConnTbl that = (NodeConnTbl) o;
        return nodeNo == that.nodeNo &&
                targetNodeno == that.targetNodeno &&
                Objects.equals(tagName, that.tagName) &&
                Objects.equals(targetTagname, that.targetTagname) &&
                Objects.equals(para1, that.para1) &&
                Objects.equals(para2, that.para2);
    }

    @Override
    public int hashCode() {

        return Objects.hash(tagName, nodeNo, targetTagname, targetNodeno, para1, para2);
    }
}
