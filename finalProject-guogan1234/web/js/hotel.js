var user = document.getElementById("loginUser");
var username = user.innerHTML;
alert(username);

//document.getElementById("hotelPage").style.visibility = "hidden";
//document.getElementById("hotelPage").style.height = "0px";
//document.getElementById("searchForm").style.visibility = "visible";
//document.getElementById("searchForm").setAttribute("hidden", "hidden");
//document.getElementById("hotelPage").setAttribute("hidden", "hidden");
//document.getElementById("hotelPage").setAttribute("hidden", "");
//document.getElementById("hotelPage").removeAttribute("hidden");

var hotelPage = 0;
var hotelPageSize = 5;
document.getElementById("li_01").onclick = function(){
    showSearchHotel();
}

function showSearchHotel(){
    //changeSearchForm();
    hideSearchForm();
    createSearchHotel();
}

function createSearchHotel(){
    var main = document.getElementById("main");
    var hotelDiv = document.createElement("div");
    hotelDiv.id = "searchHotels";
    createSearchHotelHeader(hotelDiv);
    createSearchHotelBody(hotelDiv);
    main.appendChild(hotelDiv);
}

function createSearchHotelHeader(main){
    var searchDiv = document.createElement("div");
    var city = document.createElement("input");
    city.id = "city";
    city.type = "text";
    city.placeholder = "City";
    city.name = "city";
    city.className = "form-control";
    var keyword = document.createElement("input");
    keyword.id = "keyword";
    keyword.type = "text";
    keyword.placeholder = "Word";
    keyword.name = "keyword";
    keyword.className = "form-control";
    var search = document.createElement("input");
    search.type = "button";
    search.value = "Search";
    search.className = "btn btn-default";
    search.onclick = searchHotels;
    searchDiv.appendChild(city);
    searchDiv.appendChild(keyword);
    searchDiv.appendChild(search);

    main.appendChild(searchDiv);
}

function createSearchHotelBody(main){
    var hotelListDiv = document.createElement("div");
    var ul = document.createElement("ul");
    for(var i = 1; i <= 5; i++){
        var li = document.createElement("li");
        var img = document.createElement("img");
        img.src = "./images/" + i + ".jpg";
        img.width = "200";
        img.height = "150";
        li.appendChild(img);
        var para = document.createElement("p");
        para.innerHTML = "Hotel";
        li.appendChild(para);

        ul.appendChild(li);
    }
    hotelListDiv.appendChild(ul);

    var pager = document.createElement("ul");
    pager.className = "pager";
    var previous = document.createElement("li");
    var a1 = document.createElement("a");
    a1.innerHTML = "Prev";
    a1.onclick = previousPage;
    previous.appendChild(a1);
    var next = document.createElement("li");
    var a2 = document.createElement("a");
    a2.innerHTML = "Next";
    a2.onclick = nextPage;
    next.appendChild(a2);
    pager.appendChild(previous);
    pager.appendChild(next);

    main.appendChild(hotelListDiv);
    main.appendChild(pager);
}

function searchHotels(){
    alert("[searchHotels]");
    var city = document.getElementById("city").value;
    var keyword = document.getElementById("keyword").value;
    alert(city);
    alert(keyword);
    var start = 0;
    var postData = "city=" + city + "&keyword=" + keyword + "&start=" + start + "&size=" + hotelPageSize;
    ajaxPost("/hotel", postData, hotelDataReady);
}

function previousPage(){
    if(hotelPage == 0) return;
    hotelPage--;
    var city = document.getElementById("city").value;
    var keyword = document.getElementById("keyword").value;
    var start = hotelPage * hotelPageSize;
    var end = (hotelPage + 1) * hotelPageSize;
    var postData = "city=" + city + "&keyword=" + keyword + "&start=" + start + "&size=" + hotelPageSize;
    ajaxPost("/hotel", postData, hotelDataReady);
}

function nextPage(){
    hotelPage++;
    var city = document.getElementById("city").value;
    var keyword = document.getElementById("keyword").value;
    var start = hotelPage * hotelPageSize;
    var end = (hotelPage + 1) * hotelPageSize;
    var postData = "city=" + city + "&keyword=" + keyword + "&start=" + start + "&size=" + hotelPageSize;
    ajaxPost("/hotel", postData, hotelDataReady);
}

function hotelDataReady(data){
    alert("[hotelDataReady]");
    alert(data);
}

function changeSearchForm(){
    var hidden = document.getElementById("searchForm").hidden;
    alert(hidden.value);
}

function hideSearchForm(){
    document.getElementById("searchForm").hidden = "hidden";
}

function hideSearchHotels(){
    alert("[hideSearchHotels]");
    document.getElementById("searchHotels").hidden = "hidden";
}