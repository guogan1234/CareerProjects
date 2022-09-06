package model.manager;

import data.SavedHotel;
import model.SaveHotelModel;
import utils.EntityHandler;
import utils.JdbcTemplate;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * A class manages database operations about saved hotel
 */
public class SaveHotelManager implements SaveHotelModel {
    private static JdbcTemplate jdbcTemplate = new JdbcTemplate();

    /**
     * Constructor
     */
    public SaveHotelManager(){
//        jdbcTemplate = new JdbcTemplate();
    }

    /**
     * Add a saved hotel
     * @param savedHotel A SavedHotel class contains saved hotel info
     * @return The operated row count
     */
    @Override
    public int add(SavedHotel savedHotel) {
        if(savedHotel == null) return -1;
        String sql = "insert into saved_hotel values(?, ?, ?, ?)";
        return jdbcTemplate.update(sql, savedHotel.getHotelId(), savedHotel.getHotelName(), savedHotel.getUsername(), savedHotel.getDate());
    }

    /**
     * Delete records by username
     * @param username username
     * @return The operated row count
     */
    @Override
    public int deleteAllByUser(String username) {
        if(username == null) return -1;
        String sql = "delete from saved_hotel where userName = ?";
        return jdbcTemplate.update(sql, username);
    }

    /**
     * Find saved hotels by username
     * @param username username
     * @return saved hotels with specific username
     */
    @Override
    public List<SavedHotel> findByUser(String username) {
        if(username == null) return null;
        String sql = "select * from saved_hotel where userName = ?";
        return (List<SavedHotel>) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                if(resultSet == null) return null;
                List<SavedHotel> savedHotels = new ArrayList<>();
                while (resultSet.next()){
                    SavedHotel savedHotel = new SavedHotel();
                    savedHotel.setHotelId(resultSet.getString(1));
                    savedHotel.setHotelName(resultSet.getString(2));
                    savedHotel.setUsername(resultSet.getString(3));
                    savedHotel.setDate(resultSet.getString(4));
                    savedHotels.add(savedHotel);
                }
                return savedHotels;
            }
        }, username);
    }
}
