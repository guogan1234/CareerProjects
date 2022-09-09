var calender = localStorage.getItem('tcum-calender');

if (calender == null) { //首次进来时，默认选中昨天
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
    param.section = 10;   //10分钟时段
    param.time = calenderParam;
    param.direct = CONSTANTS.DIRECT[1];
    var pieDatas_inStation = [];

    AJAX_GET('/ticketSum', param, function callback(data) {
        if (data.code > 0) {
            var flowCountMap = data.result.flowCountMap;
            var objMap = data.result.objMap;
            for (var key in objMap) {
                var innerArray = [];

                if (flowCountMap[key] != undefined) {
                    var percent = flowCountMap[key][0];
                    var ticketName = objMap[key].ticketName;
                    innerArray.push(ticketName);
                    innerArray.push(percent);
                    pieDatas_inStation.push(innerArray);
                }
            }
            initInStationPie(pieDatas_inStation);
        }
    });

    param.direct = CONSTANTS.DIRECT[0];
    var pieDatas_outStation = [];
    AJAX_GET('/ticketSum', param, function callback(data) {
        if (data.code > 0) {
            var flowCountMap = data.result.flowCountMap;
            var objMap = data.result.objMap;
            for (var key in objMap) {
                var innerArray = [];
                if (flowCountMap[key] != undefined) {
                    var percent = flowCountMap[key][0];
                    var ticketName = objMap[key].ticketName;
                    innerArray.push(ticketName);
                    innerArray.push(percent);
                    pieDatas_outStation.push(innerArray);
                }
            }
            initOutStationPie(pieDatas_outStation);
        }
    });
}

function initInStationPie(data) {

    //票卡对比图
    $('#container1').highcharts({
        chart: {
            plotBackgroundColor: null,
            plotBorderWidth: null,
            plotShadow: false
        },
        title: {
            text: '票卡累计对比 进站'
        },
        subtitle: {
            text: calender
        },
        credits: {
            enabled: false  //去掉右下角的链接
        },
        tooltip: {
            headerFormat: '{series.name}<br>',
            pointFormat: '{point.name}: <b>{point.percentage:.1f}%</b>'
        },
        plotOptions: {
            pie: {
                allowPointSelect: true,
                cursor: 'pointer',
//                size:'450%',  //饼图放大
                dataLabels: {
                    enabled: false
                },
                showInLegend: true
            }
        },
        series: [{
            type: 'pie',
            name: '票卡累计占比',
            data: data
        }]
    });
}

function initOutStationPie(data) {

    //票卡累计对比图
    $('#container2').highcharts({
        chart: {
            plotBackgroundColor: null,
            plotBorderWidth: null,
            plotShadow: false
        },
        title: {
            text: '票卡累计对比 出站'
        },
        subtitle: {
            text: calender
        },
        credits: {
            enabled: false  //去掉右下角的链接
        },
        tooltip: {
            headerFormat: '{series.name}<br>',
            pointFormat: '{point.name}: <b>{point.percentage:.1f}%</b>'
        },

        plotOptions: {
            pie: {
                allowPointSelect: true,
                cursor: 'pointer',
//                size:'450%',
                dataLabels: {
                    enabled: false
                },
                showInLegend: true
            }
        },
        series: [{
            type: 'pie',
            name: '票卡累计对比',
            data: data
        }]
    });
}

