package com.avp.config.service.base;

import com.avp.config.dao.base.DoGrpTblRepository;
import com.avp.config.dao.system.ProSystemTblRepository;
import com.avp.config.entity.base.DiTextTbl;
import com.avp.config.entity.base.DoGrpTbl;
import com.avp.config.entity.system.ProSystemTbl;
import com.avp.config.service.base.DoGrpTblService;
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
public class DoGrpTblServiceImpl implements DoGrpTblService {
    @Autowired
    private DoGrpTblRepository doGrpTblRepository;
    @Autowired
    private ProSystemTblRepository proSystemTblRepository;

    @Override
    public List<DoGrpTbl> getList(DoGrpTbl doGrpTbl) {
        List<DoGrpTbl> doGrpTblList=this.doGrpTblRepository.findAll(new Specification<DoGrpTbl>() {
            @Override
            public Predicate toPredicate(Root<DoGrpTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                Join<DoGrpTbl,ProSystemTbl> join=root.join("proSystemTbl",JoinType.LEFT);
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(doGrpTbl.getDoGrpLabel())) {
                    list.add(cb.like(root.get("doGrpLabel").as(String.class), "%" +doGrpTbl.getDoGrpLabel()+ "%"));
                }

                if(doGrpTbl.getDoNumber()!=null){
                    list.add(cb.equal(root.get("doNumber").as(Integer.class),doGrpTbl.getDoNumber()));
                }
                if(doGrpTbl.getProSystemId()!=null){
                    list.add(cb.equal(root.get("proSystemId").as(Integer.class),doGrpTbl.getProSystemId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                //query.orderBy(cb.desc(root.get("diValue").as(Integer.class)));
                List<Order> orders=new ArrayList<>();
                orders.add(cb.asc(root.get("doOrder").as(Integer.class)));
                orders.add(cb.asc(root.get("doGrpLabel").as(String.class)));
                query.orderBy(orders);
                return query.getRestriction();
            }
        });
        return doGrpTblList;
    }

    @Override
    public DoGrpTbl saveOrUpdate(DoGrpTbl doGrpTbl, String operate) throws Exception {
        //id，label,name 唯一
        DoGrpTbl doGrpTbl2=new DoGrpTbl();
        if("update".equals(operate)){

        }else{
             doGrpTbl2 =this.doGrpTblRepository.findByDoOrderAndDoGrpLabel(doGrpTbl.getDoOrder(),doGrpTbl.getDoGrpLabel());
            if(doGrpTbl2!=null){
                throw new Exception("标签和序号不能都一样");
            }
        }
        doGrpTbl2=this.doGrpTblRepository.save(doGrpTbl);
        ProSystemTbl proSystemTbl=this.proSystemTblRepository.findByProSystemId(doGrpTbl2.getProSystemId());
        doGrpTbl2.setProSystemTbl(proSystemTbl);
        return  doGrpTbl2;
    }

    @Override
    public DoGrpTbl saveDoGrpConfig(DoGrpTbl doGrpTbl,String allows) throws Exception {
        DoGrpTbl doGrpTbl2=this.doGrpTblRepository.findByDoOrderAndDoGrpLabel(doGrpTbl.getDoOrder(),doGrpTbl.getDoGrpLabel());
        doGrpTbl2.setDoAllow(Integer.valueOf(allows,2));
        return this.doGrpTblRepository.save(doGrpTbl2);
    }

    @Override
    public DoGrpTbl detail(int doOrder, String doGrpLabel) {
        return this.doGrpTblRepository.findByDoOrderAndDoGrpLabel(doOrder,doGrpLabel);
    }

    @Override
    public void del(int doOrder, String doGrpLabel) {
         this.doGrpTblRepository.deleteByDoOrderAndDoGrpLabel(doOrder,doGrpLabel);
    }
}
