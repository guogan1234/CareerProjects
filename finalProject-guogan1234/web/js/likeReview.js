// like a review
function likeReview(reviewId){
//    alert(reviewId);
    var postData = "reviewId=" + reviewId;
    ajaxPost("/review", postData, likeReviewDataReady);
}

// handle the response json of liking a review
function likeReviewDataReady(data){
//    alert(data);
    var review = JSON.parse(data);
    var hotelName = document.getElementById("pageHotelName").innerHTML;
    var user = document.getElementById("loginUser").innerHTML;
    var date = new Date();
    var dateString = date.toISOString();
    var keys = ["reviewId", "reviewTitle", "hotelId", "hotelName", "user", "dateString"];
    var values = [review.reviewId, review.title, review.hotelId, hotelName, user, dateString];
    var postData = createPostParametersData(keys, values);
    ajaxPost("/addLikedReview", postData, addLikedReviewDataReady);
}

// handle add a liked review response json
function addLikedReviewDataReady(data){
//    alert(data);
    var response = JSON.parse(data);
    if(response.success){
        var likeButtons = document.getElementsByName("liking");
        for(var i = 0; i < likeButtons.length; i++){
            likeButtons[i].className = "btn btn-primary disabled";
        }
        // refresh badge & remove badge id
        var badge = document.getElementById("likedBadge");
        badge.innerHTML = Number(badge.innerHTML) + 1;
        badge.removeAttribute("id");
    }
}