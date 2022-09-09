package com.avp.config.service.impl;

import com.avp.config.dao.AccTpTblRepository;
import com.avp.config.entity.AccTpTbl;
import com.avp.config.service.AccTpTblService;
import com.avp.config.util.UniversalStatus;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
@Transactional(rollbackFor = Exception.class)
public class AccTpTblServiceImpl implements AccTpTblService {
	@Autowired
	private AccTpTblRepository accTpTblRepository;
	
    @Override
    public void save(AccTpTbl accTpTbl) {
    	accTpTblRepository.save(accTpTbl);
    }

    @Override
    public void update(AccTpTbl accTpTbl) {
    	accTpTblRepository.save(accTpTbl);
    }

    @Override
    public void del(String accTpLabel) {
    	//此处删除点模板（当点模板下有点实例时不允许删除，逻辑待定）
    	AccTpTbl aTbl = new AccTpTbl();
    	aTbl.setAccTpLabel(accTpLabel);
    	accTpTblRepository.delete(aTbl);
    }

    @Override
    public AccTpTbl getDetail(String accTpLabel) {
        return accTpTblRepository.findByAccTpLabel(accTpLabel);
    }

    @Override
    public List<AccTpTbl> getList(String devTpLabel) {
    	
        return null;
    }

    @Override
    public void copy(AccTpTbl accTpTbl) {

    }
}
