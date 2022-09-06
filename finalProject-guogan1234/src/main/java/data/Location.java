package data;

import com.google.gson.annotations.SerializedName;

/**
 * a class that presents location
 */
public class Location {
    @SerializedName(value = "lat")
    private String latitude;
    @SerializedName(value = "lng")
    private String longitude;

    /**
     * constructor
     * @param latitude the latitude of location
     * @param longitude the longitude of location
     */
    public Location(String latitude, String longitude){
        this.latitude = latitude;
        this.longitude = longitude;
    }

    public void setLatitude(String latitude){
        this.latitude = latitude;
    }

    public String getLatitude(){
        return latitude;
    }

    public void setLongitude(String longitude){
        this.longitude = longitude;
    }

    public String getLongitude(){
        return longitude;
    }
}
