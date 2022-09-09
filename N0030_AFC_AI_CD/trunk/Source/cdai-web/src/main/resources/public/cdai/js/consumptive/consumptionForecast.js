var timeSpanArr = getTimeSpans();    //时间周期
var timeSpanId = timeSpanArr[0].id;

var startTime;
var endTime;


$(function () {
    $("#timeScope").datepicker("setDate", new Date());
    timeSpanSearch();
});

function timeSpanSearch() {

    $("#timeSpan").select2({
        data: timeSpanArr,
        placeholder: '请选择时间周期',
        allowClear: true
    });
    $("#timeSpan").val(timeSpanId).trigger('change');
}

function search() {
    startTime = $("#timeScope").val();
    timeSpanId = $("#timeSpan").val();

    concumForecastSearch();
}

function concumForecastSearch() {
    var param = new Object();
    param.page = CONSTANTS.PAGE[0]; //默认第一页
    param.pageSize = CONSTANTS.PAGE[1];    //默认展示10个
    endTime = new Date(new Date(startTime).setMonth(new Date(startTime).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTime + "/01";


    AJAX_GET('/component/predict', param, function callback(data) {
        if (data.code > 0) {
            var newArr = data.results;
            var newArr2 = [];
            for (var i = 0; i < newArr.length; i++) {
                var obj = {};
                obj.id = newArr[i].id;
                obj.name = newArr[i].name;
                obj.data = newArr[i].data;

                newArr2.push(obj);
            }
            $('#container1').highcharts({
                chart: {
                    type: 'spline'
                },
                title: {
                    text: '消耗使用预测'
                },
                subtitle: {
                    text: endTime
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
                    labels: {}
                },
                yAxis: {
                    title: {
                        text: ''
                    }
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
                series: newArr2

            });
        }
    });
}





