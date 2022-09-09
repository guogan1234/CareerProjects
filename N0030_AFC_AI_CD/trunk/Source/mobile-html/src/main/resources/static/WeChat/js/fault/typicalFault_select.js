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
        timeSpan = timeSpanArr[2].value;
    }

    $.showLoading();
    localStorage.setItem("tf-lineId", lineId);
    localStorage.setItem("tf-lineName", lineName);
    localStorage.setItem("tf-timeSpan", timeSpan);

    window.location.href = 'skip?path=fault/typicalFault';
}

