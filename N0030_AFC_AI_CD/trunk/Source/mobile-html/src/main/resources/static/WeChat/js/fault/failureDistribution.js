$(function () {
    initListDetail();
});

function initListDetail() {
    var lineId = localStorage.getItem("fd-lineId");
    var lineName = localStorage.getItem("fd-lineName");

    var timeSpan = localStorage.getItem("fd-timeSpan");

    var typeName = localStorage.getItem("tf-typeName");
    var moduleId = localStorage.getItem("mf-moduleId");

    var url = "/deviceFailed/percent";
    var url2 = "/deviceFailed/count";

    var param = new Object();
    param.page = CONSTANTS.PAGE[0]; //默认第一页
    param.pageSize = CONSTANTS.PAGE[1]; //默认展示10个

    if (typeName != "" && typeName != null) {
        lineId = localStorage.getItem("tf-lineId");
        lineName = localStorage.getItem("tf-lineName");
        timeSpan = localStorage.getItem("tf-timeSpan");

        url = "/tagFailedDetail/percent";
        url2 = "/tagFailedDetail/count";

        param.tagName = typeName;
    } else if (moduleId != "" && moduleId != null) {
        lineId = localStorage.getItem("mf-lineId");
        lineName = localStorage.getItem("mf-lineName");
        timeSpan = localStorage.getItem("mf-timeSpan");

        url = "/moduleFailedDetail/percent";
        url2 = "/moduleFailedDetail/count";

        param.moduleId = moduleId;
        param.tagName = localStorage.getItem("mf-moduleName");
    }
    if (lineId == null || lineName == null) {
        lineId = getLines()[0].value;
        lineName = getLines()[0].title;
    }
    if (timeSpan == null || timeSpan == undefined) {
        timeSpan = getTimeSpans()[2].value;   //默认选中6个月
    }

    param.lineId = lineId;
    param.timeCount = timeSpan;

    AJAX_GET(url, param, function callback(data) {
        if (data.code > 0) {
            var column1 = data.result.xValue;
            var column2 = data.result.yValue;

            //柱状图
            $('#container1').highcharts({
                chart: {
                    type: 'column'
                },
                title: {
                    text: lineName + ' 设备故障分布图'
                },
                subtitle: {
                    text: timeSpan + '个月'
                },
                credits: {
                    enabled: false  //去掉右下角的链接
                },
                xAxis: {
                    categories: column1,
                    crosshair: true
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

    //故障数量 
    AJAX_GET(url2, param, function callback(data) {
        if (data.code > 0) {
            var column1 = data.result.xValue;
            var column2 = data.result.yValue;

            $('#container2').highcharts({
                chart: {
                    type: 'column'
                },
                title: {
                    text: lineName + ' 设备故障分布图'
                },
                subtitle: {
                    text: timeSpan + '个月'
                },
                credits: {
                    enabled: false  //去掉右下角的链接
                },
                xAxis: {
                    categories: column1,

                    crosshair: true
                },
                yAxis: {
                    min: 0,
                    title: {
                        text: '故障数量'
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

    localStorage.setItem("tf-typeName", "");
    localStorage.setItem("mf-moduleId", "");
}


