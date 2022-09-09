define(function(require, exports, module){

    is_token();
	//simple array or json
	var self = exports;
	var woStatus = {
		'WO_CREATED':1,
		'RPT_SENDED':2,
		'AS_CONFIRM':3,
		'AS_SENDED':4,
		'RPR_CONFIRM':5,
		'RPR_ARRIVED':6,
		'RPR_COMPLATED':7,
		'WO_UNHANDLED':8,
		'WO_COMPLATED':9,
		'WO_SPLIT': 10
	};
	var signInWay = {
		'LOCATION_SIGN_IN': 1,
		'CAMERA_SIGN_IN': 2,
		'SCAN_SIGN_IN': 3
	}
	var woFilterType = {
		'WO_ALL': 1,
		'WO_ORIGIN': 2,
		'WO_UNHANDLED': 3
	}
	var maintenanceStep = ['车控室登记','设备内登记','保养单填写','车控室注销'];
	var woStatusTxt = ['工单已创建','报修已派发','调度已确认','调度已派发','维修已确认','维修已到达','维修已完成','遗留','工单完成','已拆分'];
	var woNextStatusTxt = ['报修派发','调度确认','调度派发','维修确认','维修到达','维修完成','已完成','重新处理','已完成','已拆分'];
	var woWaitingStatusTxt = ['等待派发','等待调度确认','等待调度派发','等待维修确认','等待维修到达','正在维修','已完成','等待重新处理','已完成','已拆分'];
	var canAssignStatusTxt = ['报修已派发','调度已派发','调度已确认','维修已确认','遗留'];
	var faultType = ['电源线、数据线安装不到位或松动','系统有异物阻塞光感','识别器安装错误','钱箱安装错误','金属爪接触不良','电子板故障','投币面板绿灯不亮,指示灯坏','投币面板绿灯不亮,系统有异物阻塞光感','投币面板绿灯不亮,与工控机通信异常','投币面板绿灯不亮,本机有故障','电源线、数据线安装不到位或松动','金属爪接触不良','马达盒推杆不到位','PD9光感被遮挡',
	'支架主板坏','马达盒无动作','进币口开启位置太低','进币口电磁阀故障','进币口O型圈断','进币口有异物堵塞','面板安装错误','支架电磁阀故障','电磁阀弹簧松','112170皮带掉落','支架滚轴轴承卡住','识别器报通讯故障','识别器脏','测速片故障','时钟光感故障','纸币破旧','皮带脱落导致卡币','驱动皮带磨损导致卡币','传输机太脏','回旋处停币','退币通道有异物卡住',
	'退币通道皮带掉落','疏导板变形','疏导轮太脏','退闭口打不开','PD5光感脏','PD5光感挡片位置不对','PD67光感脏','退币口电磁阀故障','面板安装不正确','钱箱铁滑轮不灵活','支架皮带脱落','马达盒推杆不到位','马达盒无动作','支架电磁阀故障'];
	var woTypeCnTxt = ['故障维修工单','月度保养工单','年度保养工单'];
	var woTypeEnTxt = ['repairWo','maintenanceWo','maintenanceWo'];
	var canAssignType = ['故障维修工单','月度保养工单','年度保养工单'];
	var signInWayTxt = ['定位签到','拍照签到','扫码签到'];
	var repairWayTxt = ['现场修','换件修','缺件修'];
	var pageIndex = 1, pageSize = 7, minId = maxId = 0, minDate = maxDate = '1970-01-01 00:00:00', maxSize = 1000;
	var maxDistance = 1000;
	var mui;
	var mask;
	
	self.init = function(m) {
		mui = m;
	};
	
	self.getWOStausName = function(stat_id){
		return (stat_id<1||stat_id>woStatusTxt.length) ? 'undefind' : woStatusTxt[stat_id-1];
	};
	
	//detail timeline show
	self.getWONextStatusName = function(stat_id){
		return (stat_id<1||stat_id>woNextStatusTxt.length) ? 'undefind' : woNextStatusTxt[stat_id-1];
	};
	
	//detail head show
	self.woWaitingStatusTxt = function(stat_id){
		return (stat_id<1||stat_id>woWaitingStatusTxt.length) ? 'undefind' : woWaitingStatusTxt[stat_id-1];
	};
	
	self.getFaultTypeName = function(f_type_id){
		return (f_type_id<1||f_type_id>faultType.length) ? 'undefind' : faultType[f_type_id-1];
	};
	
	self.getWOTypeCnName = function(type_id){
		return (type_id<1||type_id>woTypeCnTxt.length) ? 'undefind' : woTypeCnTxt[type_id-1];
	};
	
	self.getWOTypeEnName = function(type_id){
		return (type_id<1||type_id>woTypeEnTxt.length) ? 'undefind' : woTypeEnTxt[type_id-1];
	};
	
	self.getSignInWayTxt= function(sign_in_id){
		return (sign_in_id<1||sign_in_id>signInWayTxt.length) ? 'undefind' : signInWayTxt[sign_in_id-1];
	};
	self.getRepairWayTxt= function(repair_way_id){
		return (repair_way_id<1||repair_way_id>repairWayTxt.length) ? 'undefind' : repairWayTxt[repair_way_id-1];
	};
	
	self.extractRowId = function(row) {
		var url = (typeof(row) == 'object' && '_links' in row) ? row._links.self.href : row;			
		var reg = /\/(\w+)$/g;
		var _id = reg.exec(url)[1];
		return _id;
	};
	
	//return array eg.['stationName','lineName']
	self.getLocationTxtById = function(loc_list, loc_id) {
		var cur;
		var arrLocName = [];
		
		function getLoc(lid) {
			for(var idx in loc_list) {
				if(loc_list[idx].id == lid)
				return loc_list[idx];
			}
		}
		
		cur = getLoc(loc_id);
		arrLocName.push(utils.trim(cur.nameCn));
		while(true){
			if(!cur.parentId) break;
			cur = getLoc(cur.parentId);
			arrLocName.push(utils.trim(cur.nameCn));
		}
		return arrLocName;
	};
	
	function PrefixInteger(num, n) {
        return (Array(n).join(0) + num).slice(-n);
    }
	
	//format ISO8601 'yyyy/mm/dd hh:mm:ss'
	self.formatDateTime = function(utc_str) {
		var st = Date.parse(utc_str);
		if(false) {
			var newDate = new Date();
			newDate.setTime(st);
			return newDate.toLocaleString(); 
		} else {
			var parts = utc_str.match(/\d+/g);
		 	var isoTime = Date.UTC(parts[0], parts[1] - 1, parts[2], parts[3], parts[4], parts[5]);
		  	var isoDate = new Date(isoTime);
			var arrDate = [PrefixInteger(isoDate.getFullYear(),4),PrefixInteger(isoDate.getMonth()+1,2),PrefixInteger(isoDate.getDate(),2)];
			var arrTime = [PrefixInteger(isoDate.getHours(),2),PrefixInteger(isoDate.getMinutes(),2),PrefixInteger(isoDate.getSeconds(),2)];
			return arrDate.join('-') + ' ' + arrTime.join(':');
		}
	};
	
	self.getWOStatusEnum = function(key) {
		return woStatus[key];
	};
	
	self.getSignInWayEnum = function(key) {
		return signInWay[key];
	};
	
	self.getWoFilterTypeEnum = function(key) {
		return woFilterType[key];
	};
	
	self.canAssign = function(status_cn, type_cn) {
		return (-1 != $.inArray(status_cn,canAssignStatusTxt)) && (-1 != $.inArray(type_cn, canAssignType));
	};
	
	self.showWaiting = function(){
		if(!mask) mask = mui.createMask();
		mask.show();
		plus.nativeUI.showWaiting();
	};
	self.closeWaiting = function(){
		if(mask) mask.close();plus.nativeUI.closeWaiting();
	};
	
	self.getPageInfo = function(){
		return {
			page_index:pageIndex,
			page_size:pageSize,
			min_id:minId,
			max_id:maxId,
			min_date:minDate,
			max_date:maxDate,
			max_size: maxSize
		};
	};
	
	self.compareGT= function(s, d){
		if(isNaN(s) || isNaN(d)) return false;
		return parseInt(s) > parseInt(d);
	};
	
	self.createSerialNo = function(){
	    var date = new Date();
	    year = date.getFullYear();
	    month = date.getMonth() + 1;
	    day = date.getDate()<10 ?"0" +date.getDate():date.getDate();
	    hour = date.getHours() < 10 ? "0" + date.getHours() : date.getHours();
	    minute = date.getMinutes() < 10 ? "0" + date.getMinutes() : date.getMinutes();
	    second = date.getSeconds() < 10 ? "0" + date.getSeconds() : date.getSeconds();
	    var SerialNo=year+""+month+""+day+""+hour+""+minute+""+second+""+Math.floor(Math.random() * 10000) + 1;
	    return SerialNo;
	};
	//'yyyy/mm/dd hh:mm:ss'
	self.formatDate = function(utc_str) {
		var parts = utc_str.match(/\d+/g);
	 	var isoTime = Date.UTC(parts[0], parts[1] - 1, parts[2], parts[3], parts[4], parts[5]);
	  	var isoDate = new Date(isoTime);
		var arrDate = [PrefixInteger(isoDate.getFullYear(),4),PrefixInteger(isoDate.getMonth()+1,2),PrefixInteger(isoDate.getDate(),2)];
		var arrTime = [PrefixInteger(isoDate.getHours(),2),PrefixInteger(isoDate.getMinutes(),2),PrefixInteger(isoDate.getSeconds(),2)];
		return arrDate.join('/') + ' ' + arrTime.join(':');
	};
	self.getMaintenanceStep = function() {
		return maintenanceStep;
	}
	self.getMaxDistance = function(){
		return maxDistance;
	}
	self.getBaseUrl = function(){
		return baseUrl;
	}
	self.getBaseImgUrl = function(){
		return baseImgUrl;
	}
	self.getDownloadKnowledgebaseUrl = function() {
		return downloadKnowledgebaseUrl;
	}
	self.getImageDownloadUrl = function() {
		return imageDownloadUrl;
	}
	//input获得焦点，加载软键盘
	var nativeWebview, imm, InputMethodManager;
    var initNativeObjects = function() {
        if (mui.os.android) {
            var main = plus.android.runtimeMainActivity();
            var Context = plus.android.importClass("android.content.Context");
            InputMethodManager = plus.android.importClass("android.view.inputmethod.InputMethodManager");
            imm = main.getSystemService(Context.INPUT_METHOD_SERVICE);
        } else {
            nativeWebview = plus.webview.currentWebview().nativeInstanceObject();
        }
    };
	self.showSoftInput = function() {
		initNativeObjects();
        if (mui.os.android) {
            imm.toggleSoftInput(0, InputMethodManager.SHOW_FORCED);
        } else {
            nativeWebview.plusCallMethod({
                "setKeyboardDisplayRequiresUserAction": false
            });
        }
        setTimeout(function() {
           //此处可写具体逻辑设置获取焦点的input
           var inputElem = document.querySelector('input');
                  inputElem.focus(); 
        }, 200);
    };
    self.hangdle_fix = function(source) {
		var index = "=";
		var sec_index = ",";
		var index_postion = source.indexOf(index);
		var sec_index_postion = source.indexOf(sec_index);

		var id = source.substr(index_postion + 1, sec_index_postion - index_postion - 1);
		return id;

	};
	self.hangdle_fixAndReason = function(source) {
    	var array = [];
		var fixIndex = "=";
		var fixSecIndex = ",";
		
		console.log(JSON.stringify(source));
		for ( var i = 0;i < source.length ; i++ ) {
			var href = source[i]._links.self.href;
			var fixPostion = href.indexOf(fixIndex);
			var secFixPostion = href.indexOf(fixSecIndex);
			
			var reasonPosition = href.lastIndexOf("=");
			
			var fixId = href.substr(fixPostion + 1, secFixPostion - fixPostion - 1);
			var reasonId = href.substr(reasonPosition + 1, href.length - 2);
			
			var obj = {
				"fixId":fixId,
				"reasonId":reasonId
			};
			var equals = false;
			for( var j = 0 ;j<array.length;j++){
				if( (array[j].fixId == obj.fixId && array[j].reasonId == obj.reasonId) ){
					equals = true;
					break;
				}
			}
			if(!equals){
				array.push(obj);
			}
			
		}
		return array;

	}
    self.hangdle_component = function(source) {
		var index = "=";
		var secindex = "%";
		var index_postion = source.indexOf(index);
		var secindex_postion = source.lastIndexOf(secindex);
		var id = source.substr(index_postion + 1, secindex_postion - index_postion - 2);
		return id;
	}
    self.handle_id = function(str, lastindex) {
		var id = null;
		if (str != null) {
			var position_lastindex = str.lastIndexOf(lastindex);
		}
		if (position_lastindex) {
			id = str.substr(position_lastindex + 1);
		}
		return id;
	}
	
});