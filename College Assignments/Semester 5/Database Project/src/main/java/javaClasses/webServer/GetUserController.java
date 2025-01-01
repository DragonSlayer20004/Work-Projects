package javaClasses.webServer;

import javaClasses.databaseClasses.UserAndGradesRetrieval;

import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.ws.Provider;
import javax.xml.ws.Service;
import javax.xml.ws.ServiceMode;
import javax.xml.ws.WebServiceProvider;
import java.io.StringReader;

@WebServiceProvider
@ServiceMode(value = Service.Mode.PAYLOAD)
public class GetUserController implements Provider<Source> {

    private static final String ERROR_INVALID_REQUEST = "<response>Error: Invalid Request</response>";
    private static final String ERROR_USER_NOT_FOUND = "<response>Error: User not found</response>";

    @Override
    public Source invoke(Source request) {
        System.out.println("Processing user data request...");

        // Deserialize incoming request to extract userID
        UserRequest requestData = Serializer.deserialize(request, UserRequest.class);
        if (requestData == null || requestData.getUserID() < 0 || requestData.getUserID() <= 0) {
            return new StreamSource(new StringReader(ERROR_INVALID_REQUEST));
        }

        System.out.println("Received Request for UserID: " + requestData.getUserID());

        // Fetch user and grades data for the provided user ID
        UserWithGrades userWithGrades = UserAndGradesRetrieval.getUserWithGrades(requestData.getUserID());

        if (userWithGrades == null) {
            return new StreamSource(new StringReader(ERROR_USER_NOT_FOUND));
        }

        // Serialize response to JSON or XML
        String responseData = userWithGrades.toString();
        return new StreamSource(new StringReader(responseData));
    }
}
