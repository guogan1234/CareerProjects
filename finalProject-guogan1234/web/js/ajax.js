// ajax post function
function ajaxPost(url, data, func){
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if(xmlhttp.readyState==4 && xmlhttp.status==200){
            func(xmlhttp.responseText);
        }
    }
    xmlhttp.open("POST", url, true);
    xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
    xmlhttp.send(data);
}

// ajax post function with argument
function ajaxPostWithArgument(url, data, func, arg){
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if(xmlhttp.readyState==4 && xmlhttp.status==200){
            func(xmlhttp.responseText, arg);
        }
    }
    xmlhttp.open("POST", url, true);
    xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
    xmlhttp.send(data);
}

// ajax get function
function ajaxGet(url, func){
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if(xmlhttp.readyState==4 && xmlhttp.status==200){
            func(xmlhttp.responseText);
        }
    }
    xmlhttp.open("GET", url, true);
    xmlhttp.send();
}

// create post arguments with two array
function createPostParametersData(keys, values){
    if(keys.length != values.length){
        alert("[keys and values are not same size!]");
    }
    var payload = new String("");
    for(var i = 0; i < keys.length; i++){
        if(i == keys.length - 1){
            payload = payload + keys[i] + "=" + values[i];
            return payload;
        }
        payload = payload + keys[i] + "=" + values[i] + "&";
    }
    return payload;
}