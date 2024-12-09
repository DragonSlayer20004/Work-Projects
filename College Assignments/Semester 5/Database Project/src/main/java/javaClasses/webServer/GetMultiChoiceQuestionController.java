package javaClasses.webServer;

import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.ws.Provider;
import javax.xml.ws.Service;
import javax.xml.ws.ServiceMode;
import javax.xml.ws.WebServiceProvider;
import java.io.StringReader;

import static javaClasses.databaseClasses.GetMultiChoiceQuestionsRetrieval.GetQuestions;

@WebServiceProvider
@ServiceMode(value = Service.Mode.PAYLOAD)
public class GetMultiChoiceQuestionController implements Provider<Source> {

    public Source invoke(Source request) {
        System.out.println("Fetching quiz questions...");

        // Convert Source to Java object
        QuestionRequest requestData = Serializer.deserialize(request, QuestionRequest.class);
        if (requestData != null) {
            System.out.println("Deserialized request for questionID: " + requestData.getQuestionID());
        }

        String response = GetQuestions(requestData.getQuestionID());

        // Return a response
        return new StreamSource(new StringReader(response));
    }
}
