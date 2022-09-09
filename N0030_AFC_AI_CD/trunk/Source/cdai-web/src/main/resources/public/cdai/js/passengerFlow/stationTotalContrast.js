var stationsArr = getStations();
var stationIds = [];

var inOutStationArr = getInOutStation();
var inOutStationId = [];

var timePeriodArr = getTimePeriods();
var timePeriod;

var timeDate;
var endTime;

var colors = ['#90EE90', '#ee5fc0', '#9a74cd', '#ff813d', '#6ea7ff', '#ffbf30'];
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

    $("#station").select2({
        data: stationsArr,
        placeholder: '请选择车站',
        maximumSelectionSize: 6,
        allowClear: true
    });

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
    stationIds = $("#station").val();
    inOutStationId = $("#inOutStation").val();
    timeDate = $('#date').val();
    timePeriod = $("#timeperiod").val();

    if (stationIds == null || stationIds.length == 0) {

        $('#myModal').modal('show');
        $('#myModalLabel').html("请选择站点");
        return false;

    } else if (stationIds.length > 6) {
        $('#myModal').modal('show');
        $('#myModalLabel').html("站点最多选择6条");
        return false;

    } else {
        searchStationDatas();
    }
}

function confirmModal() {
    $('#myModal').modal('hide');
}

function searchStationDatas() {
    var param = new Object();
    param.ids = stationIds.join(',');
    param.startTime = timeDate;
    param.direct = inOutStationId;
    endTime = getEndTimeByPeriod(timeDate,timePeriod,"yyyy/MM/dd");
    param.endTime = endTime;


    AJAX_GET('/stationSum', param, function callback(data) {
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
                    var stationDataMap1 = {};
                    var stationDataMap2 = {};
                    var stationDataMap3 = {};

                    stationDataMap1.color = colors[i];
                    stationDataMap1.name = mapValue.stationName;
                    stationDataMap1.data = mapValue.dataList1;
                    dataResults1.push(stationDataMap1);

                    stationDataMap2.color = colors[i];
                    stationDataMap2.name = mapValue.stationName;
                    stationDataMap2.data = mapValue.dataList2;
                    dataResults2.push(stationDataMap2);

                    stationDataMap3.color = colors[i];
                    stationDataMap3.name = mapValue.stationName;
                    stationDataMap3.data = mapValue.dataList3;
                    dataResults3.push(stationDataMap3);
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
            text: '累计10分钟客流对比'
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
            title: {
                text: null
            },
            endOnTick: true,
            showFirstLabel: true,
            startOnTick: true
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
            text: '累计30分钟客流对比'
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
            title: {
                text: null
            },
            endOnTick: true,
            showFirstLabel: true,
            startOnTick: true
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
            text: '累计60分钟客流对比'
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
            title: {
                text: null
            },
            endOnTick: true,
            showFirstLabel: true,
            startOnTick: true
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