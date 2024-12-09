package javaClasses.webServer;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name = "request")
public class QuestionDeleteRequest {

    private Integer questionID;

    @XmlElement
    public Integer getQuestionID() {
        return questionID;
    }

    public void setQuestionID(Integer questionID) {
        this.questionID = questionID;
    }

    @Override
    public String toString() {
        return "QuestionDeleteRequest{" +
                "questionID=" + questionID +
                '}';
    }
}
