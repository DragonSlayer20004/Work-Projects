package javaClasses.webServer;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name = "request")
public class QuizGradeDeleteRequest {

    private Integer gradeID;

    @XmlElement
    public Integer getGradeID() {
        return gradeID;
    }

    public void setGradeID(Integer gradeID) {
        this.gradeID = gradeID;
    }

    @Override
    public String toString() {
        return "QuizGradeDeleteRequest{" +
                "gradeID=" + gradeID +
                '}';
    }
}
