package model;

import data.Hotel;
import java.util.List;

/**
 * An interface class to operate Hotel with database
 */
public interface HotelModel {
    /**
     * Add a hotel
     * @param hotel A Hotel class contains hotel info
     * @return The operated row count
     */
    int add(Hotel hotel);

    /**
     * Add hotels info batched
     * @param hotels A Hotel array contains hotels info
     */
    void addBatch(Hotel[] hotels);

    /**
     * Delete all records of review
     */
    void deleteAll();

    /**
     * Find hotels by city and keyword
     * @param city city name
     * @param keyword keyword
     * @param start start index
     * @param size fetch size
     * @return hotels with specific city and keyword
     */
    List<Hotel> findHotelByCityAndKeyword(String city, String keyword, String start, String size);

    /**
     * Find hotel by name
     * @param name hotel name
     * @return hotel with specific name
     */
    Hotel findByName(String name);
}
