package javaClasses.webServer;

import javaClasses.databaseClasses.QuestionCreationDepository;

import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.ws.Provider;
import javax.xml.ws.Service;
import javax.xml.ws.ServiceMode;
import javax.xml.ws.WebServiceProvider;
import java.io.StringReader;

@WebServiceProvider
@ServiceMode(value = Service.Mode.PAYLOAD)
public class CreateQuestionsController implements Provider<Source> {

    public Source invoke(Source request) {
        System.out.println("Creating a new quiz question...");

        // Convert Source to Java object
        CreateQuestionRequest requestData = Serializer.deserialize(request, CreateQuestionRequest.class);
        if (requestData != null) {
            System.out.println("Deserialized request for creating question of type: " + requestData.getQuestionType());
        } else {
            return new StreamSource(new StringReader("<response><status>error</status><message>Invalid request</message></response>"));
        }

        boolean creationSuccess = false;
        String message = "Unknown error";

        try {
            switch (requestData.getQuestionType()) {
                case "multichoice":
                    creationSuccess = QuestionCreationDepository.createMultichoiceQuestion(
                            requestData.getQuestionID(),
                            requestData.getQuestionDescription(),
                            requestData.getMultichoiceOptions(),
                            requestData.getCorrectAnswer()
                    );
                    break;

                case "fillInTheBlank":
                    creationSuccess = QuestionCreationDepository.createFillInTheBlankQuestion(
                            requestData.getQuestionID(),
                            requestData.getQuestionDescription(),
                            requestData.getCorrectAnswerText()
                    );
                    break;

                case "dragAndDrop":
                    creationSuccess = QuestionCreationDepository.createDragAndDropQuestion(
                            requestData.getQuestionID(),
                            requestData.getQuestionDescription(),
                            requestData.getCorrectAnswer(),
                            requestData.getWrongAnswerOne(),
                            requestData.getWrongAnswerTwo(),
                            requestData.getWrongAnswerThree()
                    );
                    break;

                default:
                    message = "Unsupported question type: " + requestData.getQuestionType();
            }

            if (creationSuccess) {
                message = "Question created successfully!";
            } else {
                message = "Failed to create question.";
            }

        } catch (Exception e) {
            System.err.println("Error creating question: " + e.getMessage());
            message = "Exception occurred: " + e.getMessage();
            e.printStackTrace();
        }

        // Return the response as XML
        String responseXml = "<response><status>" + (creationSuccess ? "success" : "error") +
                "</status><message>" + message + "</message></response>";
        return new StreamSource(new StringReader(responseXml));
    }
}
