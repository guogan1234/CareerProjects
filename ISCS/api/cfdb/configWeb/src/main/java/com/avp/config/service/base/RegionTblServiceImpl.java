package com.avp.config.service.base;

import com.avp.config.dao.base.RegionTblRepository;
import com.avp.config.dao.system.ProSystemTblRepository;
import com.avp.config.entity.base.DoGrpTbl;
import com.avp.config.entity.base.RegionTbl;
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
public class RegionTblServiceImpl implements RegionTblService {
    private Logger logger = LoggerFactory.getLogger(RegionTblServiceImpl.class);

    @Autowired
    private RegionTblRepository regionTblRepository;
    @Autowired
    private ProSystemTblRepository proSystemTblRepository;

    @Override
    public List<RegionTbl> getList(RegionTbl regionTbl) {
        List<RegionTbl> regionTblList = this.regionTblRepository.findAll(new Specification<RegionTbl>() {
            @Override
            public Predicate toPredicate(Root<RegionTbl> root, CriteriaQuery<?> criteriaQuery, CriteriaBuilder criteriaBuilder) {
                Join<RegionTbl,ProSystemTbl> join=root.join("proSystemTbl",JoinType.LEFT);
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(regionTbl.getRegionName())) {
                    list.add(criteriaBuilder.like(root.get("regionName").as(String.class), "%" + regionTbl.getRegionName() + "%"));
                }

                if (regionTbl.getRegionId()!=null) {
                    list.add(criteriaBuilder.equal(root.get("regionId").as(Integer.class),regionTbl.getRegionId()));
                }

                if (regionTbl.getProSystemId()!=null) {
                    list.add(criteriaBuilder.equal(root.get("proSystemId").as(Integer.class),regionTbl.getProSystemId()));
                }

                Predicate[] p = new Predicate[list.size()];
                criteriaQuery.where( criteriaBuilder.and(list.toArray(p)));
                //添加排序的功能
                //query.orderBy(cb.desc(root.get("diValue").as(Integer.class)));
                List<Order> orders=new ArrayList<>();
                orders.add(criteriaBuilder.asc(root.get("regionId").as(Integer.class)));
                criteriaQuery.orderBy(orders);
                return criteriaQuery.getRestriction();
            }
        });

        return regionTblList;
    }

    @Override
    public RegionTbl saveOrUpdate(RegionTbl regionTbl, String operate) throws Exception {
        RegionTbl regionTbl2 =new RegionTbl();
        List<RegionTbl> list = new ArrayList<>();
        if("update".equals(operate)){
            //查询详情
            regionTbl2=this.regionTblRepository.findByRegionId(regionTbl.getRegionId());
            if(!regionTbl.getRegionLabel().equals(regionTbl2.getRegionLabel())){
                 list=this.regionTblRepository.findByRegionLabel(regionTbl.getRegionLabel());
            }else if(!regionTbl.getRegionName().equals(regionTbl2.getRegionName())){
                list=this.regionTblRepository.findByRegionName(regionTbl.getRegionName());
            }
        }else{
            list=this.regionTblRepository.findByRegionIdOrRegionLabelOrRegionName(regionTbl.getRegionId(),regionTbl.getRegionLabel(),regionTbl.getRegionName());
        }
        if(list!=null && list.size()>0){
            throw new Exception("ID或标签或名称重复");
        }
        regionTbl2= this.regionTblRepository.save(regionTbl);
        if(regionTbl2.getProSystemId()!=null){
            ProSystemTbl proSystemTbl=this.proSystemTblRepository.findByProSystemId(regionTbl2.getProSystemId());
            regionTbl2.setProSystemTbl(proSystemTbl);
        }
        return  regionTbl2;
    }

    @Override
    public RegionTbl detail(Integer regionId) {
        return this.regionTblRepository.findByRegionId(regionId);
    }

    @Override
    public void del(Integer regionId) {
        this.regionTblRepository.deleteByRegionId(regionId);
    }
}
