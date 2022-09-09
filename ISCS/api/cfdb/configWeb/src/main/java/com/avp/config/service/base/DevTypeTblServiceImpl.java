package com.avp.config.service.base;

import com.avp.config.dao.base.DevTypeTblRepository;
import com.avp.config.dao.system.ProSystemTblRepository;
import com.avp.config.entity.base.DevTypeTbl;
import com.avp.config.entity.system.ProSystemTbl;
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

@Service
@Transactional(rollbackFor = Exception.class)
public class DevTypeTblServiceImpl implements DevTypeTblService {

    private Logger logger = LoggerFactory.getLogger(DevTypeTblServiceImpl.class);

    @Autowired
    private DevTypeTblRepository devTypeTblRepository;

    @Autowired
    private ProSystemTblRepository proSystemTblRepository;

    @Override
    public List<DevTypeTbl> getList(DevTypeTbl devTypeTbl) {
        List<DevTypeTbl> devTypeTblList=this.devTypeTblRepository.findAll(new Specification<DevTypeTbl>() {
            @Override
            public Predicate toPredicate(Root<DevTypeTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                Join<DevTypeTbl,ProSystemTbl> join=root.join("proSystemTbl",JoinType.LEFT);

                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(devTypeTbl.getDevTypeLabel())) {
                    list.add(cb.like(root.get("devTypeLabel").as(String.class), "%" +devTypeTbl.getDevTypeLabel()+ "%"));
                }

                if (StringUtils.isNotBlank(devTypeTbl.getDevTypeName())) {
                    list.add(cb.like(root.get("devTypeName").as(String.class), "%" +devTypeTbl.getDevTypeName()+ "%"));
                }
                if(devTypeTbl.getProSystemId()!=null){
                    list.add(cb.equal(root.get("proSystemId").as(Integer.class),devTypeTbl.getProSystemId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                //query.orderBy(cb.desc(root.get("diValue").as(Integer.class)));
                List<Order> orders=new ArrayList<>();
                orders.add(cb.asc(root.get("devTypeLabel").as(String.class)));
                query.orderBy(orders);
                return query.getRestriction();
            }
        });
        return devTypeTblList;
    }

    @Override
    public DevTypeTbl saveOrUpdate(DevTypeTbl devTypeTbl, String operate) throws Exception {
        DevTypeTbl devTypeTbl2=new DevTypeTbl();
        List<DevTypeTbl> list = new ArrayList<>();
        if("update".equals(operate)){
            //查询详情
            devTypeTbl2=this.devTypeTblRepository.findByDevTypeLabel(devTypeTbl.getDevTypeLabel());
            if(!devTypeTbl.getDevTypeName().equals(devTypeTbl2.getDevTypeName())){
                list=this.devTypeTblRepository.findByDevTypeName(devTypeTbl.getDevTypeName());
            }
        }else{
            list=this.devTypeTblRepository.findByDevTypeLabelOrDevTypeName(devTypeTbl.getDevTypeLabel(),devTypeTbl.getDevTypeName());
        }
        if(list!=null && list.size()>0){
            throw new Exception("标签或名称重复");
        }

        devTypeTbl2= this.devTypeTblRepository.save(devTypeTbl);
        ProSystemTbl proSystemTbl=this.proSystemTblRepository.findByProSystemId(devTypeTbl2.getProSystemId());
        devTypeTbl2.setProSystemTbl(proSystemTbl);
        return  devTypeTbl2;
    }

    @Override
    public DevTypeTbl detail(String devTypeLabel) {
        return this.devTypeTblRepository.findByDevTypeLabel(devTypeLabel);
    }

    @Override
    public void del(String devTypeLabel) {
        this.devTypeTblRepository.deleteByDevTypeLabel(devTypeLabel);
    }
}
