package parsing;

import data.HotelManager;

/**
 * an abstract class that parses hotel information and review information
 */
public abstract class ADataParser {
    private String hotelsPath;
    private String reviewsPath;
    private int threadNum;

    /**
     * constructor
     * @param hotelsPath hotel file path
     * @param reviewsPath reviews directory path
     */
    public ADataParser(String hotelsPath, String reviewsPath){
        this.hotelsPath = hotelsPath;
        this.reviewsPath = reviewsPath;
    }

    public void setThreadNum(int threadNum) {
        this.threadNum = threadNum;
    }

    public int getThreadNum() {
        return threadNum;
    }

    /**
     * parse hotel and review data
     */
    public void parseData(HotelManager hotelManager){
        parseHotelsData(hotelsPath, hotelManager);
        parseReviewsData(reviewsPath, hotelManager);
    }

    abstract void parseHotelsData(String path, HotelManager hotelManager);
    abstract void parseReviewsData(String path, HotelManager hotelManager);
}
