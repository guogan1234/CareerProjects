var id;
var linesArr = getLines();
var lineId = linesArr[0].id;
var lineName = linesArr[0].text;

var stationsArr;
var stationId;
var stationName;

var equipmentArr;
var equipmentId;
var equipmentName;

var maintenanceUnitArr;    //维保单位
var maintenanceUnitId;


var maintenanceStafArr;     //维保人员
var maintenanceStafId;


$(function () {
    getStationsByLineIdOnLineIdChange(lineId);
    getEquipmentByStationIdOnChange(stationId);
    initFaultsFactorSearchForm();
    search();

});

function getStationsByLineIdOnLineIdChange(lineId) {
    stationsArr = getStationsByLineId(lineId);
    stationId = stationsArr[0].id;
    stationName = stationsArr[1].text;

    $("#station").empty().select2({
        data: stationsArr,
        placeholder: '请选择车站',
        allowClear: true
    });
    $("#station").val(stationId).trigger('change');
}

function getEquipmentByStationIdOnChange(stationId) {
    equipmentArr = getEquipmentByStationId(stationId);
    equipmentId = equipmentArr[0].id;
    equipmentName = equipmentArr[0].text;

    $("#equipment").empty().select2({
        data: equipmentArr,
        placeholder: '请选择设备',
        allowClear: true
    });
    $("#equipment").val(equipmentId).trigger('change');
}

function initFaultsFactorSearchForm() {
    $("#line").select2({
        data: linesArr,
        placeholder: '请选择线路',
        allowClear: true
    });

    $("#line").val(lineId).trigger('change');

    $("#maintenanceUnit").select2({
        data: maintenanceUnitArr,
        placeholder: '请选择维保单位',
        allowClear: true
    });

    $("#maintenanceUnit").val(maintenanceUnitId).trigger('change');

    $("#maintenanceStaf").select2({
        data: maintenanceStafArr,
        placeholder: '请选择维保人员',
        allowClear: true
    });

    $("#maintenanceStaf").val(maintenanceStafId).trigger('change');
}

function search() {
    lineId = $("#line").val();
    stationId = $("#station").val();
    equipmentId = $("#equipment").val();
    maintenanceUnitId = $("#maintenanceUnit").val();
    maintenanceStafId = $("#maintenanceStaf").val();

}

$(function () {
    $(function () {
        $('#container').highcharts({
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
            exporting: {
                enabled: false
            },
            xAxis: {
                type: 'category'
            },
            yAxis: {
                min: 0,
                title: {
                    text: ''
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
                    ['6号线', 90],
                    ['7号线', 90],
                    ['8号线', 90],
                    ['9号线', 90],
                    ['10号线', 90]

                ],
            }]
        });
    });

});