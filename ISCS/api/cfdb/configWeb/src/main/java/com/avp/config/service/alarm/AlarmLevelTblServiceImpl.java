package com.avp.config.service.alarm;

import com.avp.config.dao.alarm.AlarmActTblRepository;
import com.avp.config.dao.alarm.AlarmLevelTblRepository;
import com.avp.config.entity.alarm.AlarmActTbl;
import com.avp.config.entity.alarm.AlarmLevelTbl;
import org.apache.commons.lang.StringUtils;
import org.hibernate.Session;
import org.hibernate.jpa.HibernateEntityManager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.util.ArrayList;
import java.util.List;

@Service
@Transactional(rollbackFor = Exception.class)
public class AlarmLevelTblServiceImpl implements AlarmLevelTblService {

    private Logger logger = LoggerFactory.getLogger(AlarmLevelTblServiceImpl.class);

    @Autowired
    private AlarmLevelTblRepository alarmLevelTblRepository;

    @Autowired
    private AlarmActTblRepository alarmActTblRepository;

    @PersistenceContext
    private EntityManager entityManager;

    @Override
    public List<AlarmLevelTbl> getList(AlarmLevelTbl alarmLevelTbl) {
        List<AlarmLevelTbl> alarmLevelTblList = this.alarmLevelTblRepository.findAll(new Specification<AlarmLevelTbl>() {
            @Override
            public Predicate toPredicate(Root<AlarmLevelTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(alarmLevelTbl.getLevelName())) {
                    list.add(cb.like(root.get("levelName").as(String.class), "%" + alarmLevelTbl.getLevelName() + "%"));
                }

                if (alarmLevelTbl.getLevelId() != null) {
                    list.add(cb.equal(root.get("levelId").as(Integer.class), alarmLevelTbl.getLevelId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("levelId").as(Integer.class)));
                return query.getRestriction();
            }
        });

        List<AlarmActTbl> alarmActTblList = this.alarmActTblRepository.findAll();

        //将alarmLevelList转化为游离态
        HibernateEntityManager hEntityManager = (HibernateEntityManager) entityManager;
        Session session = hEntityManager.getSession();


        //for循环获取alarmActName
        for (int i = 0; i < alarmLevelTblList.size(); i++) {
            session.evict(alarmLevelTblList.get(i));

            String alarmAct = alarmLevelTblList.get(i).getAlarmAct();
            if (StringUtils.isBlank(alarmAct)) {
                continue;
            }
            String alarmActStr = "";
            for (AlarmActTbl alarmActTbl : alarmActTblList) {
                if (alarmAct.contains(alarmActTbl.getActId().toString())) {
                    alarmActStr += alarmActTbl.getActName() + ",";
                }
            }

            if (alarmActStr.length() > 0) {
                alarmActStr = alarmActStr.substring(0, alarmActStr.length() - 1);
            }
            alarmLevelTblList.get(i).setAlarmAct(alarmActStr);
        }

        return alarmLevelTblList;
    }

    @Override
    public AlarmLevelTbl saveOrUpdate(AlarmLevelTbl alarmLevelTbl, String operate) throws Exception {
        AlarmLevelTbl alarmLevelTbl2 = new AlarmLevelTbl();
        List<AlarmLevelTbl> list = new ArrayList<>();
        if ("update".equals(operate)) {
            //查询详情
            alarmLevelTbl2 = this.alarmLevelTblRepository.findByLevelId(alarmLevelTbl.getLevelId());
            if (!alarmLevelTbl2.getLevelName().equals(alarmLevelTbl.getLevelName())) {
                list = this.alarmLevelTblRepository.findByLevelName(alarmLevelTbl.getLevelName());
            }
        } else {
            list = this.alarmLevelTblRepository.findByLevelIdOrLevelName(alarmLevelTbl.getLevelId(), alarmLevelTbl.getLevelName());
        }
        if (list != null && list.size() > 0) {
            throw new Exception("报警等级ID、报警等级名称有重复");
        }
        return this.alarmLevelTblRepository.save(alarmLevelTbl);
    }

    @Override
    public AlarmLevelTbl detail(Integer levelId) {
        return this.alarmLevelTblRepository.findByLevelId(levelId);
    }

    @Override
    public void del(Integer levelId) {
        this.alarmLevelTblRepository.deleteByLevelId(levelId);
    }
}
