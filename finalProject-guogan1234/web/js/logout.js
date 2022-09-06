// logout
document.getElementById("logout").onclick = logout;
function logout(){
    var login = validateLogin();
    if(!login) return;
//    alert(loginTime);
    var date = new Date();
    var logoutTime = date.toISOString();
    var user = document.getElementById("loginUser").innerHTML;
    var keys = ["user", "loginTime", "logoutTime"];
    var values = [user, loginTime, logoutTime];
    var postData = createPostParametersData(keys, values);
    ajaxPost("/logout", postData, logoutDataReady);
}

// handle logout response json
function logoutDataReady(data){
//    alert(data);
    var response = JSON.parse(data);
    if(response.success){
        document.getElementById("loginUser").innerHTML = "";
        var loginInfo = document.getElementById("loginInfo");
        loginInfo.innerHTML = "Successful logout! See you next time!!!";
    }
}