package model.manager;

import data.WordIndex;
import model.WordIndexModel;
import utils.EntityHandler;
import utils.JdbcTemplate;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

/**
 * A class manages database operations about word index
 */
public class WordIndexManager implements WordIndexModel {
    private static JdbcTemplate jdbcTemplate = new JdbcTemplate();

    /**
     * Constructor
     */
    public WordIndexManager(){
//        jdbcTemplate = new JdbcTemplate();
    }

    /**
     * Add word index info batched
     * @param wordIndices A WordIndex set contains word index info
     */
    @Override
    public void addBatch(Set<WordIndex> wordIndices) {
        jdbcTemplate.setAutoCommit(false);
        String sql = "insert into word_index values(?, ?, ?, ?, ?, ?, ?, ?)";
        for(WordIndex wordIndex: wordIndices){
            jdbcTemplate.addBatch(sql, wordIndex.getWord(), wordIndex.getFrequency(), wordIndex.getReviewId(), wordIndex.getHotelId(), wordIndex.getReviewTitle(), wordIndex.getReviewText(), wordIndex.getUsername(), wordIndex.getReviewDate());
        }
        jdbcTemplate.commit();
    }

    /**
     * Find word index info by word
     * @param word word
     * @param start start index
     * @param size fetch size
     * @return word index info with specific word
     */
    @Override
    public List<WordIndex> findReviewsByWord(String word, String start, String size) {
        int startInt = Integer.parseInt(start);
        int sizeInt = Integer.parseInt(size);
        String sql = "select word_index.*, hotel.name from word_index inner join hotel on word_index.hotelId = hotel.id where word = ? order by frequency desc limit ?, ?";
        return (List<WordIndex>) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                List<WordIndex> indices = new ArrayList<>();
                while (resultSet.next()){
                    WordIndex wordIndex = new WordIndex();
                    wordIndex.setWord(resultSet.getString(1));
                    wordIndex.setFrequency(resultSet.getInt(2));
                    wordIndex.setReviewId(resultSet.getString(3));
                    wordIndex.setHotelId(resultSet.getString(4));
                    wordIndex.setReviewTitle(resultSet.getString(5));
                    wordIndex.setReviewText(resultSet.getString(6));
                    wordIndex.setUsername(resultSet.getString(7));
                    wordIndex.setReviewDate(resultSet.getString(8));
                    wordIndex.setHotelName(resultSet.getString(9));
                    indices.add(wordIndex);
                }
                return indices;
            }
        }, word, startInt, sizeInt);
    }
}
