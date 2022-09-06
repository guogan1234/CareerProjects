package data;

/**
 * A class that contains saved hotel info
 */
public class SavedHotel {
    private String hotelId;
    private String hotelName;
    private String username;
    private String date;

    /**
     * Constructor
     */
    public SavedHotel() {
    }

    public SavedHotel(String hotelId, String hotelName, String username, String date) {
        this.hotelId = hotelId;
        this.hotelName = hotelName;
        this.username = username;
        this.date = date;
    }

    public String getHotelId() {
        return hotelId;
    }

    public void setHotelId(String hotelId) {
        this.hotelId = hotelId;
    }

    public String getHotelName() {
        return hotelName;
    }

    public void setHotelName(String hotelName) {
        this.hotelName = hotelName;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }
}
