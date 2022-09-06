package model.manager;

import data.LikedReview;
import model.LikeReviewModel;
import utils.EntityHandler;
import utils.JdbcTemplate;

import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * A class manages database operations about liked review
 */
public class LikeReviewManager implements LikeReviewModel {
    private static JdbcTemplate jdbcTemplate = new JdbcTemplate();

    /**
     * Constructor
     */
    public LikeReviewManager(){
//        jdbcTemplate = new JdbcTemplate();
    }

    /**
     * Add a LikedReview
     * @param likedReview A LikedReview class contains liked review info
     * @return The operated row count
     */
    @Override
    public int add(LikedReview likedReview) {
        if(likedReview == null) return -1;
        String sql = "insert into liked_review values(?, ?, ?, ?, ?, ?)";
        return jdbcTemplate.update(sql, likedReview.getReviewId(), likedReview.getReviewTitle(), likedReview.getHotelId(), likedReview.getHotelName(), likedReview.getUser(), likedReview.getDate());
    }

    /**
     * Find reviews by reviewId and user
     * @param reviewId reviewId
     * @param user user
     * @return reviews with specific reviewId and user
     */
    @Override
    public int findByIdAndUser(String reviewId, String user) {
        if(reviewId == null || user == null) return -1;
        String sql = "select * from liked_review where reviewId = ? and username = ?";
        return (int) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                int count = 0;
                while (resultSet.next()) count++;
                return count;
            }
        }, reviewId, user);
    }

    /**
     * Find the count about liked reviews by reviewId
     * @param reviewId reviewId
     * @return matched liked reviews count
     */
    @Override
    public int findById(String reviewId) {
        if(reviewId == null) return -1;
        String sql = "select * from liked_review where reviewId = ?";
        return (int) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                int count = 0;
                while (resultSet.next()) count++;
                return count;
            }
        }, reviewId);
    }
}
