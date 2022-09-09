package com.avp.config.service.impl;

import com.avp.config.dao.FormulaTpTblRepository;
import com.avp.config.entity.FormulaTpTbl;
import com.avp.config.service.FormulaTpTblService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
@Transactional(rollbackFor = Exception.class)
public class FormulaTpTblServiceImpl implements FormulaTpTblService {
    @Autowired
    private FormulaTpTblRepository formulaTpTblRepository;

    @Override
    public List<FormulaTpTbl> getList() {
        return this.formulaTpTblRepository.findAllBy();
    }
}
