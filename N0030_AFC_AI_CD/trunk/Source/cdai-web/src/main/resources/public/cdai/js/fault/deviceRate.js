var linesArr = getLines();
var lineId = linesArr[0].id;
var lineName = linesArr[0].text;

var stationsArr;
var stationId;
var stationName;

var timeSpanArr = getTimeSpans();
var timeSpan = timeSpanArr[0].id;

var startTime, endTime;
var column1, column2, column3;


$(function () {
    $("#timeScope").datepicker("setDate", '-1y');
    getStationsByLineIdOnLineIdChange(lineId);
    initFaultsSearchForm();
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

function initFaultsSearchForm() {
    $("#line").select2({
        data: linesArr,
        placeholder: '请选择线路',
        allowClear: true
    });

    $("#line").val(lineId).trigger('change');

    $("#timeSpan").select2({
        data: timeSpanArr,
        placeholder: '请选择时间周期',
        allowClear: true
    });
    $("#timeSpan").val(timeSpan).trigger('change');
}

function search() {
    lineId = $("#line").val();
    stationId = $("#station").val();
    startTime = $('#timeScope ').val();   //开始时间
    timeSpanId = $("#timeSpan").val();

    if (stationId != -1) {
        stationSearch();
    } else {
        lineSearch();
    }

}

function stationSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    param.id = stationId;
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    //根据车站查询
    param.location = 'station';

    AJAX_GET('/deviceDown/percent', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;

        }
    });

    AJAX_GET('/deviceDown/count', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column3 = data.result.yValue;

        }
    });
    initHightcharts();
}

function lineSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    param.id = lineId;
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    //根据线路查询
    param.location = 'line';

    AJAX_GET('/deviceDown/percent', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;

        }
    });

    AJAX_GET('/deviceDown/count', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column3 = data.result.yValue;

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
            text: ' 设备宕机分析'
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
                text: '故障时长',
                style: {
                    color: '#ff59e3'
                }
            },
            labels: {
                format: '{value} m',
                style: {
                    color: '#ff59e3'

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
            name: '故障时长',
            color: '#ff59e3',
            type: 'column',
            data: column3,
            tooltip: {
                valueSuffix: 'm'
            }
        }]

    });
}
