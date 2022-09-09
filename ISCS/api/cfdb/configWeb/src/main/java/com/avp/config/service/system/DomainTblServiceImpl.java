package com.avp.config.service.system;

import com.avp.config.dao.system.DomainTblRepository;
import com.avp.config.entity.system.DomainTbl;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.thymeleaf.expression.Lists;

import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.util.ArrayList;
import java.util.List;

@Service
@Transactional(rollbackFor = Exception.class)
public class DomainTblServiceImpl implements DomainTblService {
    private Logger logger= LoggerFactory.getLogger(DomainTblServiceImpl.class);

    @Autowired
    private DomainTblRepository domainTblRepository;

    @Override
    public List<DomainTbl> getList(DomainTbl domainTbl) {
        List<DomainTbl> domainTblList=this.domainTblRepository.findAll(new Specification<DomainTbl>() {
            @Override
            public Predicate toPredicate(Root<DomainTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(domainTbl.getDomainName())) {
                    list.add(cb.like(root.get("domainName").as(String.class), "%" + domainTbl.getDomainName()+ "%"));
                }

                if (domainTbl.getDomainId()!=null) {
                    list.add(cb.equal(root.get("domainId").as(Integer.class),domainTbl.getDomainId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("domainId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return domainTblList;
    }

    @Override
    public DomainTbl saveOrUpdate(DomainTbl domainTbl,String operate) throws  Exception {
        //id，label,name 唯一
        DomainTbl domainTbl2=new DomainTbl();
        List<DomainTbl> list = new ArrayList<>();
       if("update".equals(operate)){
            //查询详情
            domainTbl2=this.domainTblRepository.findByDomainId(domainTbl.getDomainId());
            if(domainTbl.getDomainLabel().equals(domainTbl2.getDomainLabel()) && domainTbl.getDomainName().equals(domainTbl2.getDomainName())){

            }else if(!domainTbl.getDomainLabel().equals(domainTbl2.getDomainLabel())){
                list=this.domainTblRepository.findByDomainLabel(domainTbl.getDomainLabel());
            }else if(!domainTbl.getDomainName().equals(domainTbl2.getDomainName())){
                list=this.domainTblRepository.findByDomainName(domainTbl.getDomainName());
            }
       }else{
           list=this.domainTblRepository.findByDomainIdOrDomainLabelOrDomainName(domainTbl.getDomainId(),domainTbl.getDomainLabel(),domainTbl.getDomainName());
       }
       if(list!=null && list.size()>0){
           throw new Exception("域ID、域标签、域名称有重复");
       }
        return this.domainTblRepository.save(domainTbl);
    }


    @Override
    public DomainTbl detail(int domainId) {
        return this.domainTblRepository.findByDomainId(domainId);
    }

    @Override
    public void del(int domainId) {
       this.domainTblRepository.deleteByDomainId(domainId);
    }
}
