package javaClasses.webServer;

import javaClasses.databaseClasses.QuestionDeletionRemoval;

import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.ws.Provider;
import javax.xml.ws.Service;
import javax.xml.ws.ServiceMode;
import javax.xml.ws.WebServiceProvider;
import java.io.StringReader;

@WebServiceProvider
@ServiceMode(value = Service.Mode.PAYLOAD)
public class DeleteQuestionController implements Provider<Source> {

    @Override
    public Source invoke(Source request) {
        System.out.println("Processing delete question request...");

        // Deserialize incoming request to extract questionID
        QuestionDeleteRequest requestData = Serializer.deserialize(request, QuestionDeleteRequest.class);
        if (requestData == null || requestData.getQuestionID() == null) {
            return new StreamSource(new StringReader("<response>Error: Invalid Request</response>"));
        }

        int questionID = requestData.getQuestionID();
        System.out.println("Deleting question with ID: " + questionID);

        // Call the database method to delete the question
        boolean isDeleted = QuestionDeletionRemoval.deleteQuestionById(questionID);

        // Prepare response
        String responseMessage = isDeleted ?
                "<response>Question successfully deleted</response>" :
                "<response>Error: Question not found or failed to delete</response>";

        return new StreamSource(new StringReader(responseMessage));
    }
}
