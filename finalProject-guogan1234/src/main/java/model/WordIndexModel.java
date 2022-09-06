package model;

import data.WordIndex;

import java.util.List;
import java.util.Set;

/**
 * An interface class to operate WordIndex with database
 */
public interface WordIndexModel {
    /**
     * Add word index info batched
     * @param wordIndices A WordIndex set contains word index info
     */
    void addBatch(Set<WordIndex> wordIndices);

    /**
     * Find word index info by word
     * @param word word
     * @param start start index
     * @param size fetch size
     * @return word index info with specific word
     */
    List<WordIndex> findReviewsByWord(String word, String start, String size);
}
