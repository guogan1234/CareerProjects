package data;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.Comparator;

/**
 * a comparator can compare Review class
 */
public class ReviewComparator implements Comparator<Review> {
    /**
     * compare two Review class
     * @param o1 a Review instance
     * @param o2 a Review instance
     * @return In logical ,if o1 great than o2 return positive number, if equal return 0. Otherwise, return negative number
     */
    @Override
    public int compare(Review o1, Review o2) {
        return dateCompare(o1, o2);
    }

    /**
     * first compare two Review class by Date, second compare their reviewId
     * @param o1 a Review instance
     * @param o2 a Review instance
     * @return In logical ,if o1 great than o2 return positive number, if equal return 0. Otherwise, return negative number
     */
    private int dateCompare(Review o1, Review o2){
        int ret = 0;
        String date1 = o1.getDate();
        String date2 = o2.getDate();
        DateTimeFormatter formatter = DateTimeFormatter.ISO_DATE_TIME;
        LocalDate localDate1 = LocalDate.parse(date1, formatter);
        LocalDate localDate2 = LocalDate.parse(date2, formatter);
        ret = localDate1.compareTo(localDate2);
        if(ret == 0){
            String id1 = o1.getReviewId();
            String id2 = o2.getReviewId();
            return id1.compareTo(id2);
        }
        return -ret;
    }
}
