var linesArr = getLines();
var lineIds = [];

var inOutStationArr = getInOutStation();    //进站和出站
var inOutStationId = [];


var timePeriodArr = getTimePeriods(); //时间周期
var timePeriod;


var timeDate;
var endTime;

var colors = ['#90EE90'];
var dataResults1 = [];
var dataResults2 = [];
var dataResults3 = [];

function zoom1() {
    var isOpen = $("#test1").hasClass("open");
    if (isOpen) {
        //open
        $("#test1").attr("class", "col-md-4");
        $("#test2").show();
        $("#test3").show();
    } else {
        //close
        $("#test1").attr("class", "col-md-12 open");
        $("#test2").hide();
        $("#test3").hide();
    }
    initHightcharts();
}

function zoom2() {
    var isOpen1 = $("#test2").hasClass("open");
    if (isOpen1) {
        //open
        $("#test2").attr("class", "col-md-4");
        $("#test1").show();
        $("#test3").show();
    } else {
        //close
        $("#test2").attr("class", "col-md-12 open");
        $("#test1").hide();
        $("#test3").hide();
    }
    initHightcharts();
}

function zoom3() {
    var isOpen2 = $("#test3").hasClass("open");
    if (isOpen2) {
        //open
        $("#test3").attr("class", "col-md-4");
        $("#test1").show();
        $("#test2").show();
    } else {
        //close
        $("#test3").attr("class", "col-md-12 open");
        $("#test1").hide();
        $("#test2").hide();
    }

    initHightcharts();

}

$(function () {
    $("#date").datepicker("setDate", '-7d');

    initSelectStation();

});


function initSelectStation() {

    $("#line").select2({
        data: linesArr,
        placeholder: '请选择线路',
        allowClear: true
    });
    // $("#line").val(lineId).trigger('change');   //默认选中值

    $("#inOutStation").select2({
        data: inOutStationArr,
        placeholder: '请选择进出站',
        allowClear: true
    });

    $("#timeperiod").select2({
        data: timePeriodArr,
        placeholder: '请选择时间周期',
        allowClear: true
    });
}

function search() {
    lineIds = $("#line").val();
    inOutStationId = $("#inOutStation").val();
    timeDate = $('#date').val();         //开始时间
    timePeriod = $("#timeperiod").val();      //结束时间

    if (lineIds == null || lineIds.length == 0) {

        $('#myModal').modal('show');
        $('#myModalLabel').html("请选择线路");         //模态框，弹框
        return false;

    } else if (lineIds.length > 5) {
        $('#myModal').modal('show');
        $('#myModalLabel').html("线路最多选择5条");
        return false;

    } else {
        searchLineDatas();
    }

}

function confirmModal() {
    $('#myModal').modal('hide');
}

function searchLineDatas() {

    var param = new Object();
    param.ids = lineIds.join(',');
    param.direct = inOutStationId;
    param.startTime = timeDate;
    endTime = getEndTimeByPeriod(timeDate,timePeriod,"yyyy/MM/dd");
    param.endTime = endTime;

    AJAX_GET('/lineShare', param, function callback(data) {
        if (data.code > 0) {

            var map = new Map();
            dataResults1 = [];
            dataResults2 = [];
            dataResults3 = [];
            map = data.result.objMap;
            var i = 0;
            for (var key in map) {
                if (map[key] != undefined) {
                    var mapValue = map[key];
                    var lineDataMap1 = {};
                    var lineDataMap2 = {};
                    var lineDataMap3 = {};

                    lineDataMap1.color = colors[i];
                    lineDataMap1.name = mapValue.lineName;
                    lineDataMap1.data = mapValue.dataList1;
                    dataResults1.push(lineDataMap1);

                    lineDataMap2.color = colors[i];
                    lineDataMap2.name = mapValue.lineName;
                    lineDataMap2.data = mapValue.dataList2;
                    dataResults2.push(lineDataMap2);

                    lineDataMap3.color = colors[i];
                    lineDataMap3.name = mapValue.lineName;
                    lineDataMap3.data = mapValue.dataList3;
                    dataResults3.push(lineDataMap3);
                    i++;
                }
            }

            initHightcharts();
        }
    });
}

function initHightcharts() {

    $(".buttonStyle").show();        // 切换视图按钮和highcharts图同时加载出来

    $('#container1').highcharts({
        chart: {
            type: 'spline',
            zoomType: 'x'
        },
        title: {
            text: '分时10分钟客流对比'
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
        series: dataResults1
    });

    $('#container2').highcharts({
        chart: {
            type: 'spline',
            zoomType: 'x'
        },
        title: {
            text: ' 分时30分钟客流对比'
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
        series: dataResults2
    });

    $('#container3').highcharts({
        chart: {
            type: 'spline',
            zoomType: 'x'
        },
        title: {
            text: ' 分时60分钟客流对比'
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
        series: dataResults3
    });
}