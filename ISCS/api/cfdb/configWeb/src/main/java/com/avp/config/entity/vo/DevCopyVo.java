package com.avp.config.entity.vo;

public class DevCopyVo {
    private String sourceDevTpLabel;//源设备标签

    private String devTpLabel;//设备标签

    private String devTpName;//设备名称

    private String sourcePointTpLabel;//源点标签

    private String pointTpLabel;//点标签

    private String pointTpName;//点名称

    private String pointType;//点类型

    private String sourceInstanceLabel;//源设备实例标签

    private String devInstanceLabel;//设备实例标签

    private String devInstanceName;//设备实例名称

    private Integer sourceDomainId;//源域

    private Integer sourceStationId;//源车站

    private Integer stationId;//车站

    //所属域
    private Integer domainId;

    private Integer locationId;//位置

    private  Integer increasedType;//自增类型
    
    private String starInput;//首个字符

    private  Integer step;//间隔

    private  Integer  count;//增加个数
    
    //标签
  	private String devLabel;

  	//名称
  	private String devName;



  	//所属子系统
  	private Integer proSystemId;

  	//所属责任区
  	private Integer regionId;

  	//所属RTU
  	private Integer rtuId;

  	//设备类型
  	private String devTypeLabel;

  	//控制权转移组
  	private String ctrlTransGrpLabel;

  	//状态
  	private Integer devStatus;

  	//是否摘要
  	private Integer isSummary;

  	//摘要值
  	private Integer summaryValue;

  	//摘要状态
  	private Integer summaryStatus;

    public String getStarInput() {
		return starInput;
	}

	public void setStarInput(String starInput) {
		this.starInput = starInput;
	}

	public String getDevLabel() {
		return devLabel;
	}

	public void setDevLabel(String devLabel) {
		this.devLabel = devLabel;
	}

	public String getDevName() {
		return devName;
	}

	public void setDevName(String devName) {
		this.devName = devName;
	}

	public Integer getDomainId() {
		return domainId;
	}

	public void setDomainId(Integer domainId) {
		this.domainId = domainId;
	}



	public Integer getProSystemId() {
		return proSystemId;
	}

	public void setProSystemId(Integer proSystemId) {
		this.proSystemId = proSystemId;
	}

	public Integer getRegionId() {
		return regionId;
	}

	public void setRegionId(Integer regionId) {
		this.regionId = regionId;
	}

	public Integer getRtuId() {
		return rtuId;
	}

	public void setRtuId(Integer rtuId) {
		this.rtuId = rtuId;
	}

	public String getDevTypeLabel() {
		return devTypeLabel;
	}

	public void setDevTypeLabel(String devTypeLabel) {
		this.devTypeLabel = devTypeLabel;
	}

	public String getCtrlTransGrpLabel() {
		return ctrlTransGrpLabel;
	}

	public void setCtrlTransGrpLabel(String ctrlTransGrpLabel) {
		this.ctrlTransGrpLabel = ctrlTransGrpLabel;
	}

	public Integer getDevStatus() {
		return devStatus;
	}

	public void setDevStatus(Integer devStatus) {
		this.devStatus = devStatus;
	}

	public Integer getIsSummary() {
		return isSummary;
	}

	public void setIsSummary(Integer isSummary) {
		this.isSummary = isSummary;
	}

	public Integer getSummaryValue() {
		return summaryValue;
	}

	public void setSummaryValue(Integer summaryValue) {
		this.summaryValue = summaryValue;
	}

	public Integer getSummaryStatus() {
		return summaryStatus;
	}

	public void setSummaryStatus(Integer summaryStatus) {
		this.summaryStatus = summaryStatus;
	}

	public String getDevTpLabel() {
        return devTpLabel;
    }

    public void setDevTpLabel(String devTpLabel) {
        this.devTpLabel = devTpLabel;
    }

    public String getDevTpName() {
        return devTpName;
    }

    public void setDevTpName(String devTpName) {
        this.devTpName = devTpName;
    }

    public String getPointTpLabel() {
        return pointTpLabel;
    }

    public void setPointTpLabel(String pointTpLabel) {
        this.pointTpLabel = pointTpLabel;
    }

    public String getPointTpName() {
        return pointTpName;
    }

    public void setPointTpName(String pointTpName) {
        this.pointTpName = pointTpName;
    }

    public String getPointType() {
        return pointType;
    }

    public void setPointType(String pointType) {
        this.pointType = pointType;
    }

    public String getDevInstanceLabel() {
        return devInstanceLabel;
    }

    public void setDevInstanceLabel(String devInstanceLabel) {
        this.devInstanceLabel = devInstanceLabel;
    }

    public String getDevInstanceName() {
        return devInstanceName;
    }

    public void setDevInstanceName(String devInstanceName) {
        this.devInstanceName = devInstanceName;
    }

    public Integer getStationId() {
        return stationId;
    }

    public void setStationId(Integer stationId) {
        this.stationId = stationId;
    }

    public Integer getSourceDomainId() {
        return sourceDomainId;
    }

    public void setSourceDomainId(Integer sourceDomainId) {
        this.sourceDomainId = sourceDomainId;
    }

    public Integer getSourceStationId() {
        return sourceStationId;
    }

    public void setSourceStationId(Integer sourceStationId) {
        this.sourceStationId = sourceStationId;
    }

    public Integer getLocationId() {
        return locationId;
    }

    public void setLocationId(Integer locationId) {
        this.locationId = locationId;
    }

    public Integer getIncreasedType() {
        return increasedType;
    }

    public void setIncreasedType(Integer increasedType) {
        this.increasedType = increasedType;
    }

    public Integer getStep() {
        return step;
    }

    public void setStep(Integer step) {
        this.step = step;
    }

    public Integer getCount() {
        return count;
    }

    public void setCount(Integer count) {
        this.count = count;
    }

    public String getSourceDevTpLabel() {
        return sourceDevTpLabel;
    }

    public void setSourceDevTpLabel(String sourceDevTpLabel) {
        this.sourceDevTpLabel = sourceDevTpLabel;
    }

    public String getSourcePointTpLabel() {
        return sourcePointTpLabel;
    }

    public void setSourcePointTpLabel(String sourcePointTpLabel) {
        this.sourcePointTpLabel = sourcePointTpLabel;
    }

    public String getSourceInstanceLabel() {
        return sourceInstanceLabel;
    }

    public void setSourceInstanceLabel(String sourceInstanceLabel) {
        this.sourceInstanceLabel = sourceInstanceLabel;
    }
}
