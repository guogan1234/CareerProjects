$(function () {
    initListDetail();
});

function initListDetail() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0]; //默认第一页
    param.pageSize = CONSTANTS.PAGE[1];    //默认展示10个
    param.count = 12;

    AJAX_GET('/component/consume', param, function callback(data) {
        if (data.code > 0) {
            var newArr = data.results;
            var arr = [];
            for (var i = 0; i < newArr.length; i++) {
                var obj = {};
                obj.id = newArr[i].id;
                obj.name = newArr[i].name;
                obj.data = newArr[i].data;

                arr.push(obj);
            }
            initHightcharts(arr);
        }
    });
}

function initHightcharts(seriesData) {
    $('#container1').highcharts({
        chart: {
            type: 'spline'
        },

        title: {
            text: '消耗使用分析'
        },
        subtitle: {
            text: ''
        },
        credits: {
            enabled: false  //去掉右下角的链接
        },
        xAxis: {
            type: 'datetime',
            endOnTick: true,
            showFirstLabel: true,
            startOnTick: true,
            labels: {}
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
                    tips += '<br/><span style="color:' + this.series.color + '">\u25CF</span>' + this.series.name + ': ' + y + '次';
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
                    enabled: true,
                    states: {
                        hover: {
                            enabled: true,
                            radius: 2
                        }
                    }
                }
            }
        },

        series: seriesData
    });
}
