package com.avp.config.service.alarm;

import com.avp.config.dao.alarm.AlarmInfoTblRepository;
import com.avp.config.dao.alarm.vw.AlarmInfoDomainStationRegionTypeLevelVwRepository;
import com.avp.config.entity.alarm.AlarmInfoTbl;
import com.avp.config.entity.alarm.vw.AlarmInfoDomainStationRegionTypeLevelVw;
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
public class AlarmInfoTblServiceImpl implements AlarmInfoTblService {

    @Autowired
    private AlarmInfoTblRepository alarmInfoTblRepository;

    @Autowired
    private AlarmInfoDomainStationRegionTypeLevelVwRepository alarmInfoDomainStationRegionTypeLevelVwRepository;

    @Override
    public List<AlarmInfoDomainStationRegionTypeLevelVw> getList(AlarmInfoTbl alarmInfoTbl) {
        List<AlarmInfoDomainStationRegionTypeLevelVw> alarmInfoTblList = this.alarmInfoDomainStationRegionTypeLevelVwRepository.findAll(new Specification<AlarmInfoDomainStationRegionTypeLevelVw>() {
            @Override
            public Predicate toPredicate(Root<AlarmInfoDomainStationRegionTypeLevelVw> root, CriteriaQuery<?> query, CriteriaBuilder cb) {

                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(alarmInfoTbl.getLabel())) {
                    list.add(cb.like(root.get("label").as(String.class), "%" + alarmInfoTbl.getLabel() + "%"));
                }

                if (StringUtils.isNotBlank(alarmInfoTbl.getDevTypeLabel())) {
                    list.add(cb.like(root.get("devTypeLabel").as(String.class), "%" + alarmInfoTbl.getDevTypeLabel() + "%"));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //?????????????????????
                //query.orderBy(cb.desc(root.get("diValue").as(Integer.class)));
                List<Order> orders = new ArrayList<>();
                orders.add(cb.asc(root.get("second").as(Integer.class)));
                orders.add(cb.asc(root.get("msecond").as(Integer.class)));
                orders.add(cb.asc(root.get("label").as(String.class)));
                query.orderBy(orders);
                return query.getRestriction();
            }
        });
        return alarmInfoTblList;
    }

    @Override
    public AlarmInfoTbl saveOrUpdate(AlarmInfoTbl alarmInfoTbl, String operate) throws Exception {
        //label,second,msecond ??????
        AlarmInfoTbl alarmInfoTbl1 = new AlarmInfoTbl();
        if ("update".equals(operate)) {

        } else {
            alarmInfoTbl1 = this.alarmInfoTblRepository.findByLabelAndSecondAndMsecond(alarmInfoTbl.getLabel(), alarmInfoTbl.getSecond(), alarmInfoTbl.getMsecond());
            if (!org.springframework.util.StringUtils.isEmpty(alarmInfoTbl1)) {
                throw new Exception("??????????????????????????????????????????????????????????????????????????????!");
            }
        }

        this.alarmInfoTblRepository.save(alarmInfoTbl);
        return alarmInfoTbl;
    }

    @Override
    public AlarmInfoTbl detail(String label, Integer second, Integer msecond) {
        return this.alarmInfoTblRepository.findByLabelAndSecondAndMsecond(label, second, msecond);
    }

    @Override
    public void del(String label, Integer second, Integer msecond) {
        this.alarmInfoTblRepository.deleteByLabelAndSecondAndMsecond(label, second, msecond);
    }
}
