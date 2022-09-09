package com.avp.config.service.impl;

import com.avp.config.dao.DioTpTblRepository;
import com.avp.config.entity.DioTpTbl;
import com.avp.config.service.DioTpTblService;
import com.avp.config.util.UniversalStatus;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
@Transactional(rollbackFor = Exception.class)
public class DioTpTblServiceImpl implements DioTpTblService {
    @Autowired
    private DioTpTblRepository dioTpTblRepository;

    @Override
    public DioTpTbl save(DioTpTbl dioTpTbl) {
        return dioTpTblRepository.save(dioTpTbl);
    }

    @Override
    public void update(DioTpTbl dioTpTbl) {

    }

    @Override
    public void del(String dioTpLabel) {
      this.dioTpTblRepository.deleteByDioTpLabel(dioTpLabel);
    }

    @Override
    public DioTpTbl getDetail(String dioTpLabel) {
        return this.dioTpTblRepository.findByDioTpLabel(dioTpLabel);
    }

    @Override
    public List<DioTpTbl> getList(String devTpLabel) {
        return this.dioTpTblRepository.findByDevTpLabel(devTpLabel);
    }

    @Override
    public void copy(DioTpTbl dioTpTbl) {

    }
}
