/**
 * Created by gjj on 2017/5/3.
 */
define(function(require, exports, module) {
    is_token();
    var tp = require("tp.js");
    var wrapper = require("_wrapper.js");
    var eventCount = 0;
    var confirmCount = 0;
    $(function () {

        var thisURL = decodeURI(document.URL);
        var params = thisURL.split("&").splice(1,thisURL.split("&").length);
        var faultdescid = params[params.length-3].split("=")[1];
        var faultdesc = params[params.length-2].split("=")[1];
        var locationId = params[params.length-1].split("=")[1];
        var equipmentId = params[params.length-4].split("=")[1];

        var roleName = 'ROLE_ASSIGN';

        AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_USER_ADMIN + "/users/search/findByRoleName?roleName=ROLE_ASSIGN&projectId="+CONSTANTS.BASE.PROJECT_ID, {}, function (users) {
            if ('_embedded' in users) {
                var record = users._embedded.users;
                tp.bind({
                    template: 'assign-user-template',
                    element: 'assign-user-table',
                    model: record,
                    append: false
                });

                var list = document.getElementById("list");
                var header = document.getElementsByTagName("header");

                list.style.height = document.body.offsetHeight + 'px';
                list.style.display = 'block';

                // mui(list).indexedList();
            }
        });

        mui('.mui-content').on('tap', 'li', function (e) {
            eventCount++;
            if(eventCount<=1) {
                var dataUid = this.getAttribute("data-uid");
                console.error(dataUid);
                mui.confirm('确认选择该调度人？', '选择调度人', null, function (e) {
                    confirmCount++;
                    if( confirmCount <= 1 ) {
                        console.error("选择操作");
                        if (e.index == 1) {

                            var param = {
                                "typeId": 1,
                                "serialNo": wrapper.createSerialNo(),
                                "equipmentId": equipmentId,
                                "faultTypeId": null,
                                "reportEmployee": JSON.parse(sessionStorage.getItem("USER")).username,
                                "planStartTime": new Date(),
                                "planEndTime": new Date(),
                                "planFixApproachId": null,
                                "planBudget": null,
                                "actualCost": null,
                                "actualWorktime": null,
                                "reportTime": new Date(),
                                "statusId": 2,
                                "enabled": true,
                                "locationId": locationId,
                                "creationTime": new Date(),
                                "lastUpdateTime": new Date(),
                                "description": faultdesc,
                                "maintenanceModeId": 1,
                                "faultDescriptionId": faultdescid,
                                "assignEmployee": extractRowId(dataUid),
                                "projectId": CONSTANTS.BASE.PROJECT_ID
                            };
                            AJAX_POST_LOCAL("/workOrders", param, function (data) {
                                console.log("su");
                                var href = data._links.self.href;
                                var workOrderId = href.split("/")[href.split("/").length - 1];
                                var badCompontParam = {"badComponentPK": {"componentId": 1, "workOrderId": workOrderId}}
                                AJAX_POST_LOCAL("/badComponents", badCompontParam, function (data) {
                                    console.log("success");
                                    var targetUrl = "/skip?path=work/wrench&report=true";
                                    //提示
                                    layer.open({
                                        content: CONSTANTS.MASSAGE.WORK.REPAIRS_SUCCESS,
                                        skin: 'msg',
                                        time: 1.5
                                    });
                                    window.location.href = targetUrl;
                                })
                            })
                        } else {
                            confirmCount == 0;
                            eventCount == 0;
                            return false;
                        }
                    }
                });
            }
        })
    });
})

function extractRowId(row){
    var url = (typeof(row) == 'object' && '_links' in row) ? row._links.self.href : row;
    var reg = /\/(\w+)$/g;
    var _id = reg.exec(url)[1];
    return _id;
}

function handleId(str, lastindex) {
    var id = null;
    if (str != null) {
        var position_lastindex = str.lastIndexOf(lastindex);
    }
    if (position_lastindex) {
        id = str.substr(position_lastindex + 1);
    }
    return id;
}
