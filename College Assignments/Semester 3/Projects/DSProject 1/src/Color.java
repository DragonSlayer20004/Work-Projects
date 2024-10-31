public class Color {
    int red;
    int blue;
    int green;

    public Color(int _red, int _blue, int _green) {
        red = _red;
        blue = _blue;
        green = _green;
    }
    public Color(Color oldColor) {
        red = oldColor.red;
        blue = oldColor.blue;
        green = oldColor.green;
    }
    public int getRed(){
        return red;
    }
    public int getBlue(){
        return blue;
    }
    public int getGreen(){
        return green;
    }
    public void setRed(int red_){
        red = red_;
    }
    public void setBlue(int blue_){
        blue = blue_;
    }
    public void setGreen(int green_){
        green = green_;
    }
}
