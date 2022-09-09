var stationsArr = getStations();
var stationId1 = stationsArr[0].value;//车站1
var stationId2 = stationsArr[1].value;//车站2
var calender = new Date();
calender.setDate(calender.getDate() - 1);
calender = calender.pattern("yyyy-MM-dd");
var maxDate = calender;

$(function () {
    initStations();
});

/**
 * 初始化车站信息
 */
function initStations() {

    $("#station1").select({
        title: "选择车站",
        items: stationsArr,
        input: stationsArr[0].title,    //默认选中第一个
        beforeClose: function (values, titles) {
            stationId1 = values;
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });

    $("#station2").select({
        title: "选择车站2",
        items: stationsArr,
        input: stationsArr[1].title,    //默认选中第二个
        beforeClose: function (values, titles) {
            stationId2 = values;
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });

    $("#input-calender").calendar({
        maxDate:maxDate,
        value:[calender],
        onChange: function (p, values, displayValues) {
            calender = values;
        }
    });
}

function btnSubmit() {
    if (stationId1 == '') {
        stationId1 = stationsArr[0].value;
    }
    if (stationId2 == '') {
        stationId2 = stationsArr[1].value;
    }
    if (stationId1 == stationId2) {
        $.alert("请选择不同车站！", "车站累计客流对比条件筛选");
        return;
    }

    $.showLoading();

    localStorage.setItem("tot-stationId1", stationId1);
    localStorage.setItem("tot-stationId2", stationId2);
    localStorage.setItem("tot-calender", calender);

    window.location.href = 'skip?path=passengerflow/station/listDetail_total';
}
