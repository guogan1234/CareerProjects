package model;

import data.SavedHotel;

import java.util.List;

/**
 * An interface class to operate SavedHotel with database
 */
public interface SaveHotelModel {
    /**
     * Add a saved hotel
     * @param savedHotel A SavedHotel class contains saved hotel info
     * @return The operated row count
     */
    int add(SavedHotel savedHotel);

    /**
     * Delete records by username
     * @param username username
     * @return The operated row count
     */
    int deleteAllByUser(String username);

    /**
     * Find saved hotels by username
     * @param username username
     * @return saved hotels with specific username
     */
    List<SavedHotel> findByUser(String username);
}
