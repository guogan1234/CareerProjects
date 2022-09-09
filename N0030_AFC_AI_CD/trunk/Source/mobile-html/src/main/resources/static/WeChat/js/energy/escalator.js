var calender = localStorage.getItem('esc-calender');
if (calender == null) {
    calender = new Date().pattern("yyyy-MM-dd");
}
var calenderParam = calender.replace(/-/g, '/');

$(function () {
    initDatas();
});

function initDatas() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0]; //默认第一页
    param.pageSize = CONSTANTS.PAGE[2]; //默认展示7个
    param.time = calenderParam;

    AJAX_GET('/escalator', param, function callback(data) {
        if (data.code > 0) {
            initHightcharts(data.result);
        }
    });
}

function initHightcharts(data) {

    //柱状图
    $('#container1').highcharts({
        chart: {
            type: 'column'
        },
        title: {
            text: '电扶梯系统能耗分布图'
        },
        subtitle: {
            text: '截止到' + calender,
        },
        credits: {
            enabled: false  //去掉右下角的链接
        },
        xAxis: {
            categories: data.dateList,
            labels: {
                formatter: function () {//提示框中数据样式设置
                    return Highcharts.dateFormat('%Y/%m/%d', this.value);
                }
            },
            crosshair: true
        },
        yAxis: {
            min: 0,
            title: {
                text: ''
            }
        },
        tooltip: {
            formatter: function () {//提示框中数据样式设置
                var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
                $.each(this.points, function () {
                    var y = this.y;
                    tips += '<br/>' + data.modeList[this.point.index] + ': ' + y + '度';
                });
                return tips;
            },
            shared: true,
            crosshairs: true
        },
        plotOptions: {
            column: {
                borderWidth: 2,
                dataLabels: {
                    enabled: true, // dataLabels设为true
                    formatter: function () {
                        return data.modeList[this.point.index];
                    },
                    shared: true,
                    crosshairs: true
                }
            }
        },
        series: [{
            name: '日期',
            data: data.energyList
        }]
    });
}
