package javaClasses.webServer;

import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.ws.Provider;
import javax.xml.ws.Service;
import javax.xml.ws.ServiceMode;
import javax.xml.ws.WebServiceProvider;
import javax.xml.ws.WebServiceContext;
import javax.xml.ws.handler.MessageContext;
import java.io.StringReader;

import static javaClasses.databaseClasses.AnswerCreationDepository.insertAnswer;

@WebServiceProvider
@ServiceMode(value = Service.Mode.PAYLOAD)
public class CreateAnswerController implements Provider<Source> {

    // Inject the WebServiceContext to manage headers and other details
    @javax.annotation.Resource
    private WebServiceContext context;

    public Source invoke(Source request) {
        System.out.println("Inserting answers...");

        // Convert Source to Java object (assume AnswerRequest is a class containing the necessary data)
        AnswerRequest requestData = Serializer.deserialize(request, AnswerRequest.class);
        if (requestData != null) {
            System.out.println("Deserialized request for questionID: " + requestData.getQuestionID());
        }

        // Call insertAnswer method to insert the answers into the database
        boolean success = insertAnswer(requestData);

        // Create a response message
        String response = success ? "<response>Answers inserted successfully!</response>" : "<response>Failed to insert answers.</response>";

        // Set the response headers if needed (optional, depending on use case)
        setResponseHeaders(success);

        // Return the response as a Source
        return new StreamSource(new StringReader(response));
    }

    // Method to set response headers and HTTP status code based on success or failure
    private void setResponseHeaders(boolean success) {
        MessageContext msgContext = context.getMessageContext();
        if (msgContext != null) {
            // Set HTTP status code based on the result
            msgContext.put(MessageContext.HTTP_RESPONSE_CODE, success ? 200 : 500); // 200 for success, 500 for error
            // Optionally, add custom headers here, for example:
            // msgContext.put("Custom-Header", "Value");
        }
    }
}
