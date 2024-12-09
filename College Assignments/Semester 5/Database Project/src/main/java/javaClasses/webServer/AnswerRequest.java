package javaClasses.webServer;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name = "AnswerRequest")
public class AnswerRequest {

    private String answerDescription;
    private int questionID;

    // Default constructor for JAXB
    public AnswerRequest() {
    }

    // Constructor with parameters
    public AnswerRequest(String answerDescription, int questionID) {
        this.answerDescription = answerDescription;
        this.questionID = questionID;
    }

    // Getter for answerDescription
    @XmlElement(name = "answerDescription")
    public String getAnswerDescription() {
        return answerDescription;
    }

    // Setter for answerDescription
    public void setAnswerDescription(String answerDescription) {
        this.answerDescription = answerDescription;
    }

    // Getter for questionID
    @XmlElement(name = "questionID")
    public int getQuestionID() {
        return questionID;
    }

    // Setter for questionID
    public void setQuestionID(int questionID) {
        this.questionID = questionID;
    }

    @Override
    public String toString() {
        return "AnswerRequest{" +
                "answerDescription='" + answerDescription + '\'' +
                ", questionID=" + questionID +
                '}';
    }
}
