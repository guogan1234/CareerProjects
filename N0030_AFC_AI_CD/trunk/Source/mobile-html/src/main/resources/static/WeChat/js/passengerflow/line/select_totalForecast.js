var calender = null;
var minDate = new Date().pattern("yyyy-MM-dd");

$(function () {
    initCalender();
});

function initCalender() {
    $("#input-calender").calendar({
        minDate:minDate,
        onChange: function (p, values, displayValues) {
            calender = values;
        }
    });
}

function btnSubmit() {

    $.showLoading();

    localStorage.setItem("ltotf-calender", calender);

    window.location.href = 'skip?path=passengerflow/line/listDetail_totalForecast';
}
