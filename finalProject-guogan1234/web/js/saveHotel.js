// save hotel
document.getElementById("saveHotel").onclick = saveHotel;
function saveHotel(){
    var keys = ["hotelId", "hotelName", "user", "dateString"];
    var hotelId = document.getElementById("pageHotelId").innerHTML;
    var hotelName = document.getElementById("pageHotelName").innerHTML;
    var user = document.getElementById("loginUser").innerHTML;
    var date = new Date();
    var dateString = date.toISOString();
    var values = [hotelId, hotelName, user, dateString];
    var postData = createPostParametersData(keys, values);
    ajaxPost("/saveHotel", postData, saveHotelDataReady);
}

// handle save hotel response json
function saveHotelDataReady(data){
//    alert(data);
    var response = JSON.parse(data);
    if(response.success){
        document.getElementById("saveHotel").className = "btn btn-link disabled";
    }
}

// show user profile
document.getElementById("loginUser").onclick = showProfile;
function showProfile(){
    showSavedHotels();
    showVisitedLinks();
}

// show user saved hotels
function showSavedHotels(){
    var user = document.getElementById("loginUser").innerHTML;
    var postData = "user=" + user;
    ajaxPost("/fetchSavedHotels", postData, savedHotelsDataReady);
}

// handle the response json of showing user saved hotels
function savedHotelsDataReady(data){
//    alert(data);
    removeSavedHotels();
    var response = JSON.parse(data);
    displaySavedHotels(response);
}

// display saved hotels in user profile
function displaySavedHotels(hotelList){
    var savedHotels = document.getElementById("savedHotels");
    for(var i = 0; i < hotelList.length; i++){
        var ele = hotelList[i];
        var a = document.createElement("a");
        a.innerHTML = ele.hotelName;
        a.className = "list-group-item";
        savedHotels.appendChild(a);
    }
}

// clear user saved hotels
document.getElementById("clearSavedHotels").onclick = clearSavedHotels;
function clearSavedHotels(){
    var user = document.getElementById("loginUser").innerHTML;
    var postData = "user=" + user;
    ajaxPost("/clearSavedHotels", postData, clearSavedHotelsDataReady);
}

// handle the response json of clearing user saved hotels
function clearSavedHotelsDataReady(data){
//    alert(data);
    var response = JSON.parse(data);
    if(response.success){
        removeSavedHotels();
    }
}

// clear user saved hotels in user profile
function removeSavedHotels(){
    var savedHotels = document.getElementById("savedHotels");
    var children = savedHotels.children;
//    alert(children.length);
    var length = children.length;
    for(var i = 0; i < length; i++){
        savedHotels.removeChild(children[0]);
    }
}