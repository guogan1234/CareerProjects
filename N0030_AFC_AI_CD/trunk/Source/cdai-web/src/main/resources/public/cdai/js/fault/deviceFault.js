var linesArr = getLines();
var lineId = linesArr[0].id;
var lineName;

var stationsArr;
var stationId;
var stationName;

var deviceTypeArr = getDeviceTypes();
var deviceTypeId = deviceTypeArr[1].id;

var timeSpanArr = getTimeSpans();    //时间周期
var timeSpanId = timeSpanArr[0].id;

var startTime;
var endTime;
var column1, column2, column3, deviceIdList;


$(function () {
    $("#timeScope").datepicker("setDate", '-1y');
    initDeviceFaultsSearchForm();
    getStationsByLineIdOnLineIdChange(lineId);
});

function getStationsByLineIdOnLineIdChange(lineId) {
    stationsArr = getStationsByLineId(lineId);

    stationsArr.unshift({id: '-1', text: '全部'});
    $("#station").empty().select2({
        data: stationsArr,
        placeholder: '请选择车站',
        allowClear: true
    });

}

function initDeviceFaultsSearchForm() {
    $("#line").select2({
        data: linesArr,
        placeholder: '请选择线路',
        allowClear: true
    });

    $("#line").val(lineId).trigger('change');

    $("#deviceType").select2({
        data: deviceTypeArr,
        placeholder: '请选择设备类型',
        allowClear: true
    });

    $("#deviceType").val(deviceTypeId).trigger('change');

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
    deviceTypeId = $("#deviceType").val();
    startTime = $("#timeScope").val();
    timeSpanId = $("#timeSpan").val();

    if (stationId != -1) {
        stationSearch();
    } else {
        lineSearch();
    }

    // 存入缓存，备页面跳转详情使用
    localStorage.setItem("startTime", startTime);
    localStorage.setItem("timeSpanId", timeSpanId);

}

function stationSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    param.deviceType = deviceTypeId;
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    //根据车站查询
    param.stationId = stationId;

    AJAX_GET('/DeviceFailed/percent', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
            deviceIdList = data.result.idList;
        }
    });

    AJAX_GET('/DeviceFailed/count', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column3 = data.result.yValue;
            deviceIdList = data.result.idList;
        }
    });
    initHightcharts();
}

function lineSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    param.deviceType = deviceTypeId;
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    //根据线路查询
    param.lineId = lineId;

    AJAX_GET('/DeviceFailed/percent', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
            deviceIdList = data.result.idList;     //设备名称相对应的模块名称
        }
    });

    AJAX_GET('/DeviceFailed/count', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column3 = data.result.yValue;
            deviceIdList = data.result.idList;
        }
    });
    initHightcharts();
}

function initHightcharts() {
    $('#container1').highcharts({
        chart: {
            zoomType: 'xy'
        },
        subtitle: {
            text: endTime
        },

        title: {
            text: ' 设备故障分析'
        },

        credits: {
            enabled: false  //去掉右下角的链接
        },

        exporting: {
            enabled: false
        },

        xAxis: {
            categories: column1,
            crosshair: true
        },
        yAxis: [{ // 第一条Y轴
            title: {
                text: '数量',
                style: {
                    color: '#ff59e3'
                }
            },
            labels: {
                format: '{value}',
                style: {
                    color: '#ff59e3'

                }
            },
            plotOptions: {
                column: {
                    cursor: 'pointer',
                    point: {
                        events: {
                            click: function (e) {
                                var index = e.point.index;
                                localStorage.setItem("deviceId", deviceIdList[index]);
                                window.location.href = '/fault/device';
                            }
                        }
                    }
                }

            },
            opposite: true
        }, { // 第二条Y轴
            title: {
                text: '百分比',
                style: {
                    color: '#55e9ff'
                }
            },
            labels: {
                format: '{value} %',
                style: {
                    color: '#55e9ff'
                }

            }
        }],
        plotOptions: {
            column: {
                cursor: 'pointer',
                point: {
                    events: {
                        click: function (e) {
                            var index = e.point.index;
                            localStorage.setItem("deviceId", deviceIdList[index]);
                            window.location.href = '/fault/device';
                        }
                    }
                }
            }
        },
        tooltip: {
            shared: true
        },

        series: [{
            name: '百分比',
            color: '#55e9ff',
            type: 'column',
            yAxis: 1,
            data: column2,
            tooltip: {
                valueSuffix: ' %'
            }

        }, {
            name: '数量',
            color: '#ff59e3',
            type: 'column',
            data: column3,
            tooltip: {
                valueSuffix: ''
            }
        }]

    });
}



