package data;

import customLock.ReentrantReadWriteLock;

import java.util.TreeSet;

/**
 * a thread safe class that manages hotels information
 */
public class ThreadSafeHotelManager extends HotelManager {
    private ReentrantReadWriteLock reentrantReadWriteLock;

    /**
     * constructor
     */
    public ThreadSafeHotelManager() {
        this.reentrantReadWriteLock = new ReentrantReadWriteLock();
    }

    /**
     * add hotel information
     * @param key hotelId
     * @param hotel hotel information
     */
    @Override
    public void addHotel(String key, Hotel hotel){
        try {
            reentrantReadWriteLock.lockWrite();
            super.addHotel(key, hotel);
        }
        finally{
            reentrantReadWriteLock.unlockWrite();
        }
    }

    /**
     * add reviews information
     * @param key hotelId
     * @param set reviews information
     */
    @Override
    public void addReview(String key, TreeSet<Review> set){
        try {
            reentrantReadWriteLock.lockWrite();
            super.addReview(key, set);
        }
        finally {
            reentrantReadWriteLock.unlockWrite();
        }
    }

    /**
     * parse inverted index
     */
    @Override
    public void parseInvertedIndex() {
        try {
            reentrantReadWriteLock.lockWrite();
            super.parseInvertedIndex();
        }
        finally {
            reentrantReadWriteLock.unlockWrite();
        }
    }

    /**
     * find hotel information
     * @param key hotelId
     */
    @Override
    public void findHotel(String key) {
        try {
            reentrantReadWriteLock.lockRead();
            super.findHotel(key);
        }finally {
            reentrantReadWriteLock.unlockRead();
        }
    }

    /**
     * get hotel information
     * @param key hotelId
     * @return json string
     */
    @Override
    public String getHotelInfo(String key) {
        try{
            reentrantReadWriteLock.lockRead();
            return super.getHotelInfo(key);
        }finally {
            reentrantReadWriteLock.unlockRead();
        }
    }

    /**
     * find reviews information
     * @param key hotelId
     */
    @Override
    public void findReviews(String key) {
        try {
            reentrantReadWriteLock.lockRead();
            super.findReviews(key);
        }finally {
            reentrantReadWriteLock.unlockRead();
        }
    }

    /**
     * get reviews information
     * @param key hotelId
     * @param num reviews num
     * @return json string
     */
    @Override
    public String getReviewsInfo(String key, String num) {
        try {
            reentrantReadWriteLock.lockRead();
            return super.getReviewsInfo(key, num);
        }finally {
            reentrantReadWriteLock.unlockRead();
        }
    }

    /**
     * find specific word information
     * @param key specific word
     */
    @Override
    public void findWord(String key) {
        try {
            reentrantReadWriteLock.lockRead();
            super.findWord(key);
        }finally {
            reentrantReadWriteLock.unlockRead();
        }
    }

    /**
     * get specific word information
     * @param key specific word
     * @param num reviews num
     * @return json string
     */
    @Override
    public String getWordInfo(String key, String num) {
        try {
            reentrantReadWriteLock.lockRead();
            return super.getWordInfo(key, num);
        }finally {
            reentrantReadWriteLock.unlockRead();
        }
    }
}
