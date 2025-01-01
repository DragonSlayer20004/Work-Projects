package javaClasses.webServer;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Unmarshaller;
import javax.xml.transform.Source;

public class Serializer {
    // Utility method to convert Source to a Java object
    public static <T> T deserialize(Source source, Class<T> clazz) {
        try {
            JAXBContext jaxbContext = JAXBContext.newInstance(clazz);
            Unmarshaller unmarshaller = jaxbContext.createUnmarshaller();
            return unmarshaller.unmarshal(source, clazz).getValue();
        } catch (JAXBException e) {
            e.printStackTrace();
            return null;
        }
    }
}