package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.AiTblEntity;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;

public interface AiTblRepository extends JpaRepository<AiTblEntity, String>{

    public void deleteByAiLabel (String aiLabel);

    @Modifying
    @Query("update AiTblEntity ac set ac.aiName=?1 where ac.pointTpLabel=?2")
    public void updateAiNameByLabel(String aiName, String pointTpLabel);
}
