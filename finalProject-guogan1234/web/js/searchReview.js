var reviewPage = 0;
var reviewPageSize = 10;
document.getElementById("searchReview").onclick = searchReview;

// search review
function searchReview(){
    var login = validateLogin();
    if(login == false) return;
    searchReviews();

    // new search page index start from 0
    reviewPage = 0;
}

// search reviews by user input
function searchReviews(){
    var reviewWord = document.getElementById("reviewWord").value;
    if(reviewWord == "") return;
    var reviewStartIndex = reviewPage * reviewPageSize;
    var keys = ["keyword", "start", "size"];
    var values = [reviewWord, reviewStartIndex, reviewPageSize];
    var postData = createPostParametersData(keys, values);
    ajaxPost("/wordIndex", postData, searchReviewDataReady);
}

// handle search reviews response json
function searchReviewDataReady(data){
//    alert(data);
    var reviews = JSON.parse(data);
//    alert(reviews.length);
    if(reviews.length == 0 && reviewPage == 0) return;
    if(reviews.length == 0) {
        reviewPage == 0 ? reviewPage = 0 : reviewPage--;
        alert("You have reached the last page!");
        return;
    }
    var reviewTable = document.getElementById("reviewTable");
    var temp = document.getElementById("reviewTableBody");
    if(temp != null) reviewTable.removeChild(temp);
    var reviewTableBody = document.createElement("tbody");
    reviewTableBody.id = "reviewTableBody";
    for(var i = 0; i < reviews.length; i++){
        var elem = reviews[i];
        var tr = document.createElement("tr");
        var hotelId = document.createElement("td");
        hotelId.innerHTML = elem.hotelId;
        tr.appendChild(hotelId);
        var hotelName = document.createElement("td");
        var hotelNameLink = document.createElement("a");
        hotelNameLink.innerHTML = elem.hotelName;
        hotelNameLink.name = "hotelName";
        hotelNameLink.onclick = function(){
            document.getElementById("showHotelPage").click();
            showHotelPage(this.innerHTML);
        }
        hotelName.appendChild(hotelNameLink);
        tr.appendChild(hotelName);
        var reviewTitle = document.createElement("td");
        reviewTitle.innerHTML = elem.reviewTitle;
        tr.appendChild(reviewTitle);
        var reviewText = document.createElement("td");
        reviewText.innerHTML = elem.reviewText;
        tr.appendChild(reviewText);
        var user = document.createElement("td");
        user.innerHTML = elem.username;
        tr.appendChild(user);
        var date = document.createElement("td");
        date.innerHTML = elem.reviewDate;
        tr.appendChild(date);

        reviewTableBody.appendChild(tr);
    }
    reviewTable.appendChild(reviewTableBody);
}

// get previous reviews info page
document.getElementById("previousReviewPage").onclick = function(){
    var login = validateLogin();
    if(login == false) return;
    if(reviewPage == 0) {
        alert("This is the first page!");
        return;
    }
    reviewPage--;
    searchReviews();
}

// get next reviews info page
document.getElementById("nextReviewPage").onclick = function(){
    var login = validateLogin();
    if(login == false) return;
    reviewPage++;
    searchReviews();
}