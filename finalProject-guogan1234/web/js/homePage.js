var hotelReviewsPage = 0;
var hotelReviewsPageSize = 10;

var hotelNames = document.getElementsByName("hotelName");
for(var i = 0; i < hotelNames.length; i++){
    hotelNames[i].onclick = function(){
        document.getElementById("showHotelPage").click();
        var hotelName = this.innerHTML;
        showHotelPage(hotelName);
    }
}

// show hotel info by hotel name
function showHotelPage(data){
    var login = validateLogin();
    if(login == false) return;
//    alert(data);
    var postData = "hotelName=" + data;
    ajaxPost("/hotelInfo", postData, hotelPageDataReady);
}

// handle hotel info response json
function hotelPageDataReady(data){
//    alert(data);
    var hotel = JSON.parse(data);
    if(hotel == null) return;
    document.getElementById("pageHotelId").innerHTML = hotel.id;
    document.getElementById("pageHotelName").innerHTML = hotel.f;
    document.getElementById("pageHotelAddress").innerHTML = hotel.ad;
    document.getElementById("saveHotel").className = "btn btn-link";

    queryHotelReviews(hotel.id);
}

// query reviews by hotel id
function queryHotelReviews(hotelId){
    var startIndex = hotelReviewsPage * hotelReviewsPageSize;
    var keys = ["hotelId", "start", "size"];
    var values = [hotelId, startIndex, hotelReviewsPageSize];
    var postData = createPostParametersData(keys, values);
    ajaxPost("/hotelReviews", postData, hotelPageReviewsReady);
}

// handle reviews response json
function hotelPageReviewsReady(data){
//    alert(data);
    var reviews = JSON.parse(data);
//    alert(reviews.length);
    if(reviews.length == 0 && hotelReviewsPage == 0) return;
    if(reviews.length == 0){
        hotelReviewsPage == 0 ? hotelReviewsPage = 0 : hotelReviewsPage--;
        alert("You have reached the last page!");
        return;
    }

    var pageHotelName = document.getElementById("pageHotelName").innerHTML;
    var hotelPageTable = document.getElementById("hotelPageTable");
    var temp = document.getElementById("hotelPageTableBody");
    if(temp != null) hotelPageTable.removeChild(temp);
    var hotelPageTableBody = document.createElement("tbody");
    hotelPageTableBody.id = "hotelPageTableBody";
    for(var i = 0; i < reviews.length; i++){
        var elem = reviews[i];
        var tr = document.createElement("tr");
        var reviewId = document.createElement("td");
        reviewId.innerHTML = elem.reviewId;
        reviewId.hidden = "hidden";
        tr.appendChild(reviewId);
        var hotelName = document.createElement("td");
        hotelName.innerHTML = pageHotelName;
        tr.appendChild(hotelName);
        var reviewTitle = document.createElement("td");
        reviewTitle.innerHTML = elem.title;
        tr.appendChild(reviewTitle);
        var reviewText = document.createElement("td");
        reviewText.innerHTML = elem.reviewText;
        tr.appendChild(reviewText);
        var user = document.createElement("td");
        user.innerHTML = elem.userNickname;
        tr.appendChild(user);
        var date = document.createElement("td");
        date.innerHTML = elem.reviewSubmissionTime;
        tr.appendChild(date);

        var operations = createReviewsOperations(elem.userNickname, elem.reviewId);
        tr.appendChild(operations);

        hotelPageTableBody.appendChild(tr);
    }
    hotelPageTable.appendChild(hotelPageTableBody);
}

