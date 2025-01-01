package javaClasses.webServer;

import javaClasses.databaseClasses.QuizGradeDepository;

import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.ws.Provider;
import javax.xml.ws.Service;
import javax.xml.ws.ServiceMode;
import javax.xml.ws.WebServiceProvider;
import java.io.StringReader;

@WebServiceProvider
@ServiceMode(value = Service.Mode.PAYLOAD)
public class PostQuizGradeController implements Provider<Source> {

    @Override
    public Source invoke(Source request) {
        System.out.println("Processing quiz grade save request...");

        // Deserialize the XML request to QuizGradeData
        QuizGradeData requestData = Serializer.deserialize(request, QuizGradeData.class);
        if (requestData != null) {
            System.out.println("Deserialized Quiz Grade Request: userID=" + requestData.getUserID() +
                    ", quizName=" + requestData.getQuizName() +
                    ", grade=" + requestData.getGrade());
        } else {
            return new StreamSource(new StringReader("<response>Error: Invalid Request</response>"));
        }

        // Save the quiz grade in the database
        boolean isSaved = QuizGradeDepository.saveQuizGrade(requestData);

        // Return appropriate response
        if (isSaved) {
            return new StreamSource(new StringReader("<response>Quiz grade successfully saved!</response>"));
        } else {
            return new StreamSource(new StringReader("<response>Error: Failed to save quiz grade.</response>"));
        }
    }
}
