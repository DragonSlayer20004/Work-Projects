/*
 *Authors: Aidan Kent, Kyle Hinkle
 * Course: COMP 2100
 * Assignment: Project 1
 * Date: 8/24/2023
 */

import java.io.*;
import java.util.Scanner;

public class Manipulator {
    /**
     * Kyle Hinkle
     * Main method to start. Requests the user to input a image file to be manipulated.
     * @param args
     */
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Boolean realFile = false;
        FileInputStream pictureFile = null;
        Bitmap bitmap;
        while(!realFile) {
            System.out.print("What image file would you like to edit: ");
            String fileName = scanner.next();
            try {
                pictureFile = new FileInputStream(fileName);
                realFile = true;
            }catch (Exception e){
                System.out.println("Invalid File: " + fileName + " Please try again");
            }
        }
        String userChoice = "null";
        try {
            bitmap = new Bitmap(pictureFile);
            pictureFile.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        while (!userChoice.equals("q"))
        {
            System.out.print("What command would you like to perform (i, g, b, v, s, d, r, m, h, or q): ");
            userChoice = scanner.next().toLowerCase();

            switch (userChoice) {
                case "i": {
                    bitmap = invert(bitmap);
                    break;
                }
                case "g": {
                    bitmap = grayScale(bitmap);
                    break;
                }
                case "b": {
                    bitmap = blur(bitmap);
                    break;
                }
                case "v": {
                    bitmap = verticallyFlip(bitmap);
                    break;
                }
                case "s": {
                    bitmap = shrink(bitmap);
                    break;
                }
                case "d": {
                    bitmap = enlarge(bitmap);
                    break;
                }
                case "r": {
                    bitmap = rotate(bitmap);
                    break;
                }
                case "m": {
                    bitmap = medianFilter(bitmap);
                    break;
                }
                case "h": {
                    bitmap = horizontallyMirror(bitmap);
                    break;
                }
                case "q": {
                    closeFile(bitmap);
                    break;
                }
                default: {
                    System.out.println("Invalid command, please try again");
                }
            }
        }
    }

    /**
     * Kyle Hinkle
     * Invert the picture
     * @param bitmap
     * @return
     */
    public static Bitmap invert(Bitmap bitmap) {

        Bitmap invertBitmap = new Bitmap(bitmap.height(), bitmap.width());
        for(int i = 0; i < bitmap.height(); i++) { //Loop through every pixel in the image
            for (int j = 0; j < bitmap.width(); j++) {

                Color colorOriginal = (bitmap.get(i, j));
                int red = 255-colorOriginal.getRed();
                int green = 255-colorOriginal.getGreen();
                int blue = 255-colorOriginal.getBlue();

                Color invertColor = new Color(red,green,blue); //Set new color for pixel
                invertBitmap.set(i,j, invertColor);
            }
        }
        return invertBitmap;
    }

    /**
     * Kyle Hinkle
     * Gray the picture
     * @param bitmap
     * @return
     */
    public static Bitmap grayScale(Bitmap bitmap) {
        Bitmap grayBitmap = new Bitmap(bitmap.height(), bitmap.width());
        for(int i = 0; i < bitmap.width(); ++i) { //Loop through every pixel in the image
            for (int j = 0; j < bitmap.height(); ++j) {

                Color colorOriginal = (bitmap.get(j, i));
                int greyScaledAmount = (int)Math.round(colorOriginal.getRed()*.3 + colorOriginal.getGreen()*.59 + colorOriginal.getBlue()*.11);
                greyScaledAmount = checkColor(greyScaledAmount);

                Color greyScaledColor = new Color(greyScaledAmount,greyScaledAmount,greyScaledAmount); //Set new color for pixel
                grayBitmap.set(j,i, greyScaledColor);
            }
        }
        return grayBitmap;
    }

