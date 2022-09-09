package com.avp.config.service.alarm;

import com.avp.config.dao.alarm.AlarmTypeTblRepository;
import com.avp.config.entity.alarm.AlarmTypeTbl;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
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
@Transactional(rollbackFor = Exception.class)
public class AlarmTypeTblServiceImpl implements  AlarmTypeTblService {
    private Logger logger = LoggerFactory.getLogger(AlarmTypeTblServiceImpl.class);

    @Autowired
    private AlarmTypeTblRepository alarmTypeTblRepository;

    @Override
    public List<AlarmTypeTbl> getList(AlarmTypeTbl alarmTypeTbl) {
        List<AlarmTypeTbl> alarmTypeTblList=this.alarmTypeTblRepository.findAll(new Specification<AlarmTypeTbl>() {
            @Override
            public Predicate toPredicate(Root<AlarmTypeTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(alarmTypeTbl.getTypeName())) {
                    list.add(cb.like(root.get("typeName").as(String.class), "%" +alarmTypeTbl.getTypeName()+ "%"));
                }

                if (alarmTypeTbl.getTypeId()!=null) {
                    list.add(cb.equal(root.get("typeId").as(Integer.class),alarmTypeTbl.getTypeId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("typeId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return alarmTypeTblList;
    }

    @Override
    public AlarmTypeTbl saveOrUpdate(AlarmTypeTbl alarmTypeTbl, String operate) throws Exception {
        AlarmTypeTbl alarmTypeTbl2=new AlarmTypeTbl();
        List<AlarmTypeTbl> list = new ArrayList<>();
        if("update".equals(operate)){
            //查询详情
            alarmTypeTbl2=this.alarmTypeTblRepository.findByTypeId(alarmTypeTbl.getTypeId());
            if(!alarmTypeTbl.getTypeName().equals(alarmTypeTbl2.getTypeName())){
                list = this.alarmTypeTblRepository.findByTypeName(alarmTypeTbl.getTypeName());
            }
        }else{
            list=this.alarmTypeTblRepository.findByTypeIdOrTypeName(alarmTypeTbl.getTypeId(),alarmTypeTbl.getTypeName());
        }
        if(list!=null && list.size()>0){
            throw new Exception("报警类型ID、报警类型名称有重复");
        }
        return this.alarmTypeTblRepository.save(alarmTypeTbl);
    }

    @Override
    public AlarmTypeTbl detail(Integer typeId) {
        return this.alarmTypeTblRepository.findByTypeId(typeId);
    }

    @Override
    public void del(Integer typeId) {
        this.alarmTypeTblRepository.deleteByTypeId(typeId);
    }
}
