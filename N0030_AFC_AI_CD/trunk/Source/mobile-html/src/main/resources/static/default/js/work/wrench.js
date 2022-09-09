/**
 * Created by gjj on 2017/4/27.
 */
define(function(require, exports, module) {
    is_token();
    loadlocations();

    var tp = require('tp');
    var utils = require('utils');
    var wrapper = require('_wrapper');
    var workOrderUtils = require('workOrder-utils');
    var uiWorkOrderHtml = require('ems-text!workorder.html');

    var fn = tp.compile(uiWorkOrderHtml);
    var ui = {
        workOrderTable: document.querySelector("#workorder-table")
    };

    /*工单的初始化参数*/
    var page_index = wrapper.getPageInfo().page_index;
    var page_size = wrapper.getPageInfo().page_size;
    var min_work_order_date = wrapper.getPageInfo().min_date;
    var max_work_order_date = wrapper.getPageInfo().max_date;
    var failureDetailPage = null;
    var record = [];
    var recordIndex = 0;
    var workOrders_len = 0;
    var totalPages = 0;
    var hasNotNextPage = false;
    var showAll = true;

    var interId;
    var username;

    $(function () {

        var thisURL = decodeURI(document.URL);
        var params = thisURL.split("&").splice(1,thisURL.split("&").length);
        if( params[0].split("=")[1] =="true" ){
            username =JSON.parse(sessionStorage.getItem("USER")).username;
            /*初始化工单列表*/
            getWorkOrder(showAll, page_index, page_size, false, true);
        } else {
            if($("#code").val() !="123"){
                WX_auth();
                interId = window.setInterval(intervalUsername,1000);
            } else {
                AJAX_USER_LOGIN("admin","admin");
                interId = window.setInterval(intervalUsername,1000);
            };
        }

        mui.each(document.querySelectorAll('.mui-slider-group .mui-scroll'), function (index, pullRefreshEl) {
            mui(pullRefreshEl).pullToRefresh({
                down: {
                    callback: pulldownRefresh
                },
                up: {
                    contentrefresh: '正在加载...',
                    callback: pullupRefresh
                }
            });
        });

    });

    function intervalUsername(){

        if(null !=JSON.parse(sessionStorage.getItem("USER")).username){
            username =JSON.parse(sessionStorage.getItem("USER")).username;
            window.clearInterval(interId);
            /*初始化工单列表*/
            getWorkOrder(showAll, page_index, page_size, false, true);
        }
    };

    function loadlocations() {
        if(null != sessionStorage.getItem('$locations')) return ;
        AJAX_ASYNC_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA + "/locations", {}, function (data_locations) {
            function extractRowId(row) {
                var url = (typeof(row) == 'object' && '_links' in row) ? row._links.self.href : row;
                var reg = /\/(\w+)$/g;
                var _id = reg.exec(url)[1];
                return _id;
            }

            var locations = [];
            if ('_embedded' in data_locations) {
                var location_len = data_locations._embedded.locations.length;
                for (var j = 0; j < location_len; j++) {
                    locations.push({
                        id: extractRowId(data_locations._embedded.locations[j]),
                        parentId: data_locations._embedded.locations[j].parentId,
                        nameCn: data_locations._embedded.locations[j].nameCn,
                        longitude: data_locations._embedded.locations[j].longitude,
                        latitude: data_locations._embedded.locations[j].latitude
                    });
                }
            }
            sessionStorage.setItem('$locations', JSON.stringify(locations));
        })
    }

    /**
     * 获取工单
     * @param {int} page_index 第几页
     * @param {int} page_size 每页记录数
     * @param {boolean} append
     */
    function getWorkOrder(showAll, page_index, page_size, append, first) {

        var wo_url = CONSTANTS.CONF.RESOURCE_WORK_ORDER+ '/workOrders';
        var param = {
            username: username,
            page: 0,
            size: 7,
            projectId:CONSTANTS.BASE.PROJECT_ID,
            sort: 'lastUpdateTime,desc'
        };
        if(append) {
            if(showAll) {
                wo_url += '/search/findByReportEmployeeAndLastUpdateTimeLessThan';
            } else {
                wo_url += '/search/findByReportEmployeeAndParentIdIsNullAndLastUpdateTimeLessThan';
            }
            param.lastUpdateTime = wrapper.formatDate(min_work_order_date);
        } else {
            if(showAll) {
                wo_url += '/search/findByReportEmployeeAndLastUpdateTimeGreaterThan';
            } else {
                wo_url += '/search/findByReportEmployeeAndParentIdIsNullAndLastUpdateTimeGreaterThan';
            }
            param.lastUpdateTime = wrapper.formatDate(max_work_order_date);
            if(!first) {
                param.sort = 'lastUpdateTime,asc';
            } else {
                ui.workOrderTable.innerHTML = '';
            }
        }

        var locations = JSON.parse(sessionStorage.getItem('$locations'));
        var equipment = [];
        var page_number = page_index - 1;

        /**获取workOrders工单表**/
        AJAX_GET_URL(wo_url,param,function (data_workOrders) {
            if('_embedded' in data_workOrders) {
                recordIndex = 0;
                record = [];
                workOrders_len = data_workOrders._embedded.workOrders.length;
                for(var i = 0; i < workOrders_len; i++) {
                    //循环工单表。对于每一个设备id，发请求
                    var wo = data_workOrders._embedded.workOrders[i];
                    showWorkOrderList(locations, wo, append);
                }
            } else {
                console.log("报修界面: 没有更多的工单数据了");
                hasNotNextPage = true;
                return false;
            }
            if('page' in data_workOrders) {
                totalPages = data_workOrders.page.totalPages;
            }
        })

        mui('.mui-scroll-wrapper').scroll();
        mui('.mui-slider-group').pullRefresh().refresh(true);
        var deceleration = mui.os.ios ? 0.003 : 0.0009;
        mui('.mui-scroll-wrapper').scroll({
            bounce: false,
            indicators: true, //是否显示滚动条
            deceleration: deceleration
        });
        //				}, 500);

    };

    function handleRecords(_record, _append) {

        for(var i = 0; i < _record.length; i++) {
            //内层循环，找到第i大的元素，并将其和第i个元素交换
            for(var j = i; j < _record.length; j++) {
                if(_record[i].last_update < _record[j].last_update) {
                    //交换两个元素的位置
                    var temp = _record[i];
                    _record[i] = _record[j];
                    _record[j] = temp;
                }
            }
        }
        if(_append) { //追加
            appendToTable(_record, _append);
        } else { //在第一条前插入
            //若该记录已存在，删除原先的，将该记录插入最上面
            for(var i = 0; i < _record.length; i++) {
                var workOrderli = document.querySelector("[workorder-id='" + _record[i].id + "']");
                if(workOrderli != null) {
                    var workOrderTable = document.getElementById('workorder-table');
                    workOrderTable.removeChild(workOrderli);
                }
            }
            var rs = fn(_record);
            ui.workOrderTable.innerHTML = rs + ui.workOrderTable.innerHTML;
        }
    }

    function showWorkOrderList(locations, wo, append) {
        var wo_lastUpdateTime = wo.lastUpdateTime;
        if(min_work_order_date === wrapper.getPageInfo().min_date) min_work_order_date = max_work_order_date = wo_lastUpdateTime;
        if(wo_lastUpdateTime > max_work_order_date) {
            max_work_order_date = wo_lastUpdateTime;
        } else if(min_work_order_date > wo_lastUpdateTime) {
            min_work_order_date = wo_lastUpdateTime;
        }
        AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA+ '/equipments/'+wo.equipmentId,{},function (data_equipment) {
            var locs = wrapper.getLocationTxtById(locations, data_equipment.locationId);
            /* 故障类型ID为-1的暂时处理逻辑 后续需要优化*/
            record.push({
                "id": wrapper.extractRowId(wo),
                "type_cn": wrapper.getWOTypeCnName(wo.typeId),
                "type_en": wrapper.getWOTypeEnName(wo.typeId),
                "type_id": wo.typeId,
                "device_no": data_equipment.nameCn,
                "line": locs.pop(),
                "station": locs.pop(),
                "status": wrapper.getWOStausName(wo.statusId),
                "statusId": wo.statusId,
                // "creator": workOrderUtils.getUserCNnameByENname(wo.reportEmployee),
                "creator": wo.reportEmployee,
                "fault_type": wo.faultDescriptionId == -1 || !wo.faultDescriptionId ? wo.description : workOrderUtils.getFaultDescById(wo.faultDescriptionId),
                "create_date": wrapper.formatDateTime(wo.creationTime),
                "last_update_date": wrapper.formatDateTime(wo.lastUpdateTime),
                "last_update": wo.lastUpdateTime
            });
            // console.error(JSON.stringify(record));
            if(++recordIndex == workOrders_len) {
                handleRecords(record, append);
            }
        })
    }

    /**
     * 下拉刷新的函数
     */
    function pulldownRefresh() {
        var reportData = this;
        setTimeout(function() {
            var cells = document.querySelectorAll('.mui-table-view-cell');
            page_index = cells.length / page_size + 1;
            getWorkOrder(showAll, page_index, page_size, false);
            reportData.endPullDownToRefresh();
        }, 1000);
    }

    /**
     * 上拉加载的函数
     */
    function pullupRefresh() {
        var reportData = this;
        setTimeout(function() {
            var cells = document.querySelectorAll('.mui-table-view-cell');
            if(cells.length < page_size) {
                reportData.endPullUpToRefresh(true);
            } else {
                page_index = cells.length / page_size + 1;
                getWorkOrder(showAll, page_index, page_size, true);
                reportData.endPullUpToRefresh(hasNotNextPage);
            }
        }, 1000);
    }

    /**
     * show_equipment_picker 显示插件
     **/
    function show_equipment_picker(equipmentData) {
        var equipment_line_station_selectpicker = new mui.PopPicker({
            layer: 3
        });
        equipment_line_station_selectpicker.cancel.addEventListener('tap', function(event) {
            equipment_line_station_selectpicker.hide();
        }, false);
        equipment_line_station_selectpicker.setData(equipmentData);

        equipment_line_station_selectpicker.show(function(items) {
            var item = items;
            if( items[2] && items[2].text ){
                var line = items[0].text;
                var station = items[1].text;
                var equipmentName = items[2].text;
                var equipmentId = items[2].id;
                var targetUrl = "/skip?path=work/equipmentselect&line="+line+"&station="+station+"&equipmentName="+equipmentName+"&equipmentId="+equipmentId;
                window.location.href = targetUrl;
            } else {
                mui.alert("您未选择设备");
            }
            wrapper.closeWaiting();
        });

    }

    /**
     * 编译模板，将数据添加至document
     * @param {Array} _record
     * @param {boolean} _append
     */
    function appendToTable(_record, _append) {
        tp.bind({
            template: 'workorder-item-template',
            element: 'workorder-table',
            model: _record,
            append: _append
        });
    }
})