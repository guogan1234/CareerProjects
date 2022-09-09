package com.avp.config.service.alarm;

import com.avp.config.dao.alarm.AlarmSourceTblRepository;
import com.avp.config.dao.alarm.vw.AlarmSourceActTypeVwRepository;
import com.avp.config.entity.alarm.AlarmSourceTbl;
import com.avp.config.entity.alarm.vw.AlarmSourceActTypeVw;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;

import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.util.ArrayList;
import java.util.List;
@Service
public class AlarmSourceTblServiceImpl implements  AlarmSourceTblService{

    private Logger logger = LoggerFactory.getLogger(AlarmLevelTblServiceImpl.class);

    @Autowired
    private AlarmSourceTblRepository alarmSourceTblRepository;
    @Autowired
    private AlarmSourceActTypeVwRepository alarmSourceActTypeVwRepository;

    @Override
    public List<AlarmSourceActTypeVw> getList(AlarmSourceTbl alarmSourceTbl) {
        List<AlarmSourceActTypeVw> alarmSourceTblList = this.alarmSourceActTypeVwRepository.findAll(new Specification<AlarmSourceActTypeVw>() {
            @Override
            public Predicate toPredicate(Root<AlarmSourceActTypeVw> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(alarmSourceTbl.getSourceName())) {
                    list.add(cb.like(root.get("sourceName").as(String.class), "%" + alarmSourceTbl.getSourceName() + "%"));
                }

                if (alarmSourceTbl.getSourceId() != null) {
                    list.add(cb.equal(root.get("sourceId").as(Integer.class), alarmSourceTbl.getSourceId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("sourceId").as(Integer.class)));
                return query.getRestriction();
            }
        });

        return alarmSourceTblList;
    }

    @Override
    public AlarmSourceTbl saveOrUpdate(AlarmSourceTbl alarmSourceTbl, String operate) throws Exception {
        AlarmSourceTbl alarmSourceTbl1 = new AlarmSourceTbl();
        List<AlarmSourceTbl> list = new ArrayList<>();
        if ("update".equals(operate)) {
            //查询详情
            alarmSourceTbl1 = this.alarmSourceTblRepository.findBySourceId(alarmSourceTbl.getSourceId());
            if (!alarmSourceTbl.getSourceName().equals(alarmSourceTbl1.getSourceName())) {
                list = this.alarmSourceTblRepository.findBySourceName(alarmSourceTbl.getSourceName());
            }
        } else {
            list = this.alarmSourceTblRepository.findBySourceIdOrSourceName(alarmSourceTbl.getSourceId(),alarmSourceTbl.getSourceName());
        }
        if (list != null && list.size() > 0) {
            throw new Exception("报警来源ID、报警来源名称有重复");
        }
        return this.alarmSourceTblRepository.save(alarmSourceTbl);
    }

    @Override
    public AlarmSourceTbl detail(Integer sourceId) {
        return this.alarmSourceTblRepository.findBySourceId(sourceId);
    }

    @Override
    public void del(Integer sourceId) {
        this.alarmSourceTblRepository.deleteBySourceId(sourceId);
    }
}
