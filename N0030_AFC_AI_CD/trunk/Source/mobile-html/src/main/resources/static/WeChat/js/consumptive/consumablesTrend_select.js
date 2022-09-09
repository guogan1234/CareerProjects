var linesArr = getLines();
var lineId = 0;
var lineName;

var stationsArr = [];
var stationId = 0;//车站 默认选择全部
var stationName;

var equipmentArr = [];
var equipmentId = 0;//设备 默认选择全部
var equipmentName;

var maintenanceUnitId = 0;  //维保单位
var maintenanceUnitName = null;

var maintenanceStaffId = 0;
var maintenanceStaffName = null;  //维保人员


$(function () {
    initFaults();
    selectStation();
    selectEquipment();
});


function initFaults() {
    linesArr.splice(0, {value: 0}, {title: '全部'});
    lineId = 0;
    lineName = '全部';

    $("#line").select({
        title: "选择线路",
        items: linesArr,
        input: linesArr[0].title,
        beforeClose: function (values, titles) {
            lineId = values;
            lineName = titles;
            selectStation();
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });

    $("#maintenanceUnit").select({
        title: "维保单位",
        items: [],
        beforeClose: function (values, titles) {
            maintenanceUnitId = values;
            maintenanceUnitName = titles;
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });
    $("#maintenanceStaff").select({
        title: "维保人员",
        items: [],
        beforeClose: function (values, titles) {
            maintenanceStaffId = values;
            maintenanceStaffName = titles;
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });
}

function selectStation() {
    if (lineId == '' || lineId == undefined) {
        lineId = linesArr[0].value;
        lineName = linesArr[0].title;
    }

    //联动    先清空再添加
    $('#station_div').empty();
    $('#station_div').html("<input class=\"weui-input\" type=\"text\" id=\"station\">");

    if (lineId && lineId != 0) {
        stationsArr = getStationsByLineId(lineId);
    }
    stationsArr.splice(0, {value: 0}, {title: '全部'});
    stationId = 0;
    stationName = '全部';
    $("#station").select({
        title: "选择车站",
        items: stationsArr,
        input: stationsArr[0].title,
        beforeClose: function (values, titles) {
            stationId = values;
            stationName = titles;
            selectEquipment();
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });
}

function selectEquipment() {
    if (stationId == '') {
        stationId = stationsArr[0].value;
        stationName = stationsArr[0].title;
    }

    //联动    先清空再添加
    $('#equipment_div').empty();
    $('#equipment_div').html("<input class=\"weui-input\" type=\"text\" id=\"equipment\">");

    if (stationId) {
        equipmentArr = getEquipmentByStationId(stationId);
    }
    equipmentArr.splice(0, {value: 0}, {title: '全部'});
    equipmentId = 0;
    equipmentName = '全部';
    $("#equipment").select({
        title: "选择设备",
        items: equipmentArr,
        input: equipmentArr[0].title,
        beforeClose: function (values, titles) {
            equipmentId = values;
            equipmentName = titles;
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });
}

function btnSubmit() {
    if (lineId == 0) {
        $.alert("请选择线路！", "耗材去向条件筛选");
        return;
    }
    if (stationId == 0) {
        $.alert("请选择车站！", "耗材去向条件筛选");
        return;
    }
    if (equipmentId == 0) {
        $.alert("请选择设备！", "耗材去向条件筛选");
        return;
    }
    if (maintenanceUnitId == 0) {
        $.alert("请选择维保单位！", "耗材去向条件筛选");
        return;
    }
    if (maintenanceStaffId == 0) {
        $.alert("请选择维保人员！", "耗材去向条件筛选");
        return;
    }

    $.showLoading();
    localStorage.setItem("ct-lineId", lineId);
    localStorage.setItem("ct-lineName", lineName);
    localStorage.setItem("ct-stationId", stationId);
    localStorage.setItem("ct-deviceId", equipmentId);
    localStorage.setItem("ct-deviceName", equipmentName);
    localStorage.setItem("ct-maintenanceUnitId", maintenanceUnitId);
    localStorage.setItem("ct-maintenanceUnitName", maintenanceUnitName);
    localStorage.setItem("ct-maintenanceStaffId", maintenanceStaffId);
    localStorage.setItem("ct-maintenanceStaffName", maintenanceStaffName);


    window.location.href = 'skip?path=consumptive/consumablesTrend';
}