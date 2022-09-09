package com.avp.config.dao.alarm.vw;

import com.avp.config.entity.alarm.vw.AlarmSourceActTypeVw;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/14.
 */
public interface AlarmSourceActTypeVwRepository extends JpaRepository<AlarmSourceActTypeVw, Integer>, JpaSpecificationExecutor<AlarmSourceActTypeVw> {

    public AlarmSourceActTypeVw findBySourceId(Integer sourceId);

    public List<AlarmSourceActTypeVw> findBySourceName(String sourceName);

    public List<AlarmSourceActTypeVw> findBySourceIdOrSourceName(Integer sourceId, String sourceName);
}
