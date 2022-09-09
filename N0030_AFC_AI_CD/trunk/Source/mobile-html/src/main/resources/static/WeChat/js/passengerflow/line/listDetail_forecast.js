var lineId = CONSTANTS.LINEID[1];
var calender = localStorage.getItem('lfor-calender');
if (calender == null) { //首次进来时，默认选中明天
    calender = new Date().pattern("yyyy-MM-dd");
}
var calenderParam = calender.replace(/-/g, '/');

$(function () {
    initDatas();
});

function initDatas() {

    var param = new Object();
    param.lineIds = lineId;
    param.time = calenderParam;

    var map = new Map();
    AJAX_GET('/lineSharePredict', param, function callback(data) {
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
            text: data[lineId].lineName + ' 分时10分钟客流预测'
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
            name: data[lineId].lineName,
            data: data[lineId].dataList1
        }]
    });

    //30分钟 线形图
    $('#container2').highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: data[lineId].lineName + ' 分时30分钟客流预测'
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
            name: data[lineId].lineName,
            data: data[lineId].dataList2
        }]
    });

    //60分钟 线形图 
    $('#container3').highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: data[lineId].lineName + ' 分时60分钟客流预测'
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
            color: '#DDA0DD',
            name: data[lineId].lineName,
            data: data[lineId].dataList3
        }]
    });
}