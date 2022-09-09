var stompClient = null;
var devTagMap = {};

$(function () {
});

function connect() {
    var socket = new SockJS('/portfolio');
    stompClient = Stomp.over(socket);
    stompClient.connect({}, function (frame) {

        stompClient.subscribe('/topic/getAlarmCount', function (message) {
            console.log('000' + message.body + '000');
        });
        stompClient.subscribe('/queue/getResponse/0', function (message) {
            console.log('111' + message.body + '111');
            receiveMessage(message.body);
        });
        stompClient.subscribe('/queue/getResponse/1', function (message) {
            console.log('222' + message.body + '222');
            receiveMessage(JSON.parse(message.body));
        });
        stompClient.subscribe('/topic/getEvent/lisa', function (message) {
            console.log('333' + message.body + '333');
        });
    });
}

function sendMessage(param) {
    // stompClient.send("/app/foo.hello", null, data);

    console.log('sendMessage:' + JSON.stringify(param));
    AJAX_POST('/statusByIds', param, function callback(data) {
        if (data.code > 0) {
            var data = data.results;

            console.log('register:' + JSON.stringify(data));
            receiveMessage(data);
        }
    });
    connect();
}

function receiveMessage(arrData) {

    for (var v = 0; v < arrData.length; v++) {
        if (arrData[v].type == 2) {  //车站
            var stationSVGMap = localStorage.getItem("stationSVGMap");
            if (stationSVGMap != null) {

                stationSVGMap = JSON.parse(stationSVGMap);
                if (stationSVGMap[arrData[v].id] != undefined) {

                    setStationState(stationSVGMap[arrData[v].id], arrData[v].state);
                }
            }
        } else if (arrData[v].type == 3) {  //设备
            var devSVGMap = localStorage.getItem("devSVGMap");
            if (devSVGMap != null) {

                devSVGMap = JSON.parse(devSVGMap);
                if (devSVGMap[arrData[v].id] != undefined) {

                    if (arrData[v].tag == 'COMSTA') {

                        devTagMap[arrData[v].id] = arrData[v].state;
                        if (arrData[v].state == 0) {
                            setDevStatus(devSVGMap[arrData[v].id], 5);
                        }
                    } else if (arrData[v].tag == 'MODE' && devTagMap[arrData[v].id] == 0) {
                        setDevStatus(devSVGMap[arrData[v].id], arrData[v].state);
                    }
                }
            }
        } else if (arrData[v].type == 4) {  //标签
            var tagSVGMap = localStorage.getItem("tagSVGMap");
            if (tagSVGMap != null) {

                tagSVGMap = JSON.parse(tagSVGMap);
                if (tagSVGMap['devid'] == arrData[v].id &&
                    tagSVGMap[arrData[v].tag] != undefined) {

                    setTagStatus(tagSVGMap[arrData[v].tag], arrData[v].state);
                }
            }
        }
    }
}

function disconnect() {
    if (stompClient != null) {
        stompClient.disconnect();
    }
    //setConnected(false);
    console.log('Disconnected');
}
