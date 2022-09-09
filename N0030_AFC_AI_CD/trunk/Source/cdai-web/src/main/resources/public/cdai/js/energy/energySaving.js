// var timeDate;
// $(function () {
//     searchone();
// });
//
// function searchone() {
//
//     timeDate = $("#date").val();
//     initEnergyDetail();
//
// }
//
// function initEnergyDetail() {
//
//     var param = new Object();
//     param.page = CONSTANTS.PAGE[0]; //默认第一页
//     param.pageSize = CONSTANTS.PAGE[2]; //默认展示7个
//     param.time = timeDate;
//
//     AJAX_GET('/power', param, function callback(data) {
//         if (data.code > 0) {
//             newArr1 = data.result;
//         }
//
//         $('#container1').highcharts({
//             chart: {
//                 type: 'column'
//             },
//             title: {
//                 text: '供电系统能耗分析与节能'
//             },
//             subtitle: {
//                 text: '截止到' + timeDate
//             },
//             credits: {
//                 enabled: false  //去掉右下角的链接
//             },
//             exporting: {
//                 enabled: false
//             },
//             xAxis: {
//                 categories: newArr1.dateList,
//                 labels: {
//                     formatter: function () {//提示框中数据样式设置
//                         return Highcharts.dateFormat('%Y/%m/%d', this.value);
//                     }
//                 },
//                 crosshair: true
//             },
//             yAxis: {
//                 min: 0,
//                 title: {
//                     text: ''
//                 }
//             },
//             tooltip: {
//                 formatter: function () {//提示框中数据样式设置
//                     var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
//                     $.each(this.points, function () {
//                         var y = this.y;
//                         tips += '<br/>' + newArr1.modeList[this.point.index] + ': ' + y + '度';
//                     });
//                     return tips;
//                 },
//                 shared: true,
//                 crosshairs: true
//             },
//             plotOptions: {
//                 column: {
//                     borderWidth: 2,
//                     dataLabels: {
//                         enabled: true, // dataLabels设为true
//                         formatter: function () {
//                             return newArr1.modeList[this.point.index];
//                         },
//                         shared: true,
//                         crosshairs: true
//                     }
//                 }
//             },
//
//             series: [{
//                 name: '日期',
//                 data: newArr1.energyList
//             }]
//         });
//     });
//     AJAX_GET('/ventilation', param, function callback(data) {
//         if (data.code > 0) {
//             newArr2 = data.result;
//         }
//         $('#container2').highcharts({
//             chart: {
//                 type: 'column'
//             },
//             title: {
//                 text: '通风系统能耗分析与节能'
//             },
//             subtitle: {
//                 text: '截止到' + timeDate
//             },
//             credits: {
//                 enabled: false  //去掉右下角的链接
//             },
//             exporting: {
//                 enabled: false
//             },
//             xAxis: {
//                 categories: newArr2.dateList,
//                 labels: {
//                     formatter: function () {//提示框中数据样式设置
//                         return Highcharts.dateFormat('%Y/%m/%d', this.value);
//                     }
//                 },
//                 crosshair: true
//             },
//             yAxis: {
//                 min: 0,
//                 title: {
//                     text: ''
//                 }
//             },
//             tooltip: {
//                 formatter: function () {//提示框中数据样式设置
//                     var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
//                     $.each(this.points, function () {
//                         var y = this.y;
//                         tips += '<br/>' + newArr2.modeList[this.point.index] + ': ' + y + '度';
//                     });
//                     return tips;
//                 },
//                 shared: true,
//                 crosshairs: true
//             },
//             plotOptions: {
//                 column: {
//                     borderWidth: 2,
//                     dataLabels: {
//                         enabled: true, // dataLabels设为true
//                         formatter: function () {
//                             return newArr2.modeList[this.point.index];
//                         },
//                         shared: true,
//                         crosshairs: true
//                     }
//                 }
//             },
//
//             series: [{
//                 name: '日期',
//                 data: newArr2.energyList
//             }]
//         });
//     });
//     AJAX_GET('/airCondition', param, function callback(data) {
//         if (data.code > 0) {
//             newArr3 = data.result;
//         }
//         $('#container3').highcharts({
//             chart: {
//                 type: 'column'
//             },
//             title: {
//                 text: '空调系统能耗分析与节能'
//             },
//             subtitle: {
//                 text: '截止到' + timeDate
//             },
//             credits: {
//                 enabled: false  //去掉右下角的链接
//             },
//             exporting: {
//                 enabled: false
//             },
//             xAxis: {
//                 categories: newArr3.dateList,
//                 labels: {
//                     formatter: function () {//提示框中数据样式设置
//                         return Highcharts.dateFormat('%Y/%m/%d', this.value);
//                     }
//                 },
//                 crosshair: true
//             },
//             yAxis: {
//                 min: 0,
//                 title: {
//                     text: ''
//                 }
//             },
//             tooltip: {
//                 formatter: function () {//提示框中数据样式设置
//                     var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
//                     $.each(this.points, function () {
//                         var y = this.y;
//                         tips += '<br/>' + newArr3.modeList[this.point.index] + ': ' + y + '度';
//                     });
//                     return tips;
//                 },
//                 shared: true,
//                 crosshairs: true
//             },
//             plotOptions: {
//                 column: {
//                     borderWidth: 2,
//                     dataLabels: {
//                         enabled: true, // dataLabels设为true
//                         formatter: function () {
//                             return newArr3.modeList[this.point.index];
//                         },
//                         shared: true,
//                         crosshairs: true
//                     }
//                 }
//             },
//
//             series: [{
//                 name: '日期',
//                 data: newArr3.energyList
//             }]
//         });
//     });
//     AJAX_GET('/escalator', param, function callback(data) {
//         if (data.code > 0) {
//             newArr4 = data.result;
//         }
//         $('#container4').highcharts({
//             chart: {
//                 type: 'column'
//             },
//             title: {
//                 text: '电扶梯系统能耗分析与节能'
//             },
//             subtitle: {
//                 text: '截止到' + timeDate
//             },
//             credits: {
//                 enabled: false  //去掉右下角的链接
//             },
//             exporting: {
//                 enabled: false
//             },
//             xAxis: {
//                 categories: newArr4.dateList,
//                 labels: {
//                     formatter: function () {//提示框中数据样式设置
//                         return Highcharts.dateFormat('%Y/%m/%d', this.value);
//                     }
//                 },
//                 crosshair: true
//             },
//             yAxis: {
//                 min: 0,
//                 title: {
//                     text: ''
//                 }
//             },
//             tooltip: {
//                 formatter: function () {//提示框中数据样式设置
//                     var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
//                     $.each(this.points, function () {
//                         var y = this.y;
//                         tips += '<br/>' + newArr4.modeList[this.point.index] + ': ' + y + '度';
//                     });
//                     return tips;
//                 },
//                 shared: true,
//                 crosshairs: true
//             },
//             plotOptions: {
//                 column: {
//                     borderWidth: 2,
//                     dataLabels: {
//                         enabled: true, // dataLabels设为true
//                         formatter: function () {
//                             return newArr4.modeList[this.point.index];
//                         },
//                         shared: true,
//                         crosshairs: true
//                     }
//                 }
//             },
//
//             series: [{
//                 name: '日期',
//                 data: newArr4.energyList
//             }]
//         });
//     });
//     AJAX_GET('/lighting', param, function callback(data) {
//         if (data.code > 0) {
//             newArr5 = data.result;
//         }
//         $('#container5').highcharts({
//             chart: {
//                 type: 'column'
//             },
//             title: {
//                 text: '照明系统能耗分析与节能'
//             },
//             subtitle: {
//                 text: '截止到' + timeDate
//             },
//             credits: {
//                 enabled: false  //去掉右下角的链接
//             },
//             exporting: {
//                 enabled: false
//             },
//             xAxis: {
//                 categories: newArr5.dateList,
//                 labels: {
//                     formatter: function () {//提示框中数据样式设置
//                         return Highcharts.dateFormat('%Y/%m/%d', this.value);
//                     }
//                 },
//                 crosshair: true
//             },
//             yAxis: {
//                 min: 0,
//                 title: {
//                     text: ''
//                 }
//             },
//             tooltip: {
//                 formatter: function () {//提示框中数据样式设置
//                     var tips = '<b>' + Highcharts.dateFormat('%Y-%m-%d %H:%M', this.x) + '</b>';
//                     $.each(this.points, function () {
//                         var y = this.y;
//                         tips += '<br/>' + newArr5.modeList[this.point.index] + ': ' + y + '度';
//                     });
//                     return tips;
//                 },
//                 shared: true,
//                 crosshairs: true
//             },
//             plotOptions: {
//                 column: {
//                     borderWidth: 2,
//                     dataLabels: {
//                         enabled: true, // dataLabels设为true
//                         formatter: function () {
//                             return newArr5.modeList[this.point.index];
//                         },
//                         shared: true,
//                         crosshairs: true
//                     }
//                 }
//             },
//             series: [{
//                 name: '日期',
//                 data: newArr5.energyList
//             }]
//         });
//     });
// }
//
//
