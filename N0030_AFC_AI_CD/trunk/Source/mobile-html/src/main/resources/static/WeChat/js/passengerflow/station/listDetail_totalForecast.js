var stationId1 = localStorage.getItem('totf-stationId1');
var stationId2 = localStorage.getItem('totf-stationId2');
if (stationId1 == null || stationId2 == null) { //首次进来时，默认选中前两个站
    stationId1 = getStations()[0].value;
    stationId2 = getStations()[1].value;
}

var calender = localStorage.getItem('totf-calender');
if (calender == null) { //首次进来时，默认选中昨天
    calender = new Date().pattern("yyyy-MM-dd");

}
var calenderParam = calender.replace(/-/g, '/');

$(function () {
    initDatas();
});

function initDatas() {

    var param = new Object();
    param.stationIds = stationId1 + ',' + stationId2;
    param.time = calenderParam;

    var map = new Map();
    AJAX_GET('/stationSumPredict', param, function callback(data) {
        if (data.code > 0) {
            map = data.result.objMap;

            initHightcharts(map);
        }
    });
}

function initHightcharts(data) {
    //10分钟 线形图
    $('#container1').highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: data[stationId1].stationName + ' ' + data[stationId2].stationName + ' 累计10分钟客流预测'
        },
        subtitle: {
            text: calender
        },
        credits: {
            enabled: false  //去掉右下角的链接
        },
        xAxis: {
            type: 'datetime',
            endOnTick: true,
            showFirstLabel: true,
            startOnTick: true,
            title: {
                text: null
            }
        },
        yAxis: {
            title: {
                text: ''
            },
            min: 0
        },
        tooltip: {
            formatter: function () {//提示框中数据样式设置
                var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
                $.each(this.points, function () {
                    var y = this.y;
                    tips += '<br/><span style="color:' + this.series.color + '">\u25CF</span>' + this.series.name + ': ' + y + '人';
                });
                return tips;
            },
            shared: true,
            crosshairs: true
        },
        plotOptions: {
            spline: {
                lineWidth: 1,
                fillOpacity: 0.1,
                marker: {
                    enabled: false,
                    states: {
                        hover: {
                            enabled: true,
                            radius: 2
                        }
                    }
                }
            }
        },
        series: [{
            color: '#90EE90',
            name: data[stationId1].stationName,
            data: data[stationId1].dataList1
        }, {
            color: '#545454',
            name: data[stationId2].stationName,
            data: data[stationId2].dataList1
        }]
    });

    //30分钟 线形图
    $('#container2').highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: data[stationId1].stationName + ' ' + data[stationId2].stationName + ' 累计30分钟客流预测'
        },
        subtitle: {
            text: calender
        },
        credits: {
            enabled: false  //去掉右下角的链接
        },
        xAxis: {
            type: 'datetime',
            endOnTick: true,
            showFirstLabel: true,
            startOnTick: true,
            title: {
                text: null
            }
        },
        yAxis: {
            title: {
                text: ''
            },
            min: 0
        },
        tooltip: {
            formatter: function () {//提示框中数据样式设置
                var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
                $.each(this.points, function () {
                    var y = this.y;
                    tips += '<br/><span style="color:' + this.series.color + '">\u25CF</span>' + this.series.name + ': ' + y + '人';
                });
                return tips;
            },
            shared: true,
            crosshairs: true
        },
        plotOptions: {
            spline: {
                lineWidth: 1,
                fillOpacity: 0.1,
                marker: {
                    enabled: false,
                    states: {
                        hover: {
                            enabled: true,
                            radius: 2
                        }
                    }
                }
            }
        },
        series: [{
            color: '#8DB6CD',
            name: data[stationId1].stationName,
            data: data[stationId1].dataList2
        }, {
            color: '#DDA0DD',
            name: data[stationId2].stationName,
            data: data[stationId2].dataList2
        }]
    });

    //60分钟 线形图 
    $('#container3').highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: data[stationId1].stationName + ' ' + data[stationId2].stationName + ' 累计60分钟客流预测'
        },
        subtitle: {
            text: calender
        },
        credits: {
            enabled: false  //去掉右下角的链接
        },
        xAxis: {
            type: 'datetime',
            endOnTick: true,
            showFirstLabel: true,
            startOnTick: true,
            title: {
                text: null
            }
        },
        yAxis: {
            title: {
                text: ''
            },
            min: 0
        },
        tooltip: {
            formatter: function () {//提示框中数据样式设置
                var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
                $.each(this.points, function () {
                    var y = this.y;
                    tips += '<br/><span style="color:' + this.series.color + '">\u25CF</span>' + this.series.name + ': ' + y + '人';
                });
                return tips;
            },
            shared: true,
            crosshairs: true
        },
        plotOptions: {
            spline: {
                lineWidth: 1,
                fillOpacity: 0.1,
                marker: {
                    enabled: false,
                    states: {
                        hover: {
                            enabled: true,
                            radius: 2
                        }
                    }
                }
            }
        },
        series: [{
            color: '#8B8682',
            name: data[stationId1].stationName,
            data: data[stationId1].dataList3
        }, {
            color: '#FF7F24',
            name: data[stationId2].stationName,
            data: data[stationId2].dataList3
        }]
    });
}