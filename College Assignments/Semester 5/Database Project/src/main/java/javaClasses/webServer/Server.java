package javaClasses.webServer;

import com.sun.net.httpserver.HttpServer;

import javax.xml.ws.*;
import java.io.IOException;
import java.net.InetSocketAddress;

@WebServiceProvider
@ServiceMode(value = Service.Mode.PAYLOAD)
public class Server{
    public static void main(String[] args) throws InterruptedException {
        String schema = "http";
        String hostName = "127.0.0.1";
        String port = "12345";
        String address = (schema + "://" + hostName + ":" + port);

        // Start an HttpServer
        HttpServer httpServer = null;
        try {
            httpServer = HttpServer.create(new InetSocketAddress("127.0.0.1", 12345), 0);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        //Rock Requests
        httpServer.createContext("/rocks", new CORSHandler(new RocksController()));
        httpServer.createContext("/rocksName", new CORSHandler(new GetRockNameController()));
        httpServer.createContext("/rocksImage", new CORSHandler(new GetRockImageController()));
        httpServer.createContext("/rocksDescription", new CORSHandler(new GetRockDescriptionController()));

        //Question Requests
        httpServer.createContext("/getQuestions", new CORSHandler(new GetQuestionsController()));
        httpServer.createContext("/getMultiChoiceQuestion", new CORSHandler(new GetMultiChoiceQuestionController()));
        httpServer.createContext("/DeleteQuestion", new CORSHandler(new DeleteQuestionController()));
        httpServer.createContext("/UpdateQuestion", new CORSHandler(new UpdateQuestionsController()));
        httpServer.createContext("/CreateQuestion", new CORSHandler(new CreateQuestionsController()));
//        httpServer.createContext("/getFillInTheBlankQuestion", new CORSHandler();
//        httpServer.createContext("/getDragAndDropQuestion", new CORSHandler();

        //User Requests
        httpServer.createContext("/getUser", new CORSHandler(new GetUserController()));

        //Post Grade Requests
        httpServer.createContext("/SaveQuizGrade", new CORSHandler(new PostQuizGradeController()));
        httpServer.createContext("/DeleteQuizGrade", new CORSHandler(new DeleteQuizGradeController()));

        //Create Answers
        httpServer.createContext("/CreateAnswer", new CORSHandler(new CreateAnswerController()));

        httpServer.setExecutor(null); // creates a default executor
        httpServer.start();

        System.out.println("Service running at " + address);
        System.out.println("Type [CTRL]+[C] to quit!");

        Thread.sleep(Long.MAX_VALUE);
    }
}