    /**
     * Kyle Hinkle
     * Blurs the picture
     * @param bitmap
     */
    public static Bitmap blur(Bitmap bitmap){
        Bitmap blurBitmap = new Bitmap(bitmap.height(), bitmap.width());

        for(int i = 0; i < bitmap.height(); i++) { //Loop through every pixel in the image
            for (int j = 0; j < bitmap.width(); j++) {
                int blurredValueRed=0;
                int blurredValueBlue=0;
                int blurredValueGreen=0;
                int total = 0;

                //Goes through searching for 25 pixels to compare to and round.
                for(int row = -2; row < 3; row++){
                    for(int column = -2; column < 3; column++){
                        if(j+column < bitmap.width() && j+column >= 0 && i+row < bitmap.height() && i+row >= 0) {
                            Color coloredColor = bitmap.get(i + row, j + column);
                            blurredValueRed += coloredColor.red;
                            blurredValueBlue += coloredColor.blue;
                            blurredValueGreen += coloredColor.green;
                            total++;
                        }
                    }
                }
                //Rounds the total caught pixels surrounding the orginal pixel together.
                blurredValueRed = Math.round(blurredValueRed/(total));
                blurredValueBlue = Math.round(blurredValueBlue/(total));
                blurredValueGreen = Math.round(blurredValueGreen/(total));
                Color newColor = new Color(blurredValueRed, blurredValueBlue, blurredValueGreen);

                blurBitmap.set(i, j, newColor);
            }
        }
        return blurBitmap;
    }

    /**
     * Aiden Kent
     * Vertically flips the image.
     * @param bitmap
     * @return
     */
    public static Bitmap verticallyFlip(Bitmap bitmap) {
        Bitmap flipBitmap = new Bitmap(bitmap.height(), bitmap.width());
        for(int i = 0; i <= bitmap.height() - 1; ++i) {
            for (int j = 0; j <= bitmap.width() - 1; ++j) {
                flipBitmap.set((bitmap.height() -1) - i, j, bitmap.get(i, j));
            }
        }
        bitmap = flipBitmap;
        return bitmap;
    }

    /**
     * Kyle Hinkle
     * Shrinks the image to be half it's original size.
     * @param bitmap
     * @return
     */
    public static Bitmap shrink(Bitmap bitmap){
        int width = 0;
        int height = 0;

        //Check if width and height are odd and if they are ignore the last odd pixel.
        if((bitmap.width()/2)%2 == 1){
            width = (bitmap.width()-1)/2;
        } else {
            width = bitmap.width()/2;
        }
        if((bitmap.height()/2)%2 == 1){
            height = (bitmap.height()-1)/2;
        } else {
            height = bitmap.height()/2;
        }
        //create new picture based on whether height and width are odd
        Bitmap shrinkBitmap = new Bitmap(height, width);
        int realPlaceWidth = 0;
        int realPlaceHeight = 0;


        for(int i = 0; i < height*2; i+=2) { //Loop through every pixel in the image
            realPlaceWidth = 0;
            for (int j = 0; j < width*2; j+=2) {
                //grab 4 pixels
                Color color1 = bitmap.get(i,j);
                Color color2 = bitmap.get(i+1,j);
                Color color3 = bitmap.get(i,j+1);
                Color color4 = bitmap.get(i+1,j+1);
                //average out the pixel colors
                int red = (color1.red + color2.red + color3.red + color4.red)/4;
                int green = (color1.green + color2.green + color3.green + color4.green)/4;
                int blue = (color1.blue + color2.blue + color3.blue + color4.blue)/4;
                Color newColor = new Color(red, blue, green);

                shrinkBitmap.set(realPlaceHeight,realPlaceWidth, newColor);
                realPlaceWidth++;
            }
            realPlaceHeight++;
        }
        return shrinkBitmap;
    }

