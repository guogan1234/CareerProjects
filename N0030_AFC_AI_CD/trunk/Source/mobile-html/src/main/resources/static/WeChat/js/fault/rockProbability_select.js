var linesArr = getLines();
var lineId = 0;
var lineName;

var stationsArr = [];
var stationId = 0;//车站 默认选择全部
var stationName;

var timeSpanArr = getTimeSpans();
var timeSpan = timeSpanArr[3].value;

$(function () {
    initFaults();
    selectStation();
});

function initFaults() {
    linesArr.splice(0, {value: 0}, {title: '全部'});
    lineId = 0;
    lineName = '全部';

    $("#line").select({
        title: "选择线路",
        items: linesArr,
        input: linesArr[0].title,
        beforeClose: function (values, titles) {
            lineId = values;
            lineName = titles;

            selectStation();
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });

    $("#time").select({
        title: "选择时间范围",
        items: timeSpanArr,
        input: timeSpanArr[2].title,
        beforeClose: function (values, titles) {
            timeSpan = values;
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });
}

function selectStation() {
    if (lineId == '') {
        lineId = linesArr[0].value;
        lineName = linesArr[0].title;
    }

    //联动    先清空再添加
    $('#station_div').empty();
    $('#station_div').html("<input class=\"weui-input\" type=\"text\" id=\"station\">");
    
    if (lineId && lineId != 0) {
        stationsArr = getStationsByLineId(lineId);
    }
    stationsArr.splice(0, {value: 0}, {title: '全部'});
    stationId = 0;
    stationName = '全部';
    $("#station").select({
        title: "选择车站",
        items: stationsArr,
        input: stationsArr[0].title,
        beforeClose: function (values, titles) {
            stationId = values;
            stationName = titles;
        },
        onChange: function (d) {
            console.log(this, d);
        },
        onClose: function (d) {
            console.log('close' + d);
        }
    });
}

function btnSubmit() {

    if (stationId == '') {
        stationId = 0;
        stationName = '全部';
    }
    if (timeSpan == '') {
        timeSpan = timeSpanArr[2].value;
    }

    $.showLoading();
    localStorage.setItem("rp-lineId", lineId);
    localStorage.setItem("rp-lineName", lineName);
    localStorage.setItem("rp-stationId", stationId);
    localStorage.setItem("rp-stationName", stationName);
    localStorage.setItem("rp-timeSpan", timeSpan);

    window.location.href = 'skip?path=fault/rockProbability';
}

