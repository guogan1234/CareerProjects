package parsing;

import data.HotelManager;

/**
 * a class that parses hotel and review data
 */
public class DataParser extends ADataParser {
    private HotelParser hotelParser;
    private ReviewParser reviewParser;

    /**
     * constructor
     * @param hotelsPath hotel file path
     * @param reviewsPath reviews directory path
     */
    public DataParser(String hotelsPath, String reviewsPath){
        super(hotelsPath, reviewsPath);
    }

    /**
     * parse hotel data
     * @param path path of a json file that contains hotel information
     */
    @Override
    public void parseHotelsData(String path, HotelManager hotelManager) {
        hotelParser = new HotelParser();
        hotelParser.parseHotelData(path, hotelManager);
    }

    /**
     * parse review data
     * @param path path of a specific directory that contains reviews files
     */
    @Override
    public void parseReviewsData(String path, HotelManager hotelManager) {
        reviewParser = new ReviewParser();
//        reviewParser.loadReviewData(path, hotelManager);
        reviewParser.loadReviewDataMultiThread(path, hotelManager, getThreadNum());
    }
}