// create review table user operations
function createReviewsOperations(username, reviewId){
        var operations = document.createElement("td");
        var edit = document.createElement("button");
        edit.innerHTML = "Edit";
        edit.setAttribute("data-toggle", "modal");
        edit.setAttribute("data-target", "#reviewModal");
        edit.setAttribute("reviewId", reviewId);
//        edit.onclick = editReview;
        edit.onclick = function(){
            editReview(reviewId);
        }
        var del = document.createElement("button");
        del.innerHTML = "Delete";
        var user = document.getElementById("loginUser").innerHTML;
        if(username == user){
            edit.className = "btn btn-primary";
            del.className = "btn btn-primary";
        }else{
            edit.className = "btn btn-primary disabled";
            del.className = "btn btn-primary disabled";
        }
        del.onclick = function(){
            deleteReview(reviewId);
        }
        operations.appendChild(edit);
        operations.appendChild(del);
        var like = document.createElement("button");
        like.className = "btn btn-primary";
        like.innerHTML = "Like ";
        likeButtonShouldDisable(like, reviewId);
        var badge = document.createElement("span");
        badge.className = "badge";
        badge.innerHTML = 0;
        like.onclick = function(){
            like.name = "liking";
            badge.id = "likedBadge";
            likeReview(reviewId);
        }
        getBadgeCount(badge, reviewId);
        like.appendChild(badge);
        operations.appendChild(like);
        return operations;
}

// check like button should disable by review id and username
function likeButtonShouldDisable(like, reviewId){
    var keys = ["user", "reviewId"];
    var user = document.getElementById("loginUser").innerHTML;
    var values = [user, reviewId];
    var postData = createPostParametersData(keys, values);
    ajaxPostWithArgument("/checkLikedReview", postData, likeButtonShouldDisableDataReady, like);
}

// handle the response json of checking like button should disable
function likeButtonShouldDisableDataReady(data, like){
//    alert(data);
    var response = JSON.parse(data);
    if(response.liked){
        like.className = "btn btn-primary disabled";
    }
}

// get total liked user
function getBadgeCount(badge, reviewId){
    var postData = "reviewId=" + reviewId;
    ajaxPostWithArgument("/countLikedReview", postData, getBadgeCountDataReady, badge);
}

// handle badge count response json
function getBadgeCountDataReady(data, badge){
//    alert(data);
    var response = JSON.parse(data);
    badge.innerHTML = response.count;
}

// get previous hotel reviews page
document.getElementById("previousHotelReviews").onclick = previousHotelReviews;
function previousHotelReviews(){
    var login = validateLogin();
    if(login == false) return;
    if(hotelReviewsPage == 0) {
        alert("This is the first page!");
        return;
    }
    hotelReviewsPage--;
    var hotelId = document.getElementById("pageHotelId").innerHTML;
    queryHotelReviews(hotelId);
}

// get next hotel reviews page
document.getElementById("nextHotelReviews").onclick = nextHotelReviews;
function nextHotelReviews(){
    var login = validateLogin();
    if(login == false) return;
    hotelReviewsPage++;
    var hotelId = document.getElementById("pageHotelId").innerHTML;
    queryHotelReviews(hotelId);
}

// write review
document.getElementById("writeReview").onclick = writeReview;
function writeReview(){
    var login = validateLogin();
    if(login == false) {
        document.getElementById("writeReview").removeAttribute("href");
    }else{
        document.getElementById("writeReview").setAttribute("href", "#reviewModal");
        document.getElementById("reviewSave").className = "";
        var hotelId = document.getElementById("pageHotelId").innerHTML;
        document.getElementById("hotelId").innerHTML = hotelId;
    }
}

// save review
document.getElementById("reviewSave").onclick = reviewSave;
function reviewSave(){
    var index=document.getElementById("reviewRate").selectedIndex;
    var text = document.getElementById("reviewRate").options[index].text;
    var html = document.getElementById("reviewRate").options[index].innerHTML;
}

// delete review
function deleteReview(reviewId){
//    alert(reviewId);
    var postData = "reviewId=" + reviewId;
    ajaxPost("/deleteReview", postData, deleteReviewDataReady);
}

// handle delete review response json
function deleteReviewDataReady(data){
//    alert(data);
    var response = JSON.parse(data);
    if(response.success){
        // refresh table
        var hotelId = document.getElementById("pageHotelId").innerHTML;
        queryHotelReviews(hotelId);
    }
}

