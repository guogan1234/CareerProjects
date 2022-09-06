var loginTime;
document.body.onload = indexLoad;
// page load function
function indexLoad(){
    var user = document.getElementById("loginUser").innerHTML;
//    alert(user);
    if(user != ""){
        var url = "/loginInfo?user=" + user;
        ajaxGet(url, loginInfoDataReady);
    }
    displayHotels("San Francisco");
}

// display hotels info by city
function displayHotels(city){
    document.getElementById("city").value = city;
    var keys = ["city", "keyword", "start", "size"];
    var values = [city, "", hotelPage, hotelPageSize];
    var postData = createPostParametersData(keys, values);
    ajaxPost("/hotel", postData, searchHotelDataReady);
}

// login
document.getElementById("login").onclick = function(){
    var user = document.getElementById("user").value;
    var password = document.getElementById("password").value;
    if(user == "" || password == ""){
        alert("User or password is empty!");
        return;
    }
    var postData = "name=" + user + "&password=" + password;
    ajaxPost("/login", postData, loginResponseReady);
}

document.getElementById("register").onclick = registerUser;

// handle login response json
function loginResponseReady(data){
//    alert(data);
    var response = JSON.parse(data);
    var flag = response.success;
    document.getElementById("loginMessage").innerHTML = response.message;
    if(flag){
        document.getElementById("loginClose").click();
        document.getElementById("loginUser").innerHTML = response.name;

        var date = new Date();
        loginTime = date.toISOString();
        var url = "/loginInfo?user=" + response.name;
        ajaxGet(url, loginInfoDataReady);
    }
}

// get user last time login info
function loginInfoDataReady(data){
//    alert(data);
    var response = JSON.parse(data);
    var loginInfo = document.getElementById("loginInfo");
    if(response == null){
        loginInfo.innerHTML = "Welcome! This is the first time you login.";
    }else{
        loginInfo.innerHTML = "Welcome! Last Login:" + response.loginTime;
//        var date = new Date(response.loginTime);
//        loginInfo.innerHTML = "Welcome! Last Login:" + date.toString();
    }
}

// handle register response json
function registerResponseReady(data){
//    alert(data);
    var response = JSON.parse(data);
    var flag = response.success;
    document.getElementById("registerMessage").innerHTML = response.message;
    if(flag){
        document.getElementById("registerClose").click();
    }
}

// register user info
function registerUser(){
    var user = document.getElementById("registerUser").value;
    var password = document.getElementById("registerPassword").value;
    if(user == "" || password == ""){
        alert("User or password is empty!");
        return;
    }
    var validate = validatePassword(password);
    if(!validate) return;
    var postData = "name=" + user + "&password=" + password;
    ajaxPost("/register", postData, registerResponseReady);
}

// validate user input password
function validatePassword(password){
    if(password.length < 5 || password.length > 10){
        var message = "The length of password should be between 5 to 10!";
        document.getElementById("registerMessage").innerHTML = message;
        return false;
    }
    var numPattern = new RegExp("\\d+");
    var numMatched = numPattern.test(password);
    if(!numMatched){
        var message = "Password need to contain a number!";
        document.getElementById("registerMessage").innerHTML = message;
        return false;
    }
    var letterPattern = new RegExp("[a-zA-Z]+");
    var letterMatched = letterPattern.test(password);
    if(!letterMatched){
        var message = "Password need to contain a letter!";
        document.getElementById("registerMessage").innerHTML = message;
        return false;
    }
    var specialCharacterPattern = new RegExp("[\\W]+");
    var specialCharacterMatched = specialCharacterPattern.test(password);
    if(!specialCharacterMatched){
        var message = "Password need to contain a special character!";
        document.getElementById("registerMessage").innerHTML = message;
        return false;
    }
    return true;
}

// check user is login or not
function isLogin(){
    var user = document.getElementById("loginUser").innerHTML;
//    alert("user:" + user);
    if(user == "")  return false;
    return true;
}

// validate user is login or not
function validateLogin(){
    var flag = isLogin();
    if(flag == false){
//        alert("Please login!");
        document.getElementById("showLoginModal").click();
        return false;
    }
    return true;
}

// clear login modal
document.getElementById("showLoginModal").onclick = clearLoginModal;
function clearLoginModal(){
//    alert("[clearLoginModal]");
    document.getElementById("loginMessage").innerHTML = "";
}