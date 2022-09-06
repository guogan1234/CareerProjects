package data;

import com.google.gson.annotations.SerializedName;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

/**
 * a class contains review information
 */
public class Review {
    private String hotelId;
    private String reviewId;
    @SerializedName(value = "ratingOverall")
    private int averageRating;
    private String title;
    private String reviewText;
    private String userNickname;
    @SerializedName(value = "reviewSubmissionTime")
    private String date;

    public Review() {
    }

    /**
     * constructor
     * @param other another Review instance
     */
    public Review(Review other){
        if(other == null) return;
        this.hotelId = other.hotelId;
        this.reviewId = other.reviewId;
        this.averageRating = other.averageRating;
        this.title = other.title;
        this.reviewText = other.reviewText;
        this.userNickname = other.userNickname;
        this.date = other.date;
    }

    public String getHotelId() {
        return hotelId;
    }

    public String getReviewId() {
        return reviewId;
    }

    public String getDate() {
        return date;
    }

    public String getReviewText() {
        return reviewText;
    }

    public String getTitle() {
        return title;
    }

    public int getAverageRating() {
        return averageRating;
    }

    /**
     * get user nick name
     * @return user nick name
     */
    public String getUserNickname() {
        if(userNickname.equals("")) userNickname = "Anonymous";
        return userNickname;
    }

    public void setHotelId(String hotelId) {
        this.hotelId = hotelId;
    }

    public void setReviewId(String reviewId) {
        this.reviewId = reviewId;
    }

    public void setAverageRating(int averageRating) {
        this.averageRating = averageRating;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public void setReviewText(String reviewText) {
        this.reviewText = reviewText;
    }

    public void setUserNickname(String userNickname) {
        this.userNickname = userNickname;
    }

    public void setDate(String date) {
        this.date = date;
    }

    /**
     * present Review class in an user-friendly format
     * @return a string with an user-friendly format
     */
    @Override
    public String toString() {
        return "***" + System.lineSeparator() +
                "hotelId:" + hotelId + System.lineSeparator() +
                "reviewId:" + reviewId + System.lineSeparator() +
                "averageRating:" + averageRating + System.lineSeparator() +
                "title:" + title + System.lineSeparator() +
                "reviewText:" + reviewText + System.lineSeparator() +
                "userNickname:" + userNickname + System.lineSeparator() +
                "date:" + date;
    }

    /**
     * create string for printing
     * @return print string
     */
    public String getPrintString(){
        DateTimeFormatter formatter = DateTimeFormatter.ISO_DATE_TIME;
        LocalDate localDate = LocalDate.parse(date, formatter);
        if(userNickname.equals("")) userNickname = "Anonymous";
        return "--------------------" + System.lineSeparator() +
                "Review by " + userNickname + " on " + localDate + System.lineSeparator() +
                "Rating: " + averageRating + System.lineSeparator() +
                "ReviewId: " + reviewId + System.lineSeparator() +
                title + System.lineSeparator() +
                reviewText + System.lineSeparator();
    }
}