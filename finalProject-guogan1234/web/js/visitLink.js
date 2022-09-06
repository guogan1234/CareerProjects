// visit a link
function visitLink(link){
//    alert("[visitLink]");
//    alert(link);
    var login = validateLogin();
    if(login == false) return;
    var keys = ["user", "link", "dateString"];
    var user = document.getElementById("loginUser").innerHTML;
    var date = new Date();
    var dateString = date.toISOString();
    var values = [user, link, dateString];
    var postData = createPostParametersData(keys, values);
    ajaxPost("/saveVisitedLink", postData, visitLinkDataReady);
}

// handle visit link response json
function visitLinkDataReady(data){
//    alert("[visitLinkDataReady]")
//    alert(data);
}

// clear visited links
document.getElementById("clearVisitedLinks").onclick = clearVisitedLinks;
function clearVisitedLinks(){
    var user = document.getElementById("loginUser").innerHTML;
    var postData = "user=" + user;
    ajaxPost("/clearVisitedLinks", postData, clearVisitedLinksDataReady);
}

// handle the response json of clearing visited links
function clearVisitedLinksDataReady(data){
//    alert("[clearVisitedLinksDataReady]");
//    alert(data);
    var response = JSON.parse(data);
    if(response.success){
        removeVisitedLinks();
    }
}

// remove visited links in user profile
function removeVisitedLinks(){
    var visitedLinks = document.getElementById("visitedLinks");
    var children = visitedLinks.children;
//    alert(children.length);
    var length = children.length;
    for(var i = 0; i < length; i++){
        visitedLinks.removeChild(children[0]);
    }
}

// show user visited links in user profile
function showVisitedLinks(){
    var user = document.getElementById("loginUser").innerHTML;
    var postData = "user=" + user;
    ajaxPost("/fetchVisitedLinks", postData, visitedLinksDataReady);
}

// handle the response json of showing user visited links
function visitedLinksDataReady(data){
//    alert("[visitedLinksDataReady]");
//    alert(data);
    removeVisitedLinks();
    var response = JSON.parse(data);
    displayVisitedLinks(response);
}

// display user visited links in user profile
function displayVisitedLinks(linkList){
    var visitedLinks = document.getElementById("visitedLinks");
    for(var i = 0; i < linkList.length; i++){
        var ele = linkList[i];
        var a = document.createElement("a");
        a.innerHTML = ele.link;
        a.href = ele.link;
        a.target = "_blank";
        a.className = "list-group-item";
        visitedLinks.appendChild(a);
    }
}