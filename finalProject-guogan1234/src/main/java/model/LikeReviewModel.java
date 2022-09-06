package model;

import data.LikedReview;

/**
 * An interface class to operate LikedReview with database
 */
public interface LikeReviewModel {
    /**
     * Add a LikedReview
     * @param likedReview A LikedReview class contains liked review info
     * @return The operated row count
     */
    int add(LikedReview likedReview);

    /**
     * Find reviews by reviewId and user
     * @param reviewId reviewId
     * @param user user
     * @return reviews with specific reviewId and user
     */
    int findByIdAndUser(String reviewId, String user);

    /**
     * Find the count about liked reviews by reviewId
     * @param reviewId reviewId
     * @return matched liked reviews count
     */
    int findById(String reviewId);
}
