package com.avp.config.entity.base;

import com.avp.config.entity.system.ProSystemTbl;
import com.fasterxml.jackson.annotation.JsonIgnore;
import org.hibernate.annotations.NotFound;
import org.hibernate.annotations.NotFoundAction;

import javax.persistence.*;
import java.io.Serializable;

/**
 * 离散输出组定义表
 * fxy
 */
@Entity
@Table(name = "do_grp_tbl",schema = "conf")
@IdClass(DoGrpMultiKeys.class)
public class DoGrpTbl implements  Serializable{
    //离散输出组标签
   @Id
   @Column(name = "do_grp_label",length = 32)
   private String doGrpLabel;
   //顺序号
   @Id
   @Column(name ="do_order")
   private  Integer doOrder;
   //输出分量数
   @Column(name = "do_number")
   private  Integer doNumber;
   //输出点号索引
   @Column(name = "do_index")
   private  Integer doIndex;
   //目标值
   @Column(name = "do_value")
   private  Integer doValue;
   //输出动作名
   @Column(name = "do_name",length = 32)
   private  String doName;
   //输出类型值及描述：0 -- 自复位开（trip）；1 -- 自复位关（close）；2 -- 上升沿
   //（pulse on）；3 -- 下降沿（pulse off）；4 -- 常开（latch on）；5 -- 常闭
   @Column(name = "do_type")
   private  Integer doType;
   //输出允许
   @Column(name = "do_allow")
   private  Integer doAllow;
   //所属专业系统ID
   private Integer proSystemId;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "proSystemId", insertable = false, updatable = false)
    @JsonIgnore
    @NotFound(action = NotFoundAction.IGNORE)
    private ProSystemTbl proSystemTbl;

    public String getDoGrpLabel() {
        return doGrpLabel;
    }

    public void setDoGrpLabel(String doGrpLabel) {
        this.doGrpLabel = doGrpLabel;
    }

    public Integer getDoOrder() {
        return doOrder;
    }

    public void setDoOrder(Integer doOrder) {
        this.doOrder = doOrder;
    }

    public Integer getDoNumber() {
        return doNumber;
    }

    public void setDoNumber(Integer doNumber) {
        this.doNumber = doNumber;
    }

    public Integer getDoIndex() {
        return doIndex;
    }

    public void setDoIndex(Integer doIndex) {
        this.doIndex = doIndex;
    }

    public Integer getDoValue() {
        return doValue;
    }

    public void setDoValue(Integer doValue) {
        this.doValue = doValue;
    }

    public String getDoName() {
        return doName;
    }

    public void setDoName(String doName) {
        this.doName = doName;
    }

    public Integer getDoType() {
        return doType;
    }

    public void setDoType(Integer doType) {
        this.doType = doType;
    }

    public Integer getDoAllow() {
        return doAllow;
    }

    public void setDoAllow(Integer doAllow) {
        this.doAllow = doAllow;
    }

    public Integer getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(Integer proSystemId) {
        this.proSystemId = proSystemId;
    }

    public ProSystemTbl getProSystemTbl() {
        return proSystemTbl;
    }

    public void setProSystemTbl(ProSystemTbl proSystemTbl) {
        this.proSystemTbl = proSystemTbl;
    }
}
