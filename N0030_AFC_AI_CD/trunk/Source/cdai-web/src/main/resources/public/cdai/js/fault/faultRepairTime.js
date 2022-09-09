var linesArr = getLines();
var lineId = linesArr[0].id;
var lineName = linesArr[0].text;

var stationsArr;
var stationId;
var stationName;

var equipmentArr;       //设备
var equipmentId;

var modulesArr = getModules();         //模块
var moduleId = modulesArr[0].id;

var startTime, endTime;
var timeSpanArr = getTimeSpans();          //时间周期
var timeSpanId = timeSpanArr[0].id;

var column1, column2;

$(function () {
    $("#timeScope").datepicker("setDate", '-1y');
    initFaultsFactorSearchForm();

    getEquipmentByStationIdOnChange(-1);
});

function getStationsByLineIdOnLineIdChange(lineId) {
    if (lineId == -1) {
        stationsArr = [];
        stationsArr.unshift({id: '-1', text: '全部'});
        $("#station").empty().select2({
            data: stationsArr,
            placeholder: '全部',
            allowClear: true
        });

        equipmentArr.unshift({id: '-1', text: '全部'});
        $("#equipment").empty().select2({
            data: equipmentArr,
            placeholder: '全部',
            allowClear: true
        });
        return false;
    }
    stationsArr = getStationsByLineId(lineId);
    stationId = stationsArr[0].id;
    stationName = stationsArr[1].text;

    stationsArr.unshift({id: '-1', text: '全部'});
    $("#station").empty().select2({
        data: stationsArr,
        placeholder: '全部',
        allowClear: true
    });
    // $("#station").val(stationId).trigger('change');
}

function getEquipmentByStationIdOnChange(stationId) {

    if (stationId == -1) {
        equipmentArr = [];
        equipmentArr.unshift({id: '-1', text: '全部'});
        $("#equipment").empty().select2({
            data: equipmentArr,
            placeholder: '全部',
            allowClear: true
        });
        return false;
    }
    equipmentArr = getEquipmentByStationId(stationId);
    equipmentArr.unshift({id: '-1', text: '全部'});
    $("#equipment").empty().select2({
        data: equipmentArr,
        placeholder: '全部',
        allowClear: true
    });
}

function initFaultsFactorSearchForm() {
    linesArr.unshift({id: '-1', text: '全部'});
    $("#line").select2({
        data: linesArr,
        placeholder: '全部',
        allowClear: true
    });

    $("#station").select2({
        data: stationsArr,
        placeholder: '全部',
        allowClear: true
    });

    $("#equipment").select2({
        data: equipmentArr,
        placeholder: '全部',
        allowClear: true
    });

    $("#module").select2({
        data: modulesArr,
        placeholder: '全部',
        allowClear: true
    });

    $("#timeSpan").select2({
        data: timeSpanArr,
        placeholder: '请选择时间周期',
        allowClear: true
    });
    $("#timeSpan").val(timeSpanId).trigger('change');
}

function search() {
    lineId = $("#line").val();
    stationId = $("#station").val();
    equipmentId = $("#equipment").val();
    moduleId = $("#module").val();
    startTime = $("#timeScope").val();
    timeSpanId = $("#timeSpan").val();

    if ((lineId == "" || lineId == "-1") && (stationId == "" || stationId == "-1") && equipmentId == "-1") {
        linesAndModuleSearch();          //全线路+模块类型

    } else if (lineId != "-1" && stationId == "-1" && equipmentId == "-1") {
        lineAndModuleSearch();           //线路+模块类型

    } else if (lineId != "-1" && stationId != "-1" && equipmentId == "-1") {
        lineAndStationModuleSearch();       //线路+车站+模块类型

    } else if (lineId != "-1" && stationId != "-1" && equipmentId != "-1") {
        lineAndStationEquipmentModuleSearch();        //车站线路模块设备
    }

}

function linesAndModuleSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    param.moduleId = moduleId;
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    //查询全线路和模块
    param.location = 'linesWithModule';

    AJAX_GET('/failureReason/repair', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
        }
    });

    initHightcharts();

}

function lineAndModuleSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    param.moduleId = moduleId;
    param.id = lineId;
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    //查询线路和模块

    param.location = "lineWithModule";

    AJAX_GET('/failureReason/repair', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
        }
    });

    initHightcharts();

}

function lineAndStationModuleSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    param.moduleId = moduleId;
    param.id = stationId;
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    //查询线路和模块,车站
    param.location = "stationWithModule";

    AJAX_GET('/failureReason/repair', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
        }
    });
    initHightcharts();

}

function lineAndStationEquipmentModuleSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    param.moduleId = moduleId;
    param.id = equipmentId;
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    //查询线路，车站，设备和模块
    param.location = "deviceWithModule";

    AJAX_GET('/failureReason/repair', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
        }
    });
    initHightcharts();

}

function initHightcharts() {
    $('#container1').highcharts({
        chart: {
            type: 'column'
        },
        title: {
            text: ' 故障修复时间分析'
        },
        subtitle: {
            text: endTime
        },
        credits: {
            enabled: false  //去掉右下角的链接
        },
        exporting: {
            enabled: false
        },
        xAxis: {
            categories: column1,
            labels: {
                formatter: function () {//提示框中数据样式设置
                    return Highcharts.dateFormat('%Y/%m', this.value);
                }
            },
            crosshair: true
        },
        yAxis: {
            min: 0,
            title: {
                text: '运行时间（m）'
            }
        },
        tooltip: {
            formatter: function () {//提示框中数据样式设置
                var tips = '<b>' + Highcharts.dateFormat('%Y-%m', this.x) + '</b>';
                $.each(this.points, function () {
                    var y = this.y;
                    tips += '<br/><span style="color:' + '">' + y + "m";
                });
                return tips;
            },
            shared: true,
            crosshairs: true
        },
        plotOptions: {
            column: {
                cursor: 'pointer',
                point: {}
            }
        },
        series: [{
            name: '平均修复时间',
            data: column2
        }]
    });
}

