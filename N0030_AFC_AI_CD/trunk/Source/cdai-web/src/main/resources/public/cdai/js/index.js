var map;
var param ="";

$(function () {
    $("#timeScope").datepicker("setDate", new Date());
    initFaultStation();
    initPassenger();

});

function initFaultStation() {

    var param = new Object();
    param.count = CONSTANTS.PAGE[1];

    AJAX_GET('/stationFailureTop', param, function callback(data) {
        if (data.code > 0) {

            column1 = data.result.xValue;
            column2 = data.result.yValue;

        }

    });
    $('#container1').highcharts({
        chart: {
            type: 'column'
        },
        title: {
            text: '车站故障分布图'
        },
        subtitle: {
            text: ''
        },
        credits: {
            enabled: false
        },

        xAxis: {
            categories: column1,
            crosshair: true
        },
        yAxis: {
            min: 0,
            title: {
                text: '车站故障数量'
            }
        },
        tooltip: {
            formatter: function () {//提示框中数据样式设置
                var tips = '';
                $.each(this.points, function () {
                    var y = this.y;
                    tips += this.x + ': ' + y;
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
            name: '车站故障数量',
            colorByPoint: true,          //每个柱子不同的颜色
            data: column2
        }]
    });
}


function initPassenger() {

    AJAX_GET('/compareByLine',param, function callback(data) {
        if (data.code > 0) {
            map = data.results;

            }
    });

    $('#container2').highcharts({
        chart: {
            type: 'areaspline',
            zoomType: 'x'
        },
        title: {
            text: '客流分时预测对比'
        },
        subtitle: {
            text: ''
        },
        credits: {
            enabled: false  //去掉右下角的链接
        },
        exporting: {
            enabled: false
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
            areaspline: {
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
        series:map

    });
}


















