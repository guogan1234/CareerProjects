$(function () {
    initListDetail();
});

function initListDetail() {
    var location = 'default';
    var id;

    var lineId = localStorage.getItem("ff-lineId");
    var lineName = localStorage.getItem("ff-lineName");
    if (lineId != null) {
        location = 'line';
        id = lineId;
    }
    var stationId = localStorage.getItem('ff-stationId');
    var stationName = localStorage.getItem('ff-stationName');

    if (stationId == null || stationId == 0) {
        id = lineId;
        stationName = '';
    } else {
        location = 'station';
        id = stationId;
    }

    var stationId = localStorage.getItem("ff-stationId");
    var stationName = localStorage.getItem("ff-stationName");

    if (stationId == null || stationName == 0) {
        location = 'station';
        id = stationId;
    }

    var equipmentId = localStorage.getItem("ff-equipmentId");
    var equipmentName = localStorage.getItem("ff-equipmentName");

    if (equipmentId == null || equipmentName == 0) {
        id = stationId;
        equipmentName = '';
    } else {
        location = 'device';
        id = equipmentId;
    }

    if (lineId == null && stationId == null) {
        location = "default";
    }

    var timeId = localStorage.getItem("ff-timeId");
    if (timeId == null) {
        timeId = 12;
    }
    var param = new Object();
    param.page = CONSTANTS.PAGE[0]; //默认第一页
    param.pageSize = CONSTANTS.PAGE[3];    //默认展示10个
    param.location = location;
    param.type = 3;
    param.moduleId = 12;
    param.id = id;
    param.count = timeId;

    AJAX_GET('/failureReason/section', param, function callback(data) {
        if (data.code > 0) {
            var column1 = data.result.xValue;
            var column2 = data.result.yValue;

            //故障时长
            $('#container1').highcharts({
                chart: {
                    type: 'column'
                },
                title: {
                    text: '故障因子运行时间分布图'
                },
                subtitle: {
                    text: timeId + "个月"
                },
                credits: {
                    enabled: false  //去掉右下角的链接
                },
                xAxis: {
                    categories: column1,
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
                        text: '运行时间(m)'
                    }
                },
                tooltip: {
                    formatter: function () {//提示框中数据样式设置
                        var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
                        $.each(this.points, function () {
                            var y = this.y;
                            tips += '<br/><span style="color:' + '">' + y;
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
                    name: '标签',
                    data: column2
                }]
            });
        }
    });

    //故障交易次数
    AJAX_GET('/failureReason/count', param, function callback(data) {
        if (data.code > 0) {
            var column1 = data.result.xValue;
            var column2 = data.result.yValue;

            $('#container2').highcharts({
                chart: {
                    type: 'column'
                },
                title: {
                    text: '故障因子交易次数分布图'
                },
                subtitle: {
                    text: timeId + "个月"
                },
                credits: {
                    enabled: false  //去掉右下角的链接
                },
                xAxis: {
                    categories: column1,
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
                        text: '交易次数'
                    }
                },
                tooltip: {
                    formatter: function () {//提示框中数据样式设置
                        var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
                        $.each(this.points, function () {
                            var y = this.y;
                            tips += '<br/><span style="color:' + '">' + y;
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
                    name: '标签',
                    data: column2
                }]
            });
        }
    });
}


