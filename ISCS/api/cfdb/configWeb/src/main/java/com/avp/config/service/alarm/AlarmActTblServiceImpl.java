package com.avp.config.service.alarm;

import com.avp.config.dao.alarm.AlarmActTblRepository;
import com.avp.config.entity.alarm.AlarmActTbl;
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
public class AlarmActTblServiceImpl implements  AlarmActTblService {

    private Logger logger = LoggerFactory.getLogger(AlarmActTblServiceImpl.class);

    @Autowired
    private AlarmActTblRepository alarmActTblRepository;

    @Override
    public List<AlarmActTbl> getList(AlarmActTbl alarmActTbl) {

        List<AlarmActTbl> alarmActTblList=this.alarmActTblRepository.findAll(new Specification<AlarmActTbl>() {
            @Override
            public Predicate toPredicate(Root<AlarmActTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(alarmActTbl.getActName())) {
                    list.add(cb.like(root.get("actName").as(String.class), "%" +alarmActTbl.getActName()+ "%"));
                }

                if (alarmActTbl.getActId()!=null) {
                    list.add(cb.equal(root.get("actId").as(Integer.class),alarmActTbl.getActId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("actId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return alarmActTblList;
    }

    @Override
    public AlarmActTbl saveOrUpdate(AlarmActTbl alarmActTbl, String operate) throws Exception {

        AlarmActTbl alarmActTbl2=new AlarmActTbl();
        List<AlarmActTbl> list = new ArrayList<>();
        if("update".equals(operate)){
            //查询详情
            alarmActTbl2=this.alarmActTblRepository.findByActId(alarmActTbl.getActId());
           if(!alarmActTbl.getActName().equals(alarmActTbl2.getActName())){
               list = this.alarmActTblRepository.findByActName(alarmActTbl.getActName());
           }
        }else{
            list=this.alarmActTblRepository.findByActIdOrActName(alarmActTbl.getActId(),alarmActTbl.getActName());
        }
        if(list!=null && list.size()>0){
            throw new Exception("动作ID、动作名称有重复");
        }
        return this.alarmActTblRepository.save(alarmActTbl);
    }

    @Override
    public AlarmActTbl detail(Integer actId) {
        return this.alarmActTblRepository.findByActId(actId);
    }

    @Override
    public void del(Integer actId) {
        this.alarmActTblRepository.deleteByActId(actId);
    }

}
