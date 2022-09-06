package worker;

import com.google.gson.Gson;
import data.Review;
import java.util.List;

/**
 * A class that handles review info
 */
public class ReviewWorker {
    public static String getReviewsJsonString(List<Review> reviews){
        if(reviews == null) return null;
        Gson gson = new Gson();
        String json = gson.toJson(reviews);
        return json;
    }

    /**
     * Review converts to String
     * @param review Review
     * @return a json string
     */
    public static String getReviewJsonString(Review review){
        if(review == null) return null;
        Gson gson = new Gson();
        String json = gson.toJson(review);
        return json;
    }
}
