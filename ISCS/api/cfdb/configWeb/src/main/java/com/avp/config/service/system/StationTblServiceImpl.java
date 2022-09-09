package com.avp.config.service.system;

import com.avp.config.dao.system.DomainTblRepository;
import com.avp.config.dao.system.StationTblRepository;
import com.avp.config.entity.base.DevTypeTbl;
import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.system.ProSystemTbl;
import com.avp.config.entity.system.StationTbl;
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
public class StationTblServiceImpl implements StationTblService {
    @Autowired
    private StationTblRepository stationTblRepository;

    @Autowired
    private DomainTblRepository domainTblRepository;

    @Override
    public List<StationTbl> getList(StationTbl stationTbl) {
        List<StationTbl> stationTblList=this.stationTblRepository.findAll(new Specification<StationTbl>() {
            @Override
            public Predicate toPredicate(Root<StationTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                Join<StationTbl,DomainTbl> join=root.join("domainTbl",JoinType.LEFT);

                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(stationTbl.getStationName())) {
                    list.add(cb.like(root.get("stationName").as(String.class), "%" + stationTbl.getStationName()+ "%"));
                }

                if (stationTbl.getStationId()!=null) {
                    list.add(cb.equal(root.get("stationId").as(Integer.class),stationTbl.getStationId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("stationId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return stationTblList;
    }

    @Override
    public StationTbl saveOrUpdate(StationTbl stationTbl, String operate) throws Exception {
        //id，label,name 唯一
        StationTbl stationTbl2=new StationTbl();
        List<StationTbl> list = new ArrayList<>();
        if("update".equals(operate)){
            //查询详情
            stationTbl2=this.stationTblRepository.findByStationId(stationTbl.getStationId());
            if(stationTbl.getStationLabel().equals(stationTbl2.getStationLabel()) && stationTbl.getStationName().equals(stationTbl2.getStationName())){

            }else if(!stationTbl.getStationLabel().equals(stationTbl2.getStationLabel())){
                list=this.stationTblRepository.findByStationLabel(stationTbl.getStationLabel());
            }else if(!stationTbl.getStationName().equals(stationTbl2.getStationName())){
                list=this.stationTblRepository.findByStationName(stationTbl.getStationName());
            }
        }else{
            list=this.stationTblRepository.findByStationIdOrStationLabelOrStationName(stationTbl.getStationId(),stationTbl.getStationLabel(),stationTbl.getStationName());
        }
        if(list!=null && list.size()>0){
            throw new Exception("车站ID、车站标签、车站名称有重复");
        }
        stationTbl2= this.stationTblRepository.save(stationTbl);
        DomainTbl domainTbl=this.domainTblRepository.findByDomainId(stationTbl2.getDomainId());
        stationTbl2.setDomainTbl(domainTbl);
        return stationTbl2;
    }

    @Override
    public StationTbl detail(int stationId) {
        return this.stationTblRepository.findByStationId(stationId);
    }

    @Override
    public void del(int stationId) {
        this.stationTblRepository.deleteByStationId(stationId);
    }
}