// edit review by review id
function editReview(reviewId){
//    alert(reviewId);
    var postData = "reviewId=" + reviewId;
    ajaxPost("/review", postData, editReviewDataReady);
}

// handle edit review response json
function editReviewDataReady(data){
//    alert(data);
    var review = JSON.parse(data);
    document.getElementById("reviewId").value = review.reviewId;
    document.getElementById("hotelId").value = review.hotelId;
    document.getElementById("reviewTitle").value = review.title;
    document.getElementById("reviewText").value = review.reviewText;
    var reviewRate = review.ratingOverall;
    document.getElementById("reviewRate").selectedIndex = reviewRate - 1;

    // hide save button & show update button
    document.getElementById("reviewSave").removeAttribute("hidden");
    document.getElementById("reviewSave").className = "btn btn-default";
    document.getElementById("reviewAdd").setAttribute("hidden", "hidden");
    document.getElementById("reviewAdd").className = "";
}

// update review
document.getElementById("reviewSave").onclick = updateReview;
function updateReview(){
    var keys = ["reviewId", "hotelId", "reviewTitle", "reviewText", "reviewRate", "user", "dateString"];
    var values = new Array();
    var reviewId = document.getElementById("reviewId").value;
    values.push(reviewId);
    var hotelId = document.getElementById("hotelId").value;
    values.push(hotelId);
    var reviewTitle = document.getElementById("reviewTitle").value;
    values.push(reviewTitle);
    var reviewText = document.getElementById("reviewText").value;
    values.push(reviewText);
    var reviewRate = document.getElementById("reviewRate").selectedIndex + 1;
    values.push(reviewRate);
    var user = document.getElementById("loginUser").innerHTML;
    values.push(user);
    var date = new Date();
    var dateString = date.toISOString();
    values.push(dateString);

    var postData = createPostParametersData(keys, values);
    ajaxPost("/updateReview", postData, updateReviewDataReady);
}

// handle update review response json
function updateReviewDataReady(data){
//    alert(data);
    var response = JSON.parse(data);
    if(response.success){
        document.getElementById("reviewClose").click();
        document.getElementById("reviewSave").setAttribute("hidden", "hidden");
        document.getElementById("reviewSave").className = "";
        document.getElementById("reviewAdd").removeAttribute("hidden");
        document.getElementById("reviewAdd").className = "btn btn-default";

        // refresh table
        var hotelId = document.getElementById("pageHotelId").innerHTML;
        queryHotelReviews(hotelId);
    }
}

// add a review
document.getElementById("reviewAdd").onclick = addReview;
function addReview(){
    var keys = ["hotelId", "reviewTitle", "reviewText", "reviewRate", "user", "dateString", "reviewId"];
    var values = new Array();
    var hotelId = document.getElementById("hotelId").innerHTML;
    values.push(hotelId);
    var reviewTitle = document.getElementById("reviewTitle").value;
    values.push(reviewTitle);
    var reviewText = document.getElementById("reviewText").value;
    values.push(reviewText);
    var reviewRate = document.getElementById("reviewRate").selectedIndex + 1;
    values.push(reviewRate);
    var user = document.getElementById("loginUser").innerHTML;
    values.push(user);
    var date = new Date();
    var dateString = date.toISOString();
    values.push(dateString);
    var reviewId = dateString + "_" + user;
    values.push(reviewId);

    var postData = createPostParametersData(keys, values);
    ajaxPost("/addReview", postData, addReviewDataReady);
}

// handle add review response json
function addReviewDataReady(data){
//    alert(data);
    var response = JSON.parse(data);
    if(response.success){
        document.getElementById("reviewClose").click();
        // refresh table
        var hotelId = document.getElementById("pageHotelId").innerHTML;
        queryHotelReviews(hotelId);
    }
}