    /**
     * Kyle Hinkle
     * Enlarges the picture to be double it's original size.
     * @param bitmap
     * @return
     */
    public static Bitmap enlarge(Bitmap bitmap){
        Bitmap growBitmap = new Bitmap(bitmap.height()*2, bitmap.width()*2);
        int currentHeight = 0;
        int currentWidth = 0;
        for(int i = 0; i < bitmap.height() && currentHeight < growBitmap.height(); i++){
            for(int j = 0; j< bitmap.width() && currentWidth < growBitmap.width(); j++){
                Color originalColor = bitmap.get(i, j);

                growBitmap.set(currentHeight,currentWidth, originalColor);
                growBitmap.set(currentHeight+1,currentWidth, originalColor);
                growBitmap.set(currentHeight,currentWidth+1, originalColor);
                growBitmap.set(currentHeight+1,currentWidth+1, originalColor);
                currentWidth+=2;
            }
            currentWidth = 0;
            currentHeight+=2;
        }
        return growBitmap;
    }
    /**
     * Kyle Hinkle
     * Rotates image to the right, clockwise.
     * @param bitmap
     * @return
     */
    public static Bitmap rotate(Bitmap bitmap) {
        Bitmap rotateBitmap = new Bitmap(bitmap.width(), bitmap.height());
        for(int i = 0; i < bitmap.width(); ++i){
            for(int j = 0; j < bitmap.height(); ++j) {
                rotateBitmap.set(i, bitmap.height()-j-1 , bitmap.get(j, i));
            }
        }
        return rotateBitmap;
    }
    /**
     * Kyle Hinkle
     * Mirrors image along the y-axis
     * @param bitmap
     * @return
     */
    public static Bitmap horizontallyMirror(Bitmap bitmap) {
        Bitmap mirroredBitmap = new Bitmap(bitmap.height(), bitmap.width());
        for(int i = 0; i < bitmap.width(); ++i){
            for(int j = 0; j < bitmap.height(); ++j) {
                mirroredBitmap.set(j, bitmap.width() -i - 1, bitmap.get(j, i));
            }
        }
        return mirroredBitmap;
    }

    /**
     * Kyle Hinkle
     * Applies a median filter to the image.
     * @param bitmap
     * @return
     */
    public static Bitmap medianFilter(Bitmap bitmap) {
        Bitmap filteredBitmap = new Bitmap(bitmap.height(), bitmap.width());
        for(int i = 0; i < bitmap.width(); ++i) { //Loop through every pixel in the image
            for (int j = 0; j < bitmap.height(); ++j) {

                int[] red = new int[9]; //Declaring variables
                int[] green = new int[9];
                int[] blue = new int[9];
                int total = 0;

                //Goes through searching for 9 pixels to compare to and round.
                for(int row = -1; row < 2; row++){
                    for(int column = -1; column < 2; column++){
                        if(i+column < bitmap.width() && i+column >= 0 && j+row < bitmap.height() && j+row >= 0) {
                            Color coloredColor = bitmap.get(j + row, i + column);
                            red[total] = coloredColor.red;
                            green[total] = coloredColor.blue;
                            blue[total] = coloredColor.green;
                            total++;
                        }
                    }
                }
                int redMedian = sort(red); //using sort in order to get the median value
                int greenMedian = sort(green);
                int blueMedian = sort(blue);

                Color medianColor = new Color(redMedian,blueMedian, greenMedian); //Set new color for pixel
                filteredBitmap.set(j,i, medianColor);
            }
        }
        return filteredBitmap;
    }

    /**
     * Kyle Hinkle
     * Sorts values around for median filter
     * @param values
     * @return
     */
    public static int sort( int[] values ) {
        for(int i = 0; i < values.length; i++){ //loop through the entire array of values replacing the order from lowest to highest.
            for(int j = i; j < values.length; j++) { //loop through array starting at location i as everything before it is smaller and were trying to find the next smallest
                if (values[j] > values[i]) {
                    int temp = values[i]; //holds current I
                    values[i] = values[j]; //Replace I with J
                    values[j] = temp; // Move orginal I and replace with J
                    j--;
                }
            }
        }
        return values[4];
    }

