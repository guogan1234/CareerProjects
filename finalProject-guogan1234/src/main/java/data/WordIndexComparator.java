package data;

import java.text.SimpleDateFormat;
import java.util.Comparator;
import java.util.Date;

/**
 * a comparator can compare word index information
 */
public class WordIndexComparator implements Comparator<WordIndex> {
    /**
     * compare two WordIndex class information
     * @param o1 a WordIndex instance
     * @param o2 a WordIndex instance
     * @return In logical ,if o1 great than o2 return positive number, if equal return 0. Otherwise, return negative number
     */
    @Override
    public int compare(WordIndex o1, WordIndex o2) {
        return frequencyCompare(o1, o2);
    }

    /**
     * first compare two WordIndex class by frequency, second compare their date
     * @param o1 a WordIndex instance
     * @param o2 a WordIndex instance
     * @return In logical ,if o1 great than o2 return positive number, if equal return 0. Otherwise, return negative number
     */
    private int frequencyCompare(WordIndex o1, WordIndex o2){
        int ret = 0;
        ret = o1.getFrequency() - o2.getFrequency();
        if(ret == 0){
            Date d1 = null;
            Date d2 = null;
            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss");
            String date1 = o1.getReviewDate();
            String date2 = o2.getReviewDate();
            try{
                d1 = format.parse(date1);
                d2 = format.parse(date2);
            }
            catch (Exception e){
                System.out.println("Word index date string parse exception!");
            }
            ret = d1.compareTo(d2);
        }
        return -ret;
    }
}
