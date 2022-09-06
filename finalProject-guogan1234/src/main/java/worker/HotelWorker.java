package worker;

import com.google.gson.Gson;
import data.Hotel;
import model.HotelModel;
import model.ReviewModel;
import model.manager.HotelDataManager;
import model.manager.ReviewManager;

import java.util.List;

/**
 * A worker handles hotel info
 */
public class HotelWorker {
    public static String getHotelsJsonString(List<Hotel> hotels){
        if(hotels == null) return null;
        Gson gson = new Gson();
        String json = gson.toJson(hotels);
//        System.out.println(json);
        return json;
    }

    /**
     * Hotel converts to String
     * @param hotel hotel
     * @return a json string
     */
    public static String getHotelJsonString(Hotel hotel){
        if(hotel == null) return null;
        Gson gson = new Gson();
        return gson.toJson(hotel);
    }

    /**
     * Get hotels info json string
     * @param city city name
     * @param keyword keyword
     * @param start start index
     * @param size fetch size
     * @return a json string
     */
    public String handleRequest(String city, String keyword, String start, String size){
        HotelModel model = new HotelDataManager();
        List<Hotel> hotels = model.findHotelByCityAndKeyword(city, keyword, start, size);
        if(hotels == null) return null;
        ReviewModel reviewModel = new ReviewManager();
        for(Hotel hotel: hotels){
            if(hotel == null) continue;
            double averageRate = -1.0;
            averageRate = reviewModel.calculateRateByHotelId(hotel.getId());
            hotel.setAverageRate(averageRate);
        }
        return HotelWorker.getHotelsJsonString(hotels);
    }
}
