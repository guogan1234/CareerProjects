package parsing;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import data.Hotel;
import data.HotelManager;
import model.HotelModel;
import model.manager.HotelDataManager;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

/**
 * a class that parses hotel data
 */
public class HotelParser {
    /**
     * parse hotel file
     * @param filePath hotel file
     * @param hotelManager a class manages hotel information
     */
    public void parseHotelData(String filePath, HotelManager hotelManager){
        if(filePath == null) return;
        System.out.println("[HotelParser]:" + filePath);
        Gson gson = new Gson();

        try {
            String fileData = new String(Files.readAllBytes(Paths.get(filePath)));
            JsonParser parser = new JsonParser();
            JsonObject jsonObject = (JsonObject) parser.parse(fileData);
            JsonArray jsonArray = jsonObject.getAsJsonArray("sr");
            Hotel[] hotels = gson.fromJson(jsonArray, Hotel[].class);
            for(Hotel hotel: hotels){
                if(hotel == null) continue;
//                System.out.println(hotel.getId() + ",[#]," + hotel);
                hotelManager.addHotel(hotel.getId(), hotel);
            }
            HotelModel model = new HotelDataManager();
            model.addBatch(hotels);
        }catch (IOException e){
            System.out.println("[parseHotelData, IOException]");
        }
    }
}
