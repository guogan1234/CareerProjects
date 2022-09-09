package com.avp.config.service.system;

import com.avp.config.dao.system.ProSystemTblRepository;
import com.avp.config.entity.system.ProSystemTbl;
import com.avp.config.entity.system.StationTbl;
import com.avp.config.service.system.ProSystemTblService;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.util.ArrayList;
import java.util.List;

@Service
@Transactional(rollbackFor=Exception.class)
public class ProSystemTblServiceImpl implements ProSystemTblService {

    @Autowired
    private ProSystemTblRepository proSystemTblRepository;


    @Override
    public List<ProSystemTbl> getList(ProSystemTbl proSystemTbl) {
        List<ProSystemTbl> proSystemTblList=this.proSystemTblRepository.findAll(new Specification<ProSystemTbl>() {
            @Override
            public Predicate toPredicate(Root<ProSystemTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(proSystemTbl.getProSystemName())) {
                    list.add(cb.like(root.get("proSystemName").as(String.class), "%" + proSystemTbl.getProSystemName()+ "%"));
                }

                if (proSystemTbl.getProSystemId()!=null) {
                    list.add(cb.equal(root.get("proSystemId").as(Integer.class),proSystemTbl.getProSystemId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("proSystemId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return proSystemTblList;
    }

    @Override
    public ProSystemTbl saveOrUpdate(ProSystemTbl proSystemTbl, String operate) throws Exception {
        //id，label,name 唯一
        ProSystemTbl proSystemTbl2=new ProSystemTbl();
        List<ProSystemTbl> list = new ArrayList<>();
        if("update".equals(operate)){
            //查询详情
            proSystemTbl2=this.proSystemTblRepository.findByProSystemId(proSystemTbl.getProSystemId());
            if(proSystemTbl.getProSystemLabel().equals(proSystemTbl2.getProSystemLabel()) && proSystemTbl.getProSystemName().equals(proSystemTbl2.getProSystemName())){

            }else if(!proSystemTbl.getProSystemLabel().equals(proSystemTbl2.getProSystemLabel())){
                list=this.proSystemTblRepository.findByProSystemLabel(proSystemTbl.getProSystemLabel());
            }else if(!proSystemTbl.getProSystemName().equals(proSystemTbl2.getProSystemName())){
                list=this.proSystemTblRepository.findByProSystemName(proSystemTbl.getProSystemName());
            }
        }else{
            list=this.proSystemTblRepository.findByProSystemIdOrProSystemLabelOrProSystemName(proSystemTbl.getProSystemId(),proSystemTbl.getProSystemLabel(),proSystemTbl.getProSystemName());
        }
        if(list!=null && list.size()>0){
            throw new Exception("专业ID、专业标签、专业名称有重复");
        }
        return this.proSystemTblRepository.save(proSystemTbl);
    }

    @Override
    public ProSystemTbl detail(int proSystemId) {
        return this.proSystemTblRepository.findByProSystemId(proSystemId);
    }

    @Override
    public void del(int proSystemId) {
        this.proSystemTblRepository.deleteByProSystemId(proSystemId);
    }
}
