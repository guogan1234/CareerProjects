package parsing;

import data.HotelManager;

import java.io.File;

/**
 * a class traverse specific directory
 */
public class DirectoryTraverser {
    /**
     * traverse specific directory
     * @param path specific directory path
     * @param hotelManager a class manages hotel information
     */
    public static void traverse(String path, HotelManager hotelManager){
        if(path == null || hotelManager == null) return;
        System.out.println("[DirectoryTraverse]" + path);
        File file = new File(path);
        if(file.isDirectory()) {
            recursiveTraverse(file, ".json", hotelManager);
        }
    }

    /**
     * recursive traverse directory
     * @param path specific directory
     * @param ext file extension
     * @param hotelManager a class manages hotel information
     */
    private static void recursiveTraverse(File path, String ext, HotelManager hotelManager){
        String[] list = path.list();
        String srcPath = path.getPath();
        System.out.println("#:" + srcPath);
        for(int i = 0; i < list.length;i++){
            File temp = new File(srcPath + File.separator + list[i]);
            if(temp.isDirectory()){
                recursiveTraverse(temp, ext, hotelManager);
            }
            if(temp.isFile()){
                String filePath = temp.getPath();
//                System.out.println("@:" + filePath);
                if(filePath.contains(ext)){
//                    paths.add(temp.getPath());
                    ReviewParser.parseReviews(filePath, hotelManager);
                }
            }
        }
        return;
    }
}
