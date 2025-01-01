package javaClasses.webServer;

import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.ws.Provider;
import javax.xml.ws.Service;
import javax.xml.ws.ServiceMode;
import javax.xml.ws.WebServiceProvider;
import java.io.StringReader;

import static javaClasses.databaseClasses.RockNameRetrieval.GetRockInfoById;

@WebServiceProvider
@ServiceMode(value = Service.Mode.PAYLOAD)
public class RocksController implements Provider<Source> {

    public Source invoke(Source request) {
        System.out.println("AM I BEING IN A ROCK THINGY??");

        // Convert Source to Java object
        RockRequest requestData = Serializer.deserialize(request, RockRequest.class);
        if (requestData != null) {
            System.out.println("Deserialized ID: " + requestData.getRockID());
        }

        // Return a response
        return new StreamSource(new StringReader("<div><p>Hello There! THIS HERE IS ROCKS BISCUITS!!!!" + "</p>" +
        "<p> Your rock is: " + GetRockInfoById(requestData.getRockID()) +
                "</p></div>"
        ));
    }
}