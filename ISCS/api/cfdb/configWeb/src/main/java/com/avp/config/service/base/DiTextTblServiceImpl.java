package com.avp.config.service.base;

import com.avp.config.dao.base.DiTextTblRepository;
import com.avp.config.dao.system.ProSystemTblRepository;
import com.avp.config.entity.base.DevTypeTbl;
import com.avp.config.entity.base.DiTextTbl;
import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.system.ProSystemTbl;
import com.avp.config.service.base.DiTextTblService;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.persistence.criteria.*;
import java.util.ArrayList;
import java.util.List;

@Service
@Transactional(rollbackFor = Exception.class)
public class DiTextTblServiceImpl implements DiTextTblService {
    @Autowired
    private DiTextTblRepository diTextTblRepository;
    @Autowired
    private ProSystemTblRepository proSystemTblRepository;

    @Override
    public List<DiTextTbl> getList(DiTextTbl diTextTbl) {
        List<DiTextTbl> diTextTblList=this.diTextTblRepository.findAll(new Specification<DiTextTbl>() {
            @Override
            public Predicate toPredicate(Root<DiTextTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                Join<DiTextTbl,ProSystemTbl> join=root.join("proSystemTbl",JoinType.LEFT);
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(diTextTbl.getDiTextLabel())) {
                    list.add(cb.like(root.get("diTextLabel").as(String.class), "%" +diTextTbl.getDiTextLabel()+ "%"));
                }

                if (diTextTbl.getDiValue()!=null) {
                    list.add(cb.equal(root.get("diValue").as(Integer.class),diTextTbl.getDiValue()));
                }
                if(diTextTbl.getProSystemId()!=null){
                    list.add(cb.equal(root.get("proSystemId").as(Integer.class),diTextTbl.getProSystemId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                //query.orderBy(cb.desc(root.get("diValue").as(Integer.class)));
                List<Order> orders=new ArrayList<>();
                orders.add(cb.asc(root.get("diValue").as(Integer.class)));
                orders.add(cb.asc(root.get("diTextLabel").as(String.class)));
                query.orderBy(orders);
                return query.getRestriction();
            }
        });
        return diTextTblList;
    }

    @Override
    public DiTextTbl saveOrUpdate(DiTextTbl diTextTbl, String operate) throws Exception {
        //id，label,name 唯一
        DiTextTbl diTextTbl2=new DiTextTbl();
        if("update".equals(operate)){

        }else{
             diTextTbl2 =this.diTextTblRepository.findByDiTextLabelAndDiValue(diTextTbl.getDiTextLabel(),diTextTbl.getDiValue());
            if(diTextTbl2!=null){
                throw new Exception("标签和实际值不能都一样");
            }
        }
        diTextTbl2= this.diTextTblRepository.save(diTextTbl);
        ProSystemTbl proSystemTbl=this.proSystemTblRepository.findByProSystemId(diTextTbl2.getProSystemId());
        diTextTbl2.setProSystemTbl(proSystemTbl);
        return  diTextTbl2;
    }

    @Override
    public DiTextTbl detail(int diValue, String diTextLabel) {
        return this.diTextTblRepository.findByDiTextLabelAndDiValue(diTextLabel,diValue);
    }

    @Override
    public void del(int diValue, String diTextLabel) {
       this.diTextTblRepository.deleteByDiTextLabelAndDiValue(diTextLabel,diValue);
    }
}
