package com.avp.config.dao;

import com.avp.config.entity.FormulaTpTbl;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface FormulaTpTblRepository extends JpaRepository<FormulaTpTbl,String>{
    public List<FormulaTpTbl> findAllBy();
}
