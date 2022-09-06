package data;

/**
 * A class that contains liked review info
 */
public class LikedReview {
    private String reviewId;
    private String reviewTitle;
    private String hotelId;
    private String hotelName;
    private String user;
    private String date;

    /**
     * constructor
     */
    public LikedReview() {
    }

    public LikedReview(String reviewId, String reviewTitle, String hotelId, String hotelName, String user, String date) {
        this.reviewId = reviewId;
        this.reviewTitle = reviewTitle;
        this.hotelId = hotelId;
        this.hotelName = hotelName;
        this.user = user;
        this.date = date;
    }

    public String getReviewId() {
        return reviewId;
    }

    public void setReviewId(String reviewId) {
        this.reviewId = reviewId;
    }

    public String getReviewTitle() {
        return reviewTitle;
    }

    public void setReviewTitle(String reviewTitle) {
        this.reviewTitle = reviewTitle;
    }

    public String getHotelId() {
        return hotelId;
    }

    public void setHotelId(String hotelId) {
        this.hotelId = hotelId;
    }

    public String getHotelName() {
        return hotelName;
    }

    public void setHotelName(String hotelName) {
        this.hotelName = hotelName;
    }

    public String getUser() {
        return user;
    }

    public void setUser(String user) {
        this.user = user;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }
}
