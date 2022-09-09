var calender = localStorage.getItem('ttim-calender');

if (calender == null) {
    calender = new Date();
    calender.setDate(calender.getDate() - 1);
    calender = calender.pattern("yyyy-MM-dd");
}
var calenderParam = calender.replace(/-/g, '/');

$(function () {
    initDatas();
});

function initDatas() {

    var param = new Object();
    param.time = calenderParam;

    var map = new Map();
    AJAX_GET('/ticketShare', param, function callback(data) {
        if (data.code > 0) {
            map = data.result.objMap;

            initHightcharts(map);
        }
    });


    function initHightcharts(data) {
        for (var key in data) {
            var domStr = "<div class=listdetail'><div id='container_" + key + "' class='listdetail-div'></div> </div><hr style=\"background-color:#FFF0F5;height: 1px;border: none;\">";
            $("#ticket-div").append(domStr);

            //票卡分时对比图
            $('#container_' + key).highcharts({
                chart: {
                    type: 'spline'
                },
                title: {
                    text: data[key].ticketName
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
                    name: '10分钟',
                    color: '#7cb5ec',
                    data: data[key].dataList1
                }, {
                    name: '30分钟',
                    color: '#DDA0DD',
                    data: data[key].dataList2
                }, {
                    name: '60分钟',
                    color: '#e4d354',
                    data: data[key].dataList3
                }]
            });
        }
    }
}