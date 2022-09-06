package model.manager;

import data.Hotel;
import data.Location;
import model.HotelModel;
import utils.EntityHandler;
import utils.JdbcTemplate;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * A class manages database operations about hotel
 */
public class HotelDataManager implements HotelModel {
    private static JdbcTemplate jdbcTemplate = new JdbcTemplate();

    /**
     * Constructor
     */
    public HotelDataManager(){
//        jdbcTemplate = new JdbcTemplate();
    }

    /**
     * Add a hotel
     * @param hotel A Hotel class contains hotel info
     * @return The operated row count
     */
    @Override
    public int add(Hotel hotel) {
        return 0;
    }

    /**
     * Add hotels info batched
     * @param hotels A Hotel array contains hotels info
     */
    @Override
    public void addBatch(Hotel[] hotels) {
        jdbcTemplate.setAutoCommit(false);
        String sql = "insert into hotel values(?, ?, ?, ?, ?, ?, ?)";
        for(Hotel hotel: hotels){
            Location location = hotel.getLocation();
            jdbcTemplate.addBatch(sql, hotel.getId(), hotel.getName(), hotel.getAddress(), hotel.getCity(), hotel.getState(), location.getLatitude(), location.getLongitude());
        }
        jdbcTemplate.commit();
    }

    /**
     * Delete all records of review
     */
    @Override
    public void deleteAll() {
        //SET SQL_SAFE_UPDATES = 0;
        //delete from usf_20fall_final_project_db.review;
    }

    /**
     * Find hotels by city and keyword
     * @param city city name
     * @param keyword keyword
     * @param start start index
     * @param size fetch size
     * @return hotels with specific city and keyword
     */
    @Override
    public List<Hotel> findHotelByCityAndKeyword(String city, String keyword, String start, String size) {
        String packedKeyword = "%" + keyword + "%";
        int startInt = Integer.parseInt(start);
        int sizeInt = Integer.parseInt(size);
        String sql = "select * from hotel where city = ? and name like ? limit ?,?";
        return (List<Hotel>) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                List<Hotel> hotels = new ArrayList<>();
                while (resultSet.next()){
                    Hotel hotel = new Hotel();
                    hotel.setId(resultSet.getString(1));
                    hotel.setName(resultSet.getString(2));
                    hotel.setAddress(resultSet.getString(3));
                    hotel.setCity(resultSet.getString(4));
                    hotel.setState(resultSet.getString(5));
                    hotel.setLocation(new Location(resultSet.getString(6), resultSet.getString(7)));
                    hotels.add(hotel);
                }
                return hotels;
            }
        }, city, packedKeyword, startInt, sizeInt);
    }

    /**
     * Find hotel by name
     * @param name hotel name
     * @return hotel with specific name
     */
    @Override
    public Hotel findByName(String name) {
        String sql = "select * from hotel where name = ?";
        return (Hotel) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                if(resultSet == null) return null;
                Hotel hotel = null;
                if(resultSet.next()){
                    hotel = new Hotel();
                    hotel.setId(resultSet.getString(1));
                    hotel.setName(resultSet.getString(2));
                    hotel.setAddress(resultSet.getString(3));
                    hotel.setCity(resultSet.getString(4));
                    hotel.setState(resultSet.getString(5));
                    hotel.setLocation(new Location(resultSet.getString(6), resultSet.getString(7)));
                }
                return hotel;
            }
        }, name);
    }
}
