package javaClasses.webServer;

import javaClasses.databaseClasses.QuizGradeDeletionRemoval;

import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.ws.Provider;
import javax.xml.ws.Service;
import javax.xml.ws.ServiceMode;
import javax.xml.ws.WebServiceProvider;
import java.io.StringReader;

@WebServiceProvider
@ServiceMode(value = Service.Mode.PAYLOAD)
public class DeleteQuizGradeController implements Provider<Source> {

    @Override
    public Source invoke(Source request) {
        System.out.println("Processing delete quiz grade request...");

        // Deserialize incoming request to extract gradeID
        QuizGradeDeleteRequest requestData = Serializer.deserialize(request, QuizGradeDeleteRequest.class);
        if (requestData == null || requestData.getGradeID() == null) {
            return new StreamSource(new StringReader("<response>Error: Invalid Request</response>"));
        }

        int gradeID = requestData.getGradeID();
        System.out.println("Deleting quiz grade with ID: " + gradeID);

        // Call the database method to delete the quiz grade
        boolean isDeleted = QuizGradeDeletionRemoval.deleteQuizGradeById(gradeID);

        // Prepare response
        String responseMessage = isDeleted ?
                "<response>Quiz grade successfully deleted</response>" :
                "<response>Error: Quiz grade not found or failed to delete</response>";

        return new StreamSource(new StringReader(responseMessage));
    }
}
