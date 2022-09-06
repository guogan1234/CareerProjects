package model;

import data.Review;
import java.util.List;

/**
 * An interface class to operate Review with database
 */
public interface ReviewModel {
    /**
     * Add reviews info batched
     * @param reviews A review array contains reviews info
     */
    void addBatch(Review[] reviews);

    /**
     * Find reviews by hotelId
     * @param id hotelId
     * @param start start index
     * @param size fetch size
     * @return reviews with specific hotelId
     */
    List<Review> findByIdOrderByDateDesc(String id, String start, String size);

    /**
     * Add a review
     * @param review A Review class contains review info
     * @return The operated row count
     */
    int add(Review review);

    /**
     * Delete review by reviewId
     * @param id reviewId
     * @return The operated row count
     */
    int deleteById(String id);

    /**
     * Update review by reviewId
     * @param review reviewId
     * @return The operated row count
     */
    int update(Review review);

    /**
     * Find review by reviewId
     * @param reviewId reviewId
     * @return review with reviewId
     */
    Review findById(String reviewId);

    /**
     * Calculate average rate by hotelId
     * @param hotelId hotelId
     * @return average rate
     */
    double calculateRateByHotelId(String hotelId);
}
