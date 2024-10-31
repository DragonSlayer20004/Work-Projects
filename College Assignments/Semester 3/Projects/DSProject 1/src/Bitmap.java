import java.io.FileInputStream;
import java.io.IOException;

public class Bitmap {
    Color[][] colorMap; //Pixel Map

    /**
     * Kyle Hinkle
     * Creates a pixelmap from a fileInputStream
     * @param _fileInput
     * @throws IOException
     */
    public Bitmap(FileInputStream _fileInput) throws IOException {
        FileInputStream fileInput = _fileInput;
        createPixelMap(fileInput);
    }

    /**
     * Kyle Hinkle
     * Creates a empty pixel map with the given height and width.
     * @param height
     * @param width
     */
    public Bitmap(int height, int width){
        colorMap = new Color[height][width];
    }

    /**
     * Kyle Hinkle
     * Gives width of pixelmap
     * @return
     */
    public int width(){
        return colorMap[0].length;
    }

    /**
     * Kyle Hinkle
     * Gives Height of pixelmap
     * @return
     */
    public int height(){
        return colorMap.length;
    }

    /**
     * Kyle Hinkle
     * Reads in the fileInputStream and constructs a header and bitmap out of it.
     * Aiden Kent did Padding.
     * @param fileInput
     * @throws IOException
     */
    public void createPixelMap(FileInputStream fileInput) throws IOException {
        int fileSize;
        //creates the header of the bitmap
        fileInput.skip(2);
        fileSize = byteToInt(fileInput);
        fileInput.skip(12);
        int width = byteToInt(fileInput);
        int height = byteToInt(fileInput);
        fileInput.skip(28);
        System.out.println();
        System.out.println(width + "   " + height);

        //creates the pixels within the bitmap
        colorMap = new Color[height][width];
        int padding = (4 - (width * 3) % 4) % 4;
        for(int i = height-1; i >= 0; --i) { //Loop through every pixel in the image
            for (int j = 0; j < width; ++j) {
                colorMap[i][j] = decodePixel(fileInput);
            }
            fileInput.skip(padding);
        }
    }

    /**
     * Kyle Hinkle
     * Gets the selected pixel from the pixelMap
     * @param height
     * @param width
     * @return
     */
    public Color get(int height, int width){
        return colorMap[height][width];
    }

    /**
     * Kyle Hinkle
     * Sets the selected height width in the pixelmap to a pixel.
     * @param height
     * @param width
     * @param color
     */
    public void set(int height, int width, Color color){
        colorMap[height][width] = color;
    }

    /**
     * Kyle Hinkle
     * Reads in bytes and converts them into a pixel.
     * @param fileInput
     * @return
     * @throws IOException
     */
    private Color decodePixel(FileInputStream fileInput) throws IOException {
        Color newColor = new Color(0,0,0);

        newColor.setBlue(fileInput.read());
        newColor.setGreen(fileInput.read());
        newColor.setRed(fileInput.read());

        return newColor;
    }

    /**
     * /Kyle Hinkle
     * Takes four bytes and turns them into an int
     * @return
     */
    private static int byteToInt(FileInputStream fileInput) throws IOException {
        int value1 = fileInput.read();
        int value2 = fileInput.read();
        int value3 = fileInput.read();
        int value4 = fileInput.read();

        return value1 + (value2 << 8) + (value3 << 16) + (value4 << 24);
    }
}