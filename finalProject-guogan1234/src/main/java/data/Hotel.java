package data;

import com.google.gson.annotations.SerializedName;

/**
 * a class that contains hotel information
 */
public class Hotel {
    @SerializedName(value = "f")
    private String name;

    private String id;

    @SerializedName(value = "ll")
    private Location location;

    @SerializedName(value = "ad")
    private String address;

    @SerializedName(value = "ci")
    private String city;

    @SerializedName(value = "pr")
    private String state;

    private double averageRate;

    public Hotel() {
    }

    /**
     * constructor
     * @param other another Hotel instance
     */
    public Hotel(Hotel other){
        if(other == null) return;
        this.name = other.name;
        this.id = other.id;
        this.location = new Location(other.location.getLatitude(), other.location.getLongitude());
        this.address = other.address;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getAddress() {
        return address;
    }

    public String getCity() {
        return city;
    }

    public String getState() {
        return state;
    }

    public Location getLocation() {
        return new Location(location.getLatitude(), location.getLongitude());
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setId(String id) {
        this.id = id;
    }

    public void setLocation(Location location) {
        this.location = new Location(location.getLatitude(), location.getLongitude());
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public void setState(String state) {
        this.state = state;
    }

    public double getAverageRate() {
        return averageRate;
    }

    public void setAverageRate(double averageRate) {
        this.averageRate = averageRate;
    }

    /**
     * toString
     * @return string representation of this hotel
     */
    @Override
    public String toString() {
        return "***" + System.lineSeparator() +
                "name:" + name + System.lineSeparator() +
                "id:" + id + System.lineSeparator() +
                "Location:" + location.getLatitude() + "," + location.getLongitude() + System.lineSeparator() +
                "address:" + address + System.lineSeparator() +
                "city:" + city + System.lineSeparator() +
                "state:" + state;
    }

    /**
     * create string for printing
     * @return
     */
    public String getPrintString(){
        return System.lineSeparator() + "********************" + System.lineSeparator() +
                name + ": " + id + System.lineSeparator() +
                address + System.lineSeparator() +
                city + ", " + state + System.lineSeparator();
    }
}
