$(function () {
    initListDetail();
});

function initListDetail() {
    var lineId = localStorage.getItem("mf-lineId");
    var lineName = localStorage.getItem("mf-lineName");
    if (lineName == null || lineName == null) {
        lineId = getLines()[0].value;
        lineName = getLines()[0].title;
    }
    var moduleId = localStorage.getItem("mf-moduleId");
    var moduleName = localStorage.getItem("mf-moduleName");
    if (moduleId == null || moduleId == '' || moduleName == null) {
        moduleId = getModules()[2].value;
        moduleName = getModules()[2].title;
    }
    var timeSpan = localStorage.getItem("mf-timeSpan");
    if (timeSpan == null) {
        timeSpan = getTimeSpans()[2].value;   //默认选中6个月
    }

    var param = new Object();
    param.page = CONSTANTS.PAGE[0];
    param.pageSize = CONSTANTS.PAGE[1];
    param.timeCount = timeSpan;
    param.lineId = lineId;
    param.moduleId = moduleId;

    AJAX_GET('/moduleFailed/percent', param, function callback(data) {
        if (data.code > 0) {
            var column1 = data.result.xValue;
            var column2 = data.result.yValue;

            //柱状图百分比
            $('#container1').highcharts({
                chart: {
                    type: 'column'
                },
                title: {
                    text: lineName + ' 模块故障分布图'
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
                        cursor: 'pointer',
                        point: {
                            events: {
                                click: function (e) {
                                    localStorage.setItem("mf-moduleId", this.x);
                                    localStorage.setItem("mf-moduleName", e.point.category);
                                    window.location.href = 'skip?path=fault/failureDistribution';
                                }
                            }
                        }
                    }
                },
                series: [{
                    name: moduleName + '标签',
                    data: column2
                }]
            });
        }
    });

    //故障数量
    AJAX_GET('/moduleFailed/count', param, function callback(data) {
        if (data.code > 0) {
            var column1 = data.result.xValue;
            var column2 = data.result.yValue;

            $('#container2').highcharts({
                chart: {
                    type: 'column'
                },
                title: {
                    text: lineName + ' 模块故障分布图'
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
                        cursor: 'pointer',
                        point: {
                            events: {
                                click: function (e) {
                                    localStorage.setItem("mf-moduleId", this.x);
                                    localStorage.setItem("mf-moduleName", e.point.category);
                                    window.location.href = 'skip?path=fault/failureDistribution';
                                }
                            }
                        }
                    }

                },
                series: [{
                    name: moduleName + '标签',
                    data: column2
                }]
            });
        }
    });
}