    /**
     * Kyle Hinkle
     * It also makes a new fileOutputStream in order to save the new image.
     * Aiden Kent did Padding.
     * @param bitmap
     */
    public static void closeFile(Bitmap bitmap){
        int padding = (4 - (bitmap.width() * 3) % 4) % 4;
        Boolean realFile = false;
        while (!realFile) {
            System.out.print("What do you want to name your new image file: ");
            Scanner scanner = new Scanner(System.in);
            String newFileName = scanner.next();
            try {
                FileOutputStream newPicture = new FileOutputStream(newFileName);
                newPicture.write('B'); //Creates the header of the new file.
                newPicture.write('M'); //unsigned char
                int dataSize = bitmap.height() * (bitmap.width() * 3 + padding);
                int wholeSize = dataSize+54+2;
                IntToByte(wholeSize,newPicture); // total size of file
                IntToByte(0, newPicture); // Reserved, always 0
                IntToByte(54, newPicture); // Offset, start of data from front of file, should be 54
                IntToByte(40, newPicture); // size of header, always 40
                IntToByte(bitmap.width(), newPicture); // width of image in pixels
                IntToByte(bitmap.height(), newPicture); // height of image in pixels
                ShortToByte((short) 1, newPicture); // planes in image, always 1
                ShortToByte((short) 24, newPicture); // color bit depths, always 24
                IntToByte(0, newPicture); //Compression, always 0
                IntToByte(dataSize, newPicture); // size of color data in bytes
                IntToByte(72, newPicture); // horizontalResolution, unreliable, use 72 when writing
                IntToByte(72, newPicture); // verticalResolution, unreliable, use 72 when writing
                IntToByte(0, newPicture); // colors in palette, use 0 when writing
                IntToByte(0, newPicture); // important colors, use 0 when writing

                for(int i = bitmap.height()-1; i >= 0; --i) { //Loop through every pixel in the image
                    for (int j = 0; j < bitmap.width(); ++j) {
                        newPicture.write(bitmap.colorMap[i][j].blue);
                        newPicture.write(bitmap.colorMap[i][j].green);
                        newPicture.write(bitmap.colorMap[i][j].red);
                    }
                    //Adds in the extra padding to every column if needed.
                    for(int j = 0; j < padding; ++j) {
                        newPicture.write(0);
                    }
                }


                //Padding for Header
                newPicture.write(0);
                newPicture.write(0);

                //Close all the files in use
                newPicture.close();
                realFile = true;
                System.out.println("Successfully saved your new file!");
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    /**
     * Kyle Hinkle
     * Checks to make sure that colors are within 0-255 and if they aren't it will force them to be.
     * @param colorValue
     * @return
     */
    private static int checkColor(int colorValue) { //check color value to insure its under/above the threshold
        if (colorValue < 0) {
            colorValue = 0;
        } else if (colorValue > 255) {
            colorValue = 255;
        }
        return colorValue;
    }

    /**
     * Kyle Hinkle
     * Writes out ints into byte format
     * @param changingInt
     * @param outputStream
     * @throws IOException
     */
    private static void IntToByte(int changingInt, FileOutputStream outputStream) throws IOException {
        outputStream.write(changingInt); //+ (changingInt >> 16) + (changingInt >> 24);
        outputStream.write((changingInt >>> 8));
        outputStream.write((changingInt >>> 16));
        outputStream.write((changingInt >>> 24));
    }

    /**
     * Kyle Hinkle
     * Writes out shorts into byte format
     * @param changingShort
     * @param outputStream
     * @throws IOException
     */
    private static void ShortToByte(short changingShort, FileOutputStream outputStream) throws IOException {
        outputStream.write(changingShort); //+ (changingInt >> 16) + (changingInt >> 24);
        outputStream.write((changingShort >>> 8));
    }


    /*
    /**
     * Kyle Hinkle
     * Takes in the file and grabs all the bytes out of it.
     * We want to edit the byte array and not the file itself.
     * @param filePicture
     * @return

    public static byte[] byteConverter(FileInputStream filePicture){
        byte[] newByte;
        try {
            newByte = filePicture.readAllBytes();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        /*
        Padding should go here

        System.out.println("Image Byte size: " + newByte.length);
        return newByte;
    }
    */
}

