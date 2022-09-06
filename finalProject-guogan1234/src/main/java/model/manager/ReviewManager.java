package model.manager;

import data.Review;
import model.ReviewModel;
import utils.EntityHandler;
import utils.JdbcTemplate;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * A class manages database operations about review
 */
public class ReviewManager implements ReviewModel {
    private static JdbcTemplate jdbcTemplate = new JdbcTemplate();

    /**
     * Constructor
     */
    public ReviewManager(){
//        jdbcTemplate = new JdbcTemplate();
    }

    /**
     * Add reviews info batched
     * @param reviews A review array contains reviews info
     */
    @Override
    public void addBatch(Review[] reviews) {
        jdbcTemplate.setAutoCommit(false);
        String sql = "insert into review values(?, ?, ?, ?, ?, ?, ?)";
        for(Review review: reviews){
            jdbcTemplate.addBatch(sql, review.getReviewId(), review.getHotelId(), review.getAverageRating(), review.getTitle(), review.getReviewText(), review.getUserNickname(), review.getDate());
        }
        jdbcTemplate.commit();
    }

    /**
     * Find reviews by hotelId
     * @param id hotelId
     * @param start start index
     * @param size fetch size
     * @return reviews with specific hotelId
     */
    @Override
    public List<Review> findByIdOrderByDateDesc(String id, String start, String size) {
        int startInt = Integer.parseInt(start);
        int sizeInt = Integer.parseInt(size);
        String sql = "select * from review where hotelId = ? order by dateString desc limit ?, ?";
        return (List<Review>) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                List<Review> reviews = new ArrayList<>();
                while (resultSet.next()){
                    Review review = new Review();
                    review.setReviewId(resultSet.getString(1));
                    review.setHotelId(resultSet.getString(2));
                    review.setAverageRating(resultSet.getInt(3));
                    review.setTitle(resultSet.getString(4));
                    review.setReviewText(resultSet.getString(5));
                    review.setUserNickname(resultSet.getString(6));
                    review.setDate(resultSet.getString(7));
                    reviews.add(review);
                }
                return reviews;
            }
        }, id, startInt, sizeInt);
    }

    /**
     * Add a review
     * @param review A Review class contains review info
     * @return The operated row count
     */
    @Override
    public int add(Review review) {
        if(review == null) return -1;
        String sql = "insert into review values(?, ?, ?, ?, ?, ?, ?)";
        return jdbcTemplate.update(sql, review.getReviewId(), review.getHotelId(), review.getAverageRating(), review.getTitle(), review.getReviewText(), review.getUserNickname(), review.getDate());
    }

    /**
     * Delete review by reviewId
     * @param id reviewId
     * @return The operated row count
     */
    @Override
    public int deleteById(String id) {
        String sql = "delete from review where reviewId = ?";
        return jdbcTemplate.update(sql, id);
    }

    /**
     * Update review by reviewId
     * @param review reviewId
     * @return The operated row count
     */
    @Override
    public int update(Review review) {
        if(review == null) return -1;
        String sql = "update review set rating = ?, title = ?, reviewText = ?, dateString = ? where reviewId = ?";
        return jdbcTemplate.update(sql, review.getAverageRating(), review.getTitle(), review.getReviewText(), review.getDate(), review.getReviewId());
    }

    /**
     * Find review by reviewId
     * @param reviewId reviewId
     * @return review with reviewId
     */
    @Override
    public Review findById(String reviewId) {
        if(reviewId == null) return null;
        String sql = "select * from review where reviewId = ?";
        return (Review) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                if(resultSet == null) return null;
                Review review = null;
                if(resultSet.next()){
                    review = new Review();
                    review.setReviewId(resultSet.getString(1));
                    review.setHotelId(resultSet.getString(2));
                    review.setAverageRating(resultSet.getInt(3));
                    review.setTitle(resultSet.getString(4));
                    review.setReviewText(resultSet.getString(5));
                    review.setUserNickname(resultSet.getString(6));
                    review.setDate(resultSet.getString(7));
                }
                return review;
            }
        }, reviewId);
    }

    /**
     * Calculate average rate by hotelId
     * @param hotelId hotelId
     * @return average rate
     */
    @Override
    public double calculateRateByHotelId(String hotelId) {
        if(hotelId == null) return -1;
        String sql = "select avg(rating) as averageRate from review where hotelId = ?";
        return (double) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                double averageRate = -1;
                if(resultSet.next()){
                    averageRate = resultSet.getDouble(1);
                }
                return averageRate;
            }
        }, hotelId);
    }
}
