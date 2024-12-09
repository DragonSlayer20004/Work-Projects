package javaClasses.webServer;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name = "request")
public class UserRequest {
    private int userID;

    @XmlElement
    public int getUserID() {
        return userID;
    }

    public void setUserID(int userID) {
        this.userID = userID;
    }

    @Override
    public String toString() {
        return "UserRequest{" +
                "userID=" + userID +
                '}';
    }
}
