package javaClasses.webServer;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name = "request")
public class RockRequest {

    public int getRockID() {
        return rockID;
    }

    public void setRockID(int rockID) {
        this.rockID = rockID;
    }

    private int rockID;

    @Override
    public String toString() {
        return "Request{" +
                "rockID='" + rockID + "'" +
                '}';
    }
}
