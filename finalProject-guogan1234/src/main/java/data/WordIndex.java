package data;

/**
 * a class that contains word inverted index
 */
public class WordIndex {
    private String word;
    private int frequency;
    private String reviewId;
    private String reviewDate;
    private String reviewTitle;
    private String reviewText;
    private String username;
    private String hotelId;
    private String hotelName;

    public WordIndex() {
    }

    /**
     * constructor
     * @param other another WordIndex instance
     */
    public WordIndex(WordIndex other){
        this.frequency = other.frequency;
        this.reviewId = other.reviewId;
        this.reviewDate = other.reviewDate;
        this.hotelId = other.hotelId;
        this.word = other.word;
        this.reviewTitle = other.reviewTitle;
        this.reviewText = other.reviewText;
        this.username = other.username;
    }

    public WordIndex(String word, int frequency, String reviewId, String reviewDate, String hotelId, String reviewTitle, String reviewText, String username){
        this.frequency = frequency;
        this.reviewId = reviewId;
        this.reviewDate = reviewDate;
        this.hotelId = hotelId;
        this.word = word;
        this.reviewTitle = reviewTitle;
        this.reviewText = reviewText;
        this.username = username;
    }

    public int getFrequency() {
        return frequency;
    }

    public String getHotelId() {
        return hotelId;
    }

    public String getReviewId() {
        return reviewId;
    }

    public String getReviewDate() {
        return reviewDate;
    }

    public String getWord() {
        return word;
    }

    public String getReviewTitle() {
        return reviewTitle;
    }

    public String getReviewText() {
        return reviewText;
    }

    public String getUsername() {
        return username;
    }

    public void setHotelName(String hotelName) {
        this.hotelName = hotelName;
    }

    public void setWord(String word) {
        this.word = word;
    }

    public void setFrequency(int frequency) {
        this.frequency = frequency;
    }

    public void setReviewId(String reviewId) {
        this.reviewId = reviewId;
    }

    public void setReviewDate(String reviewDate) {
        this.reviewDate = reviewDate;
    }

    public void setReviewTitle(String reviewTitle) {
        this.reviewTitle = reviewTitle;
    }

    public void setReviewText(String reviewText) {
        this.reviewText = reviewText;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public void setHotelId(String hotelId) {
        this.hotelId = hotelId;
    }
}
