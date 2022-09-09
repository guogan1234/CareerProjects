var linesArr = getLines();
var lineId = linesArr[0].value;
var lineName = linesArr[0].title;

var ModulesArr = getModules();
var moduleId = ModulesArr[2].value;
var moduleName = ModulesArr[2].title;

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

    $("#module").select({
        title: "选择模块",
        items: ModulesArr,
        input: moduleName,
        beforeClose: function (values, titles) {
            moduleId = values;
            moduleName = titles;
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
        timeSpan =  timeSpanArr[2].value;
    }
    if (moduleId == '') {
        moduleId = ModulesArr[2].value;
        moduleName = ModulesArr[2].title;
    }

    $.showLoading();
    localStorage.setItem("mf-lineId", lineId);
    localStorage.setItem("mf-lineName", lineName);
    localStorage.setItem("mf-moduleId", moduleId);
    localStorage.setItem("mf-moduleName", moduleName);
    localStorage.setItem("mf-timeSpan", timeSpan);

    window.location.href = 'skip?path=fault/moduleFailure';
}

