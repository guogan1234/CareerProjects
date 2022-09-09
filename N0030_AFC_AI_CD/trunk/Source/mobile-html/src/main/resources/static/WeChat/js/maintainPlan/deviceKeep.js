$(function () {
    initDatas();
});

function initDatas() {
    initTable('table-class');
}

function initTable(id) {
    if (document.getElementsByTagName) {

        for (var v = 0; v < 4; v++) {
            var table = document.getElementsByClassName(id)[v];
            var rows = table.getElementsByTagName("tr");

            for (i = 0; i < rows.length; i++) {
                if (i % 2 == 0) {
                    rows[i].className = "evenrowcolor";
                } else {
                    rows[i].className = "oddrowcolor";
                }
            }
        }
    }
}