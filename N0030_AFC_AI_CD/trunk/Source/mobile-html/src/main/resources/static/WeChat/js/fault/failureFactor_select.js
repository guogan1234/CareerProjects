var linesArr = getLines();
var lineId = 0;
var lineName;

var stationsArr = [];
var stationId = 0;//车站 默认选择全部
var stationName;

var equipmentArr = [];
var equipmentId = 0;//设备 默认选择全部
var equipmentName;

var deviceTypeId = 0;
var deviceTypeName = null;

var ModulesArr = getModules();
var moduleId = ModulesArr[0].value;
var moduleName = ModulesArr[0].title;

var timeId = 0;
var timeName = null;

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
    var newArr3 = [];
    newArr3.push({value: 1, title: "TVM"});
    newArr3.push({value: 2, title: "闸机"});
    $("#deviceType").select({
        title: "设备类型",
        items: newArr3,
        beforeClose: function (values, titles) {
            deviceTypeId = values;
            deviceTypeName = titles;
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });

    $("#module").select({
        title: "选择模块",
        items: ModulesArr,
        input: moduleName,
        beforeClose: function (values, titles) {
            moduleId = values;
            moduleName = titles;
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });
    var newArr = [];
    newArr.push({value: 3, title: "3个月"});
    newArr.push({value: 6, title: "6个月"});
    newArr.push({value: 9, title: "9个月"});
    newArr.push({value: 12, title: "12个月"});
    $("#time").select({
        title: "选择时间范围",
        items: newArr,
        beforeClose: function (values, titles) {
            timeId = values;
            timeName = titles;
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
        $.alert("请选择线路！", "故障因子分析条件筛选");
        return;
    }
    if (stationId == 0) {
        $.alert("请选择车站！", "故障因子分析条件筛选");
        return;
    }
    if (deviceTypeId == 0) {
        $.alert("请选择设备类型！", "故障因子分析条件筛选");
        return;
    }
    if (equipmentId == 0) {
        $.alert("请选择设备！", "故障因子分析条件筛选");
        return;
    }
    if (moduleId == 0) {
        $.alert("请选择模块名称！", "故障因子分析条件筛选");
        return;
    }
    if (timeId == 0) {
        $.alert("请选择时间范围！", "故障因子分析条件筛选");
        return;
    }

    $.showLoading();
    localStorage.setItem("ff-lineId", lineId);
    localStorage.setItem("ff-lineName", lineName);
    localStorage.setItem("ff-stationId", stationId);
    localStorage.setItem("ff-deviceTypeId", deviceTypeId);
    localStorage.setItem("ff-deviceTypeName", deviceTypeName);
    localStorage.setItem("ff-equipmentId", equipmentId);
    localStorage.setItem("ff-equipmentName", equipmentName);
    localStorage.setItem("ff-moduleId", moduleId);
    localStorage.setItem("ff-moduleName", moduleName);
    localStorage.setItem("ff-timeId", timeId);
    localStorage.setItem("ff-timeName", timeName);

    window.location.href = 'skip?path=fault/failureFactor';
}

