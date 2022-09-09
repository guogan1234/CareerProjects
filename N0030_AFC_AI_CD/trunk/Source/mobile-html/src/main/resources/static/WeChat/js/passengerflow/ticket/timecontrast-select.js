var calender = new Date();
calender.setDate(calender.getDate() - 1);
calender = calender.pattern("yyyy-MM-dd");
var maxDate = calender;

$(function () {
    initCalender();
});

function initCalender() {
    $("#input-calender").calendar({
        value: [calender],  //默认选中昨天
        maxDate: maxDate,
        onChange: function (p, values, displayValues) {
            calender = values;
        }
    });
}

function btnSubmit() {

    $.showLoading();

    localStorage.setItem("ttim-calender", calender);

    window.location.href = 'skip?path=passengerflow/ticket/timecontrast';
}
