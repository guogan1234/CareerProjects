package data;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import model.WordIndexModel;
import model.manager.WordIndexManager;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

/**
 * a class manages hotels information
 */
public class HotelManager {
    private Map<String, Hotel> hotelMap;
    private Map<String, TreeSet<Review>> reviewMap;
    private Map<String, String> wordMap;
    private Map<String, TreeSet<WordIndex>> indexMap;
    private long indicesCount;

    /**
     * constructor
     */
    public HotelManager(){
        hotelMap = new TreeMap<>();
        reviewMap = new TreeMap<>();
        wordMap = new HashMap<>();
        indexMap = new HashMap<>();
    }

    /**
     * add hotel information
     * @param key hotelId
     * @param hotel hotel information
     */
    public void addHotel(String key, Hotel hotel){
        if(hotelMap == null || hotel == null) return;
        hotelMap.put(key, hotel);
    }

    /**
     * add reviews information
     * @param key hotelId
     * @param set reviews information
     */
    public void addReview(String key, TreeSet<Review> set){
        if(key == null || set == null) return;
        reviewMap.put(key, set);
    }

    /**
     * collects all words in entire review texts
     */
    private void collectWords(){
        for(String key: reviewMap.keySet()){
            TreeSet<Review> reviews = reviewMap.get(key);
            if(reviews == null) continue;
            System.out.println("[collect words...]");
            for(Review review: reviews){
                if(review == null) continue;
                String reviewText = review.getReviewText().toLowerCase();
                // format review text
                String text = reviewText.replaceAll("\\p{Punct}|\\d","");
                String[] contents = text.split(" ");
                if(contents == null) continue;
                for(int a = 0; a < contents.length; a++){
                    String word = contents[a];
                    if(word.equals("a") || word.equals("the") || word.equals("and") || word.equals("or")) continue;
                    if(word.equals("is") || word.equals("are") || word.equals("were")) continue;
                    boolean findWord = false;
                    for(String keyWord: wordMap.keySet()){
                        if(keyWord.equals(word)){
                            findWord = true;
                            break;
                        }
                    }
                    if(!findWord){
                        wordMap.put(word, word);
                    }
                }
            }
        }
    }

    /**
     * parse word inverted index
     */
    private void parseWordIndex(){
        for(String key: wordMap.keySet()){
//            System.out.println("Word:" + key);
            System.out.println("[Parse Word Index...]" + key);
            TreeSet<WordIndex> wordIndices = new TreeSet<>(new WordIndexComparator());
            for(String reviewKey: reviewMap.keySet()){
//                System.out.println("##:" + key + "," + reviewKey);
                TreeSet<Review> reviews = reviewMap.get(reviewKey);
                if(reviews == null) continue;
                for(Review review: reviews){
                    if(review == null) continue;
                    String reviewText = review.getReviewText().toLowerCase();
                    // format review text
                    String text = reviewText.replaceAll("\\p{Punct}|\\d","");
                    String[] contents = text.split(" ");
                    if(contents == null) continue;
                    int count = 0;
                    for(int a = 0; a < contents.length; a++){
                        if(contents[a].equals(key)) count++;
                    }
                    if(count != 0){
                        wordIndices.add(new WordIndex(key, count, review.getReviewId(), review.getDate(), review.getHotelId(), review.getTitle(), review.getReviewText(), review.getUserNickname()));
                    }
                }
            }
            int wordIndicesSize = wordIndices.size();
            indicesCount = indicesCount + wordIndicesSize;
//            System.out.println("count:" + wordIndicesSize + "," + indicesCount);
            indexMap.put(key, wordIndices);
            WordIndexModel model = new WordIndexManager();
            model.addBatch(wordIndices);
        }
    }

    /**
     * parse inverted index
     */
    public void parseInvertedIndex(){
        collectWords();
        parseWordIndex();
    }

    /**
     * find hotel information
     * @param key hotelId
     */
    public void findHotel(String key){
        if(hotelMap == null){
            System.out.println("hotelHashMap is null!");
            return;
        }
        Hotel hotel = hotelMap.get(key);
        if(hotel == null){
            System.out.println("Cannot find a hotel with id = " + key);
        }
        else {
            System.out.println(hotel);
        }
    }

