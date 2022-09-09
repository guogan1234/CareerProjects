package com.avp.config.dao.base;

import com.avp.config.entity.base.DevTypeTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface DevTypeTblRepository extends JpaRepository<DevTypeTbl, String>, JpaSpecificationExecutor<DevTypeTbl> {
    DevTypeTbl save(DevTypeTbl devTypeTbl);

    List<DevTypeTbl> findByDevTypeLabelOrDevTypeName(String devTypeLabel,String devTypeName);

    List<DevTypeTbl> findByDevTypeName(String devTypeName);

    public  DevTypeTbl findByDevTypeLabel(String devTypeLabel);

    public void deleteByDevTypeLabel(String devTypeLabel);
}
