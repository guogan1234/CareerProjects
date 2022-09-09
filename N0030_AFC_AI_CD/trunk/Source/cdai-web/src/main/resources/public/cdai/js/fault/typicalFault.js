var linesArr = getLines();
var lineId = linesArr[0].id;
var lineName;

var stationsArr;
var stationId;
var stationName;

var timeSpanArr = getTimeSpans();    //时间周期
var timeSpanId = timeSpanArr[3].id;

var startTime;
var endTime;

var column1, column2, column3, tagIdList;

$(function () {
    $("#timeScope").datepicker("setDate", '-1y','');
    initFaultsSearchForm();
    getStationsByLineIdOnLineIdChange(lineId);
    // search();

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
        placeholderOption: "first",
        allowClear: true
    });
    $("#line").val(lineId).trigger('change');

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
    startTime = $('#timeScope ').val();   //开始时间
    timeSpanId = $("#timeSpan").val();        //结束时间

    lineName = $('#line').select2('data')[0].text;
    stationName = $("#station").select('data')[0].text;

    if (stationId != -1) {
        //选择车站查询典型故障的百分比和数量
        stationSearch();
    } else {
        //选择线路查询典型故障的百分比和数量
        lineSearch();
    }
    // 存入缓存，备页面跳转详情使用
    localStorage.setItem("lineId", lineId);
    localStorage.setItem("lineName", lineName);
    localStorage.setItem("stationId", stationId);
    localStorage.setItem("stationName", stationName);
    localStorage.setItem("startTime", startTime);
    localStorage.setItem("timeSpanId", timeSpanId);
}

function stationSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";
    //根据车站查询
    param.stationId = stationId;

    AJAX_GET('/TagFailed/count', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
            tagIdList = data.result.idList;
        }
    });

    AJAX_GET('/TagFailed/percent', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column3 = data.result.yValue;
            tagIdList = data.result.idList;
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
    //根据线路查询
    param.lineId = lineId;

    AJAX_GET('/TagFailed/count', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column2 = data.result.yValue;
            tagIdList = data.result.idList;     //模块名称相对应的tagId
        }
    });

    AJAX_GET('/TagFailed/percent', param, function callback(data) {
        if (data.code > 0) {
            column1 = data.result.xValue;
            column3 = data.result.yValue;
            tagIdList = data.result.idList;
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
            text: ' 典型故障分析'
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
                                localStorage.setItem("tagId", tagIdList[index]);
                                localStorage.setItem("tagName", column1[index]);
                                window.location.href = '/fault/typical';
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
                            localStorage.setItem("tagId", tagIdList[index]);
                            localStorage.setItem("tagName", column1[index]);
                            window.location.href = '/fault/typical';
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







