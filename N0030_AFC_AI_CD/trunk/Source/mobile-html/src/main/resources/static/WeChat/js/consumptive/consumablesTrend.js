$(function () {
    initListDetail();
});

function initListDetail() {
    var location = 'default';
    var id;

    var lineId = localStorage.getItem("ct-lineId");
    var lineName = localStorage.getItem("ct-lineName");
    if (lineId != null) {
        location = 'line';
        id = lineId;
    }
    var stationId = localStorage.getItem('ct-stationId');
    var stationName = localStorage.getItem('ct-stationName');

    if (stationId == null || stationId == 0) {
        id = lineId;
        stationName = '';
    } else {
        location = 'station';
        id = stationId;
    }
    var location = 'station';
    var id;

    var stationId = localStorage.getItem("ct-stationId");
    var stationName = localStorage.getItem("ct-stationName");

    if (stationId == null || stationName == 0) {
        location = 'station';
        id = stationId;

    }
    var equipmentId = localStorage.getItem("ct-equipmentId");
    var equipmentName = localStorage.getItem("ct-equipmentName");

    if (equipmentId == null || equipmentName == 0) {
        id = stationId;
        equipmentName = '';
    } else {
        location = 'device';
        id = equipmentId;
    }
}

maintenanceUnitId = localStorage.getItem('ct-maintenanceUnitId');
maintenanceStaffId = localStorage.getItem('ct-maintenanceStaffId');


$('#container1').highcharts({
    chart: {
        type: 'column'
    },
    title: {
        text: '耗材去向分析'
    },
    subtitle: {
        text: ''
    },
    credits: {
        enabled: false  //去掉右下角的链接
    },
    xAxis: {
        type: 'category',
        labels: {
            style: {
                fontSize: '12px',

            }
        }
    },
    yAxis: {
        min: 0,
        title: {
            text: '数量'
        }
    },
    tooltip: {
        formatter: function () {//提示框中数据样式设置
            var tips = '';
            $.each(this.points, function () {
                var y = this.y;
                tips += '<br/><span style="color:' + this.series.color + '">\u25CF</span>' + this.series.name + ': ' + y + '人';
            });
            return tips;
        },
        shared: true,
        crosshairs: true

    },

    series: [{
        data: [
            ['1号线', 125],
            ['2号线', 110],
            ['3号线', 100],
            ['4号线', 95],
            ['5号线', 90],
            ['6号线', 86]

        ],
    }]
});

