var lineId = localStorage.getItem("lineId");
var lineName = localStorage.getItem("lineName");

var stationId = localStorage.getItem("stationId");
var stationName = localStorage.getItem("stationName");

var startTime = localStorage.getItem("startTime");    //开始时间
var timeSpanId = localStorage.getItem("timeSpanId");  //结束时间

var tagId = localStorage.getItem("tagId");
var tagName = localStorage.getItem("tagName");

var endTime;
var column1, column2, column3;

$(function () {
    if (stationId != -1) {
        stationSearch();
    } else {
        lineSearch();
    }
});

function stationSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    param.tagId = tagId;
    //根据车站查询
    param.stationId = stationId;

    AJAX_GET('/tagFailedDetail/count', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
        }
    });

    AJAX_GET('/tagFailedDetail/percent', param, function callback(data) {
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
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    param.tagId = tagId;
    //根据线路查询
    param.lineId = lineId;

    AJAX_GET('/tagFailedDetail/count', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
        }
    });

    AJAX_GET('/tagFailedDetail/percent', param, function callback(data) {
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
            text: tagName + ' 故障分布图'
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
            data: column3,
            tooltip: {
                valueSuffix: ' %'
            }

        }, {
            name: '数量',
            color: '#ff59e3',
            type: 'column',
            data: column2,
            tooltip: {
                valueSuffix: ''
            }
        }]

    });
}


