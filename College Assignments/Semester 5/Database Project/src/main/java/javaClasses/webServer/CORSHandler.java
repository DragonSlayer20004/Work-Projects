package javaClasses.webServer;

import com.sun.net.httpserver.Headers;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.ws.Provider;
import java.io.*;


// CORSHandler to handle requests and add CORS headers
class CORSHandler implements HttpHandler {
    private Provider<Source> server;

    public CORSHandler(Provider<Source> server) {
        this.server = server;
    }

    @Override
    public void handle(HttpExchange exchange) throws IOException {
        String requestMethod = exchange.getRequestMethod().toUpperCase();
        Headers responseHeaders = exchange.getResponseHeaders();

        // Add CORS headers to the response
        responseHeaders.add("Access-Control-Allow-Origin", "*");
        responseHeaders.add("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        responseHeaders.add("Access-Control-Allow-Headers", "Content-Type");

        // Handle OPTIONS preflight request
        if ("OPTIONS".equals(requestMethod)) {
            exchange.sendResponseHeaders(204, -1); // 204 No Content for OPTIONS requests
            return;
        }

        // Handle POST requests and forward to the JAX-WS service
        if ("POST".equals(requestMethod)) {
            String line;

            // Call the JAX-WS provider's invoke method manually
            Source requestSource = new StreamSource(exchange.getRequestBody());
            Source responseSource = server.invoke(requestSource);

            // Convert the response Source back to a string
            StringWriter writer = new StringWriter();
            StreamSource streamSource = (StreamSource) responseSource;
            BufferedReader responseReader = new BufferedReader(streamSource.getReader());
            while ((line = responseReader.readLine()) != null) {
                writer.write(line);
            }

            // Write response
            byte[] responseBytes = writer.toString().getBytes("UTF-8");
            try (OutputStream os = exchange.getResponseBody()) {
                exchange.sendResponseHeaders(200, writer.toString().length());
                os.write(responseBytes);
            }

        } else {
            String response = "Method Not Allowed";
            exchange.sendResponseHeaders(405, response.length());
            try (OutputStream os = exchange.getResponseBody()) {
                os.write(response.getBytes());
            }
        }
    }
}