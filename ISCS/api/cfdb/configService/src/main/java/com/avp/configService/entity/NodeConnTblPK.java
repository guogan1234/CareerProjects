package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Id;
import java.io.Serializable;
import java.util.Objects;

public class NodeConnTblPK implements Serializable {
    private String tagName;
    private int nodeNo;

    @Column(name = "tag_name", nullable = false, length = 32)
    @Id
    public String getTagName() {
        return tagName;
    }

    public void setTagName(String tagName) {
        this.tagName = tagName;
    }

    @Column(name = "node_no", nullable = false)
    @Id
    public int getNodeNo() {
        return nodeNo;
    }

    public void setNodeNo(int nodeNo) {
        this.nodeNo = nodeNo;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        NodeConnTblPK that = (NodeConnTblPK) o;
        return nodeNo == that.nodeNo &&
                Objects.equals(tagName, that.tagName);
    }

    @Override
    public int hashCode() {

        return Objects.hash(tagName, nodeNo);
    }
}
