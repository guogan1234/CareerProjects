$(function () {
    initListDetail();
});

function initListDetail() {
    var location = 'line';
    var id;

    var lineId = localStorage.getItem("rp-lineId");
    var lineName = localStorage.getItem("rp-lineName");
    if (lineId == null || lineName == null) {
        lineId = getLines()[0].value;
        lineName = getLines()[0].title;
    }
    var stationId = localStorage.getItem('rp-stationId');
    var stationName = localStorage.getItem('rp-stationName');

    if (stationId == null || stationId == 0) {
        id = lineId;
        stationName = '';
    } else {
        location = 'station';
        id = stationId;
    }

    var timeSpan = localStorage.getItem("rp-timeSpan");
    if (timeSpan == null) {
        timeSpan = getTimeSpans()[3].value;   //默认选中12个月;
    }

    var param = new Object();
    param.page = CONSTANTS.PAGE[0]; //默认第一页
    param.pageSize = CONSTANTS.PAGE[1];    //默认展示10个
    param.location = location;
    param.id = id;
    param.count = timeSpan;

    AJAX_GET('deviceDown/percent', param, function callback(data) {
        if (data.code > 0) {
            var column1 = data.result.xValue;
            var column2 = data.result.yValue;
            //柱状图
            $('#container1').highcharts({
                chart: {
                    type: 'column'
                },
                title: {
                    text: lineName + ' ' + stationName + ' 设备宕机率分布图'
                },
                subtitle: {
                    text: timeSpan + '个月'
                },
                credits: {
                    enabled: false  //去掉右下角的链接
                },
                xAxis: {
                    categories: column1
                },
                yAxis: {
                    min: 0,
                    title: {
                        text: '百分比（%）'
                    }
                },
                tooltip: {
                    formatter: function () {//提示框中数据样式设置
                        var tips = '';
                        $.each(this.points, function () {
                            var y = this.y;
                            tips += this.x + ': ' + y + '%';
                        });
                        return tips;
                    },
                    shared: true,
                    crosshairs: true
                },
                plotOptions: {
                    column: {
                        borderWidth: 2
                    }
                },
                series: [{
                    name: '设备',
                    data: column2
                }]
            });
        }
    });

    AJAX_GET('deviceDown/count', param, function callback(data) {
        if (data.code > 0) {
            var column1 = data.result.xValue;
            var column2 = data.result.yValue;

            $('#container2').highcharts({
                chart: {
                    type: 'column'
                },
                title: {
                    text: lineName + ' ' + stationName + ' 设备宕机率分布图'
                },
                subtitle: {
                    text: timeSpan + "个月"
                },
                credits: {
                    enabled: false  //去掉右下角的链接
                },
                xAxis: {
                    categories: column1
                },
                yAxis: {
                    min: 0,
                    title: {
                        text: '交易次数'
                    }
                },
                tooltip: {
                    formatter: function () {//提示框中数据样式设置
                        var tips = '';
                        $.each(this.points, function () {
                            var y = this.y;
                            tips += this.x + ': ' + y + '次';
                        });
                        return tips;
                    },
                    shared: true,
                    crosshairs: true
                },
                plotOptions: {
                    column: {
                        cursor: 'pointer'
                    }
                },
                series: [{
                    name: '设备',
                    data: column2
                }]
            });
        }
    });
}




