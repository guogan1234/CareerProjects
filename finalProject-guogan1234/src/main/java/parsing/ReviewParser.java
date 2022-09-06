package parsing;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import data.HotelManager;
import data.Review;
import data.ReviewComparator;
import model.ReviewModel;
import model.manager.ReviewManager;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * a class that parses review data
 */
public class ReviewParser {
    private ExecutorService executorService;
    private int count = 0;
    private int pendingTask = 0;
    private HotelManager safeHotelManager;
//    private static final Logger logger = LogManager.getLogger();

    /**
     * increase pending task
     */
    private synchronized void incrementTasks(){
        pendingTask++;
    }

    /**
     * decrease pending task
     */
    private synchronized void decrementTasks(){
        pendingTask--;
        if(pendingTask == 0){
            notifyAll();
        }
    }

    /**
     * combine total count
     * @param fileCount counts of json file
     */
    private synchronized void combineCount(int fileCount){
        count += fileCount;
    }

    /**
     * combine local reviews information to entire reviews information
     * @param hotelId id of the hotel that contains local reviews information
     * @param reviews local reviews information
     */
    private void combineData(String hotelId, TreeSet<Review> reviews){
        safeHotelManager.addReview(hotelId, reviews);
    }

    /**
     * wait all pending task complete
     */
    private synchronized void waitToFinish(){
        while (pendingTask != 0){
            try {
//                System.out.println("##:" + pendingTask);
                wait();
            }catch (InterruptedException e){
                System.out.println(e);
            }
        }
        if(!executorService.isShutdown()){
            executorService.shutdown();
        }
//        System.out.println("Count:" + count);
//        logger.debug("Count:" + count);
    }

    /**
     * load and parse reviews data
     * @param path specific directory path
     * @param hotelManager a class manages hotel information
     */
    public void loadReviewData(String path, HotelManager hotelManager){
        DirectoryTraverser.traverse(path, hotelManager);
        hotelManager.parseInvertedIndex();
    }

    /**
     * load reviews data with multiple threads
     * @param path a directory path contains reviews files
     * @param hotelManager hotel information manager
     * @param num thread number
     */
    public void loadReviewDataMultiThread(String path, HotelManager hotelManager, int num){
        if(path == null) return;
        safeHotelManager = hotelManager;
        if(num != 0){
            executorService = Executors.newFixedThreadPool(num);
            Path startPath = Paths.get(path);
            if(startPath == null) return;
            recursiveParseReviewFiles(startPath);

            waitToFinish();
//            hotelManager.parseInvertedIndex();
        }else {
            loadReviewData(path, hotelManager);
        }
    }

    /**
     * recursively traverse the directory that contains reviews files
     * @param path the directory that contains reviews files
     */
    private void recursiveParseReviewFiles(Path path){
        if(!Files.isDirectory(path)) return;
        try(DirectoryStream<Path> filesAndFolders = Files.newDirectoryStream(path)){
            for(Path filePath: filesAndFolders){
                if(Files.isDirectory(filePath)){
                    recursiveParseReviewFiles(filePath);
                }else {
                    if(filePath.toString().endsWith(".json")){
                        JsonFileWorker fileWorker = new JsonFileWorker(filePath);
                        incrementTasks();
                        executorService.submit(fileWorker);
                    }
                }
            }
        }catch (IOException e){
            System.out.println(e);
//            logger.debug(e);
        }
    }

    /**
     * json file worker that parses review file
     */
    class JsonFileWorker implements Runnable{
        private Path path;
        private int localCount = 0;
        private String hotelId;
        private TreeSet<Review> reviewSet;

        public JsonFileWorker(Path path) {
            this.path = path;
        }

        /**
         * parse json file
         * @param path json file path
         */
        private void parseJsonFile(Path path){
            Gson gson = new Gson();
            try {
                String fileData = new String(Files.readAllBytes(path));
                JsonParser parser = new JsonParser();
                JsonObject jsonObject = (JsonObject) parser.parse(fileData);
                JsonObject reviewDetail = jsonObject.getAsJsonObject("reviewDetails");
                JsonObject reviewCollection = reviewDetail.getAsJsonObject("reviewCollection");
                JsonArray reviewArray = reviewCollection.getAsJsonArray("review");
                Review[] reviews = gson.fromJson(reviewArray, Review[].class);
                if (reviews == null) return;
//                System.out.println("reviews length:" + reviews.length);
                if (reviews.length < 1) return;
                Review head = reviews[0];
                if (head == null) return;
                hotelId = head.getHotelId();
                reviewSet = new TreeSet<>(new ReviewComparator());
                for (Review review : reviews) {
                    reviewSet.add(review);
                }
                ReviewModel model = new ReviewManager();
                model.addBatch(reviews);
            }catch (IOException e){
                System.out.println("[parseReviews IOException]");
//                logger.debug("[parseReviews IOException]");
            }
        }

        @Override
        public void run() {
            localCount++;
            try {
                parseJsonFile(path);
            }
            finally {
                combineCount(localCount);
                combineData(hotelId, reviewSet);
                decrementTasks();
            }
        }
    }

    /**
     * parse reviews data
     * @param jsonFile specific json file path
     * @param hotelManager a class manages hotel information
     */
    public static void parseReviews(String jsonFile, HotelManager hotelManager){
        Gson gson = new Gson();

        try {
            String fileData = new String(Files.readAllBytes(Paths.get(jsonFile)));
            JsonParser parser = new JsonParser();
            JsonObject jsonObject = (JsonObject) parser.parse(fileData);
            JsonObject reviewDetail = jsonObject.getAsJsonObject("reviewDetails");
            JsonObject reviewCollection = reviewDetail.getAsJsonObject("reviewCollection");
            JsonArray reviewArray = reviewCollection.getAsJsonArray("review");
            Review[] reviews = gson.fromJson(reviewArray, Review[].class);
            if(reviews == null) return;
//            System.out.println("reviews length:" + reviews.length);
//            logger.debug("reviews length:" + reviews.length);
            if(reviews.length < 1) return;
            Review head = reviews[0];
            if(head == null) return;
            TreeSet<Review> treeSet = new TreeSet<>(new ReviewComparator());
            for(Review review: reviews){
                treeSet.add(review);
            }
            hotelManager.addReview(head.getHotelId(), treeSet);
            ReviewModel model = new ReviewManager();
            model.addBatch(reviews);
        }
        catch (IOException e){
            System.out.println("[parseReviews IOException]");
//            logger.debug("[parseReviews IOException]");
        }
    }
}
