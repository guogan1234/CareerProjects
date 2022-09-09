/**
 * Created by pw on 2017/11/16.
 */
var obj = new Array();
var obj2 = new Array();
var obj3 = new Array();

$(function () {
    initDatas();
});

function initDatas() {
    //AJAX
    var res_obj;
    $.ajax({url:"http://112.124.51.202:4060/stationShare?ids=119%2C114&time=2017%2F11%2F16",
        success:function (result) {
            res_obj = result;
            var obj_result;
            obj_result = res_obj.result;
            var objMap = obj_result.objMap;
            var data_114 = objMap[114];
            //var data_119 = objMap.119;
            var data_114_list1 = data_114.dataList1;
            var data_114_list2 = data_114.dataList2;
            var data_114_list3 = data_114.dataList3;

            var data_119 = objMap[119];
            var data_119_list1 = data_119.dataList1;
            var data_119_list2 = data_119.dataList2;
            var data_119_list3 = data_119.dataList3;

            var data = new Array();
            data.push(data_114_list1);
            data.push(data_114_list2);
            data.push(data_114_list3);

            //
            var obj_sub1 = new Object();
            obj_sub1.name = "龙江站";
            obj_sub1.data = data_114_list3;

            var obj_sub2 = new Object();
            obj_sub2.name = "草场门";
            obj_sub2.data = data_119_list3;

            var obj_sub3 = new Object();
            obj_sub3.name = "龙江站";
            obj_sub3.data = data_114_list2;

            var obj_sub4 = new Object();
            obj_sub4.name = "草场门";
            obj_sub4.data = data_119_list2;

            var obj_sub5 = new Object();
            obj_sub5.name = "龙江站";
            obj_sub5.data = data_114_list1;

            var obj_sub6 = new Object();
            obj_sub6.name = "草场门";
            obj_sub6.data = data_119_list1;

            obj.push(obj_sub1);
            obj.push(obj_sub2);
            obj2.push(obj_sub3);
            obj2.push(obj_sub4);
            obj3.push(obj_sub5);
            obj3.push(obj_sub6);

            var v = "haha";

            initCharts1(obj);
            initCharts2(obj2);
            initCharts3(obj3);
        }
    });

}

function initCharts1(data) {
    //
    $('#container1').highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: '龙江 龙江分时10分钟客流对比'
        },
        xAxis: {
            type: 'datetime',
            title: {
                text: null
            }
        },
        yAxis: {
            title: {
                text: null
            },
            min: 0
        },
        tooltip: {
            headerFormat: '<b>{series.name}</b><br>',
            pointFormat: '{series.name}: <b>{point.y}</b><br/>'
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
        series: data
    });
}

function initCharts2(data) {
    $('#container2').highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: '龙江 龙江分时30分钟客流对比'
        },
        xAxis: {
            type: 'datetime',
            title: {
                text: null
            }
        },
        yAxis: {
            title: {
                text: null
            },
            min: 0
        },
        tooltip: {
            headerFormat: '<b>{series.name}</b><br>',
            pointFormat: '{series.name}: <b>{point.y}</b><br/>'
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
        series: data
    });
}

function initCharts3(data) {
    $('#container5').highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: '龙江 龙江分时60分钟客流对比'
        },
        xAxis: {
            type: 'datetime',
            title: {
                text: null
            }
        },
        yAxis: {
            title: {
                text: null
            },
            min: 0
        },
        tooltip: {
            headerFormat: '<b>{series.name}</b><br>',
            pointFormat: '{series.name}: <b>{point.y}</b><br/>'
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
        series: data
    });
}