    /**
     * get the city of a hotel
     * @param key hotelId
     * @return city
     */
    public String getHotelCity(String key){
        if(hotelMap == null){
            System.out.println("hotelHashMap is null!");
            return null;
        }
        Hotel hotel = hotelMap.get(key);
        if(hotel == null){
            System.out.println("Cannot find a hotel with id = " + key);
            return null;
        }
        return hotel.getCity();
    }

    /**
     * get hotel city weather
     * @param key hotelId
     * @param weather weather information
     * @return json string
     */
    public String getHotelWeather(String key, String weather){
        if(hotelMap == null){
            System.out.println("hotelHashMap is null!");
        }
        JsonObject jsonObject = new JsonObject();
        if(key == null){
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("hotelId", "invalid");
            return jsonObject.toString();
        }
        Hotel hotel = hotelMap.get(key);
        if(hotel == null){
            System.out.println("Cannot find a hotel with id = " + key);
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("hotelId", "invalid");
            return jsonObject.toString();
        }
        jsonObject.addProperty("success", true);
        jsonObject.addProperty("hotelId", hotel.getId());
        jsonObject.addProperty("name", hotel.getName());
        jsonObject.addProperty("addr", hotel.getAddress());
        jsonObject.addProperty("city", hotel.getCity());
        jsonObject.addProperty("state", hotel.getState());
        Location location = hotel.getLocation();
        if(location != null) {
            jsonObject.addProperty("lat", location.getLatitude());
            jsonObject.addProperty("lng", location.getLongitude());
        }
//        jsonObject.addProperty("weather", weather);
        JsonParser jsonParser = new JsonParser();
        JsonObject weatherObject = (JsonObject) jsonParser.parse(weather);
        jsonObject.add("weather", weatherObject);
        return jsonObject.toString();
    }

    /**
     * get hotel information
     * @param key hotelId
     * @return json string
     */
    public String getHotelInfo(String key){
        if(hotelMap == null){
            System.out.println("hotelHashMap is null!");
        }
        JsonObject jsonObject = new JsonObject();
        if(key == null){
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("hotelId", "invalid");
            return jsonObject.toString();
        }
        Hotel hotel = hotelMap.get(key);
        if(hotel == null){
            System.out.println("Cannot find a hotel with id = " + key);
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("hotelId", "invalid");
            return jsonObject.toString();
        }
        jsonObject.addProperty("success", true);
        jsonObject.addProperty("hotelId", hotel.getId());
        jsonObject.addProperty("name", hotel.getName());
        jsonObject.addProperty("addr", hotel.getAddress());
        jsonObject.addProperty("city", hotel.getCity());
        jsonObject.addProperty("state", hotel.getState());
        Location location = hotel.getLocation();
        if(location != null) {
            jsonObject.addProperty("lat", location.getLatitude());
            jsonObject.addProperty("lng", location.getLongitude());
        }
        return jsonObject.toString();
    }

    /**
     * find reviews information
     * @param key hotelId
     */
    public void findReviews(String key){
        if(key == null || reviewMap == null) return;
        TreeSet<Review> reviews = reviewMap.get(key);
        if(reviews == null) return;
        for(Review review: reviews){
            System.out.println(review);
        }
    }

    /**
     * get reviews information
     * @param key hotelId
     * @param numString reviews num
     * @return json string
     */
    public String getReviewsInfo(String key, String numString){
        if(reviewMap == null) return null;
        JsonObject jsonObject = new JsonObject();
        if(key == null || numString == null){
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("hotelId", "invalid");
            return jsonObject.toString();
        }
        TreeSet<Review> reviews = reviewMap.get(key);
        if(reviews == null) {
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("hotelId", "invalid");
            return jsonObject.toString();
        }
        jsonObject.addProperty("success", true);
        jsonObject.addProperty("hotelId", key);
        JsonArray jsonArray = new JsonArray();
        int size = reviews.size();
        int num = Integer.parseInt(numString);
        if(num >= size) num = size;
        int i = 0;
        for(Review review: reviews){
            if(i == num) break;
            if(review == null) continue;
            JsonObject jsonObject1 = new JsonObject();
            jsonObject1.addProperty("reviewId", review.getReviewId());
            jsonObject1.addProperty("title", review.getTitle());
            jsonObject1.addProperty("user", review.getUserNickname());
            jsonObject1.addProperty("reviewText", review.getReviewText());
            jsonObject1.addProperty("date", review.getDate());
//            jsonArray.add(jsonObject1.toString());
            jsonArray.add(jsonObject1);
            i++;
        }
//        jsonObject.addProperty("reviews", jsonArray.toString());
        jsonObject.add("reviews", jsonArray);
        return jsonObject.toString();
    }

