/**
 * Ajax Get
 * @param url
 * @param param
 * @param callback
 * @constructor
 */
function AJAX_GET(url, param, callback) {
    $.ajax({
        type: 'GET',
        url: '/resource/' + url,
        data: param,
        dataType: 'JSON',
        async: false, //默认设置下，所有请求均为异步请求。如果需要发送同步请求，请将此选项设置为 false。注意，同步请求将锁住浏览器，用户其它操作必须等待请求完成才可以执行。  
        success: function (data) {
            if (callback != null && typeof callback == 'function') {
                callback(data);
            }
        }
    });
}

function AJAX_GET_ASYNC(url, param, callback) {
    $.ajax({
        type: 'GET',
        url: '/resource/' + url,
        data: param,
        dataType: 'JSON',
        success: function (data) {
            if (callback != null && typeof callback == 'function') {
                callback(data);
            }
        }
    });
}

function AJAX_POST(url, param, callback) {
    $.ajax({
        url: '/resource/' + url,
        method: "POST",
        dataType: "json",
        contentType: "application/json; charset=utf-8",
        data: JSON.stringify(param),
        success: function (data) {
            if (callback != null && typeof callback == 'function') {
                callback(data);
            }
        },
    });
}


function AJAX_GET_URL(url, param, callback, errorFunction) {
    if (CONSTANTS.AJAX.ENABLED) {
        $.ajax({
            type: 'GET',
            url: url,
            data: param,
            dataType: 'JSON',
            headers: {
                "Authorization": "bearer " + CONSTANTS.AJAX.TO_KEN
            },
            success: function (data) {
                if (callback != null && typeof callback == 'function') {
                    callback(data);
                }
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                if (errorFunction != null && typeof errorFunction == 'function') {
                    errorFunction(XMLHttpRequest, textStatus, errorThrown);
                }
            }
        });
    }
}

function AJAX_ASYNC_GET_URL(url, param, callback, errorFunction) {
    if (CONSTANTS.AJAX.ENABLED) {
        $.ajax({
            type: 'GET',
            async: false,
            url: url,
            data: param,
            dataType: 'JSON',
            headers: {
                "Authorization": "bearer " + CONSTANTS.AJAX.TO_KEN
            },
            success: function (data) {
                if (callback != null && typeof callback == 'function') {
                    callback(data);
                }
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                if (errorFunction != null && typeof errorFunction == 'function') {
                    errorFunction(XMLHttpRequest, textStatus, errorThrown);
                }
            }
        });
    }
}

/**
 * Delete Row By Key
 * @param url
 * @param id
 * @constructor
 */
function AJAX_DELETE(url, id) {
    if (CONSTANTS.AJAX.ENABLED) {
        $.ajax({
            url: CONSTANTS.CONF.BASE_URL + url + '/' + id,
            type: "delete",
            dataType: 'json',
            beforeSend: function (xhr, settings) {
                xhr.setRequestHeader("Authorization", "bearer " + CONSTANTS.AJAX.TO_KEN);
            }, error: function (error, type) {
                console.log(error);
            }
        });
    }
}

/**
 * Local Ajax Post
 * @param url
 * @param param
 * @param callback
 * @constructor
 */
function AJAX_POST_LOCAL(url, param, callback) {
    if (CONSTANTS.AJAX.ENABLED) {
        $.ajax({
            url: CONSTANTS.CONF.RESOURCE_LOCAL_URL + url,
            method: "POST",
            dataType: "json",
            contentType: "application/json; charset=utf-8",
            data: JSON.stringify(param),
            success: function (data) {
                if (callback != null && typeof callback == 'function') {
                    callback(data);
                }
            },
            error: function (jqXHR, textStatus, errorThrown) {

            }
        });
    }
}
