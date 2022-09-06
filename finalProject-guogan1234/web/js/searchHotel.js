var hotelPage = 0;
var hotelPageSize = 3;
document.getElementById("searchHotel").onclick = searchHotel;

// search hotel
function searchHotel(){
    var login = validateLogin();
    if(login == false) return;
    clearHotelPage();
    searchHotels();

    // new search page index start from 0
    hotelPage = 0;
}

// search hotels by user input
function searchHotels(){
    var city = document.getElementById("city").value;
    var hotelWord = document.getElementById("hotelWord").value;
    if(city == "") return;
    var hotelStartIndex = hotelPage * hotelPageSize;
    var keys = ["city", "keyword", "start", "size"];
    var values = [city, hotelWord, hotelStartIndex, hotelPageSize];
    var postData = createPostParametersData(keys, values);
    ajaxPost("/hotel", postData, searchHotelDataReady);
}

// handle search hotels response json
function searchHotelDataReady(data){
//    alert("[searchHotelDataReady]");
//    alert(data);
    var response = JSON.parse(data);
//    alert(response.length);
    if(response.length == 0 && hotelPage == 0) return;
    if(response.length == 0){
        hotelPage == 0 ? hotelPage = 0 : hotelPage--;
        alert("You have reached the last page!");
        return;
    }
//    var hotelList = document.getElementById("hotelList").children;
    for(var i = 0; i < response.length; i++){
        var ele = response[i];
        var city = ele.ci;
        var replacedCity = city.replace(/ /g, "-");
        var link = "https://www.expedia.com/" + replacedCity + "-Hotels.h" + ele.id + ".Hotel-Information";
        if(i == 0){
            document.getElementById("hotelId-01").innerHTML = ele.id;
            document.getElementById("hotelName-01").innerHTML = ele.f;
            document.getElementById("hotelRate-01").innerHTML = "rating: " + ele.averageRate;
            document.getElementById("hotelLink-01").href = link;
            document.getElementById("hotelLink-01").setAttribute("title", link);
            document.getElementById("hotelLink-01").onclick = function(){
                visitLink(link);
            }
        }else if(i == 1){
            document.getElementById("hotelId-02").innerHTML = ele.id;
            document.getElementById("hotelName-02").innerHTML = ele.f;
            document.getElementById("hotelRate-02").innerHTML = "rating: " + ele.averageRate;
            document.getElementById("hotelLink-02").href = link;
            document.getElementById("hotelLink-02").setAttribute("title", link);
            document.getElementById("hotelLink-02").onclick = function(){
                visitLink(link);
            }
        }else if(i == 2){
            document.getElementById("hotelId-03").innerHTML = ele.id;
            document.getElementById("hotelName-03").innerHTML = ele.f;
            document.getElementById("hotelRate-03").innerHTML = "rating: " + ele.averageRate;
            document.getElementById("hotelLink-03").href = link;
            document.getElementById("hotelLink-03").setAttribute("title", link);
            document.getElementById("hotelLink-03").onclick = function(){
                visitLink(link);
            }
        }
    }
}

// get previous hotels info page
document.getElementById("previousHotelPage").onclick = function(){
    var login = validateLogin();
    if(login == false) return;
    if(hotelPage == 0){
        alert("This is the first page!");
        return;
    }
    clearHotelPage();
    hotelPage--;
    searchHotels();
}

// get next hotels info page
document.getElementById("nextHotelPage").onclick = function(){
    var login = validateLogin();
    if(login == false) return;
    clearHotelPage();
    hotelPage++;
    searchHotels();
}

// clear hotels info in hotel page
function clearHotelPage(){
    document.getElementById("hotelId-01").innerHTML = "Hotel Id";
    document.getElementById("hotelName-01").innerHTML = "Name";
    document.getElementById("hotelRate-01").innerHTML = "rating";
    document.getElementById("hotelLink-01").href = "";

    document.getElementById("hotelId-02").innerHTML = "Hotel Id";
    document.getElementById("hotelName-02").innerHTML = "Name";
    document.getElementById("hotelRate-02").innerHTML = "rating";
    document.getElementById("hotelLink-02").href = "";

    document.getElementById("hotelId-03").innerHTML = "Hotel Id";
    document.getElementById("hotelName-03").innerHTML = "Name";
    document.getElementById("hotelRate-03").innerHTML = "rating";
    document.getElementById("hotelLink-03").href = "";
}