    /**
     * find specific word information
     * @param key specific word
     */
    public void findWord(String key){
        if(indexMap == null){
            System.out.println("indexMap is null!");
        }
        TreeSet<WordIndex> index = indexMap.get(key);
        if(index == null) return;
        System.out.println(key + " hits " + index.size() + " reviews!");
        for(WordIndex wordIndex: index){
            if(wordIndex == null) continue;
            System.out.println("#------------------------");
            System.out.println("#frequency:" + wordIndex.getFrequency());
            String hotelId = wordIndex.getHotelId();
            String reviewId = wordIndex.getReviewId();
            if(reviewMap == null) break;
            TreeSet<Review> reviews = reviewMap.get(hotelId);
            for(Review review: reviews){
                String temp = review.getReviewId();
                if(temp == null) continue;
                if(temp.equals(reviewId)){
                    System.out.println(review);
                }
            }
        }
    }

    /**
     * get specific word information
     * @param key specific word
     * @param numString reviews num
     * @return json string
     */
    public String getWordInfo(String key, String numString){
        if(indexMap == null){
            System.out.println("indexMap is null!");
            return null;
        }
        JsonObject jsonObject = new JsonObject();
        if(key == null || numString == null){
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("word", "invalid");
            return jsonObject.toString();
        }
        TreeSet<WordIndex> index = indexMap.get(key);
        if(index == null) {
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("word", "invalid");
            return jsonObject.toString();
        }
        jsonObject.addProperty("success", true);
        jsonObject.addProperty("word", key);
        JsonArray jsonArray = new JsonArray();
        int size = index.size();
        int num = Integer.parseInt(numString);
        if(num >= size) num = size;
        int i = 0;
        for(WordIndex wordIndex: index){
            if(i == num) break;
            if(wordIndex == null) continue;
            String hotelId = wordIndex.getHotelId();
            String reviewId = wordIndex.getReviewId();
            if(reviewMap == null) break;
            TreeSet<Review> reviews = reviewMap.get(hotelId);
            if(reviews == null) break;
            for(Review review: reviews){
                if(i == num) break;
                if(review == null) continue;
                String temp = review.getReviewId();
                if(temp.equals(reviewId)) {
                    JsonObject jsonObject1 = new JsonObject();
                    jsonObject1.addProperty("reviewId", review.getReviewId());
                    jsonObject1.addProperty("title", review.getTitle());
                    jsonObject1.addProperty("user", review.getUserNickname());
                    jsonObject1.addProperty("reviewText", review.getReviewText());
                    jsonObject1.addProperty("date", review.getDate());
//                    jsonArray.add(jsonObject1.toString());
                    jsonArray.add(jsonObject1);

                    i++;
                }
            }
        }
//        jsonObject.addProperty("reviews", jsonArray.toString());
        jsonObject.add("reviews", jsonArray);
        return jsonObject.toString();
    }

    /**
     * print hotels information to file
     * @param filePath output file path
     */
    public void printHotelsToFile(String filePath){
        File file = new File(filePath);
        PrintWriter printWriter = null;
        try {
            printWriter = new PrintWriter(file);
        }catch (FileNotFoundException e){
            System.out.println(e);
            return;
        }

        if(hotelMap == null || printWriter == null) return;
        for(String hotelId: hotelMap.keySet()){
            Hotel hotel = hotelMap.get(hotelId);
            if(hotel == null) continue;
            printWriter.print(hotel.getPrintString());
        }
        printWriter.close();
    }

    /**
     * print reviews information to file
     * @param filePath output file path
     */
    public void printReviewsToFile(String filePath){
        File file = new File(filePath);
        PrintWriter printWriter = null;
        try {
            printWriter = new PrintWriter(file);
        }catch (FileNotFoundException e){
            System.out.println(e);
            return;
        }

        if(hotelMap == null || reviewMap == null || printWriter == null) return;
        for(String hotelId: hotelMap.keySet()){
            Hotel hotel = hotelMap.get(hotelId);
            if(hotel == null) continue;
            printWriter.print(hotel.getPrintString());
            TreeSet<Review> reviews = reviewMap.get(hotelId);
            if(reviews != null){
                for(Review review: reviews){
                    printWriter.print(review.getPrintString());
                }
            }
        }
        printWriter.close();
    }
}
