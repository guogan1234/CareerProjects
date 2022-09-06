package model;

import data.VisitedLink;

import java.util.List;

/**
 * An interface class to operate VisitedLink with database
 */
public interface VisitedLinkModel {
    /**
     * Add a visited link
     * @param visitedLink A VisitedLink class contains visited link info
     * @return The operated row count
     */
    int add(VisitedLink visitedLink);

    /**
     * Find visited links by user
     * @param user user
     * @return visited links with specific username
     */
    List<VisitedLink> findByUser(String user);

    /**
     * Delete records of visited links
     * @param user user
     * @return The operated row count
     */
    int deleteByUser(String user);
}
