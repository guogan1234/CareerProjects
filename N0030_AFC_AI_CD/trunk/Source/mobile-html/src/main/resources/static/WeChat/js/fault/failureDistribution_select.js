var linesArr = getLines();
var lineId = linesArr[0].value;
var lineName = linesArr[0].title;

var timeSpanArr = getTimeSpans();
var timeSpan = timeSpanArr[2].value;

$(function () {
    initFaults();
});

function initFaults() {

    $("#line").select({
        title: "选择线路",
        items: linesArr,
        input: lineName,
        beforeClose: function (values, titles) {
            lineId = values;
            lineName = titles;
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

function btnSubmit() {
    if (lineId == '') {
        lineId = linesArr[0].value;
        lineName = linesArr[0].title;
    }
    if (timeSpan == '') {
        timeSpan = timeSpanArr[2].vaule;
    }

    $.showLoading();
    localStorage.setItem("fd-lineId", lineId);
    localStorage.setItem("fd-lineName", lineName);
    localStorage.setItem("fd-timeSpan", timeSpan);

    window.location.href = 'skip?path=fault/failureDistribution';
}



