package com.avp.config.service.base;

import com.avp.config.dao.base.UnitTblRepository;
import com.avp.config.entity.base.DevTypeTbl;
import com.avp.config.entity.base.UnitTbl;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.persistence.criteria.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zhoujs on 2018/5/17.
 */
@Service
@Transactional(rollbackFor=Exception.class)
public class UnitTblServiceImpl implements UnitTblService {

    private Logger logger = LoggerFactory.getLogger(UnitTblServiceImpl.class);

    @Autowired
    private UnitTblRepository unitTblRepository;

    @Override
    public List<UnitTbl> getList(UnitTbl unitTbl) {
        List<UnitTbl> unitTblList = this.unitTblRepository.findAll(new Specification<UnitTbl>() {
            @Override
            public Predicate toPredicate(Root<UnitTbl> root, CriteriaQuery<?> criteriaQuery, CriteriaBuilder criteriaBuilder) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(unitTbl.getUnitName())) {
                    list.add(criteriaBuilder.like(root.get("unitName").as(String.class), "%" + unitTbl.getUnitName() + "%"));
                }

                if (unitTbl.getUnitId()!=null) {
                    list.add(criteriaBuilder.equal(root.get("unitId").as(Integer.class), unitTbl.getUnitId()));
                }

                Predicate[] p = new Predicate[list.size()];
                criteriaQuery.where( criteriaBuilder.and(list.toArray(p)));
                //添加排序的功能
                //query.orderBy(cb.desc(root.get("diValue").as(Integer.class)));
                List<Order> orders=new ArrayList<>();
                orders.add(criteriaBuilder.asc(root.get("unitId").as(Integer.class)));
                criteriaQuery.orderBy(orders);
                return criteriaQuery.getRestriction();
            }
        });
        return unitTblList;
    }

    @Override
    public UnitTbl saveOrUpdate(UnitTbl unitTbl, String operate)throws  Exception {
        UnitTbl unitTbl2=new UnitTbl();
        List<UnitTbl> list = new ArrayList<>();
        if("update".equals(operate)){
            //查询详情
            unitTbl2=this.unitTblRepository.findByUnitId(unitTbl.getUnitId());
            if(!unitTbl.getUnitName().equals(unitTbl2.getUnitName())){
                list=this.unitTblRepository.findByUnitName(unitTbl.getUnitName());
            }
        }else{
            list=this.unitTblRepository.findByUnitIdOrUnitName(unitTbl.getUnitId(),unitTbl.getUnitName());
        }
        if(list!=null && list.size()>0){
            throw new Exception("ID或名称重复");
        }
        return this.unitTblRepository.save(unitTbl);
    }

    @Override
    public UnitTbl detail(Integer unitId) {
        return this.unitTblRepository.findByUnitId(unitId);
    }

    @Override
    public void del(Integer unitId) {
        this.unitTblRepository.deleteByUnitId(unitId);
    }
}
