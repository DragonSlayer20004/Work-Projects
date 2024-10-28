import java.io.File;
import java.io.IOException;
import java.util.*;

public class Graphs {
    private static String reviewFileName;
    public static Graph graph = new Graph();

    /**
     * Kyle Hinkle
     * Main method. takes in a file and allows the user to request information and changes about the graph.
     * @param args
     */
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        //Reads in the files and attempts to put all the words in the files into trees.
        boolean success = false;
        //User input file name and if that file doesn't exist requests them to try again.
        while(!success) {
            try {
                System.out.print("Enter a graph file: ");
                reviewFileName = scanner.nextLine();
                if(reviewFileName.equals("skip1")|| reviewFileName.equals("Skip")){
                    reviewFileName = "src/Example1.txt";
                }
                if(reviewFileName.equals("skip2")|| reviewFileName.equals("Skip2")){
                    reviewFileName = "src/Example2.txt";
                }
                if(reviewFileName.equals("skip3")|| reviewFileName.equals("Skip3")){
                    reviewFileName = "src/Example3.txt";
                }
                Scanner fileScanner = new Scanner(new File(reviewFileName));
                //Reads in all the words in the graph file

                fileIntoGraph(fileScanner);

                success = true;
            } catch (IOException e) {
                System.out.println("Invalid File: " + reviewFileName + " Please try again");
            }
        }
        String userChoice = "";

        while (!userChoice.equals("8")) {
            System.out.println("What command would you like to perform: ");
            System.out.println("1. Is Connected");
            System.out.println("2. Minimum Spanning Tree");
            System.out.println("3. Shortest Path");
            System.out.println("4. Is Metric");
            System.out.println("5. Make Metric");
            System.out.println("6. Traveling Salesman Problem");
            System.out.println("7. Approximate TSP");
            System.out.println("8. Quit");
            System.out.println();
            System.out.print("Make your choice (1 - 8): ");
            userChoice = scanner.next().toLowerCase();
            System.out.println();
            switch (userChoice) {
                case "1": {
                    isConnected();
                    break;
                }
                case "2": {
                    minimumSpanningTree();
                    break;
                }
                case "3": {
                    shortestPath();
                    break;
                }
                case "4": {
                    isMetric();
                    break;
                }
                case "5": {
                    makeMetric();
                    break;
                }
                case "6": {
                    travelingSalesmanProblem();
                    break;
                }
                case "7": {
                    approximateTSP();
                    break;
                }
                case "8": {
                    //Do nothing and let user leave
                    break;
                }
                default: {
                    System.out.println("Invalid command, please try again");
                }
            }
        }
    }

    /**
     * Kyle Hinkle
     * Converts text document to a graph element.
     * @param fileScanner
     */
    private static void fileIntoGraph(Scanner fileScanner) {
        graph.nodeCount = fileScanner.nextInt(); //reads in how many nodes there are
        graph.table = new int[graph.nodeCount][graph.nodeCount];

        for(int i = 0; i < graph.nodeCount; i++){
            int connectedNodes = fileScanner.nextInt();
            for(int j = 0; j < connectedNodes; j++){
                int location = fileScanner.nextInt();
                graph.table[i][location] = fileScanner.nextInt();
            }
        }
    }

    /**
     * Kyle Hinkle
     * Checks if graph is connected and print whether it is or not
     */
    public static void isConnected(){
        if(depthFirstSearch(graph)){
            System.out.println("Graph is connected.");
            System.out.println();
        } else{
            System.out.println("Graph is not connected");
            System.out.println();
        }
    }

    /**
     * Kyle Hinkle
     * Checks to see if the graph is connected.
     * @return
     */
    public static boolean depthFirstSearch(Graph testGraph){
        boolean[] connected = new boolean[testGraph.nodeCount];

        for(int i = 0; i < connected.length; i++){ // set connected to false
            connected[i] = false;
        }
        depthFirstSearch(testGraph, 0, connected);

         if(connectivityChecker(connected)){
             return false;
         }
        return true;
    }

    /**
     * Kyle Hinkle
     * Recursion helper for depthFirstSearch
     * @param testGraph
     * @param index
     * @param connected
     */
    public static void depthFirstSearch(Graph testGraph, int index, boolean[] connected){
        for(int i = 0; i < testGraph.nodeCount; i++){
            if(testGraph.table[index][i] != 0){
                if(!connected[i]){
                    connected[i] = true;
                    depthFirstSearch(testGraph, i, connected);
                }
            }
        }
    }

    /**
     * Kyle Hinkle
     * Prints out what the minimum Spanning Tree is
     */
    public static void minimumSpanningTree(){
        Graph shortestConnections = new Graph();
        shortestConnections.table = new int[graph.nodeCount][graph.nodeCount];
        shortestConnections.nodeCount = graph.nodeCount;
        int[] nodesConnected = new int[graph.nodeCount];

        for(int i = 0; i < graph.nodeCount; i++){
            int smallestColumn = 0;
            int smallestWeight = 0;
            for(int j = 0; j < graph.nodeCount; j++) {
                if(graph.table[i][j] != 0){
                    if(smallestColumn == 0 ) {
                        smallestColumn = j;
                        smallestWeight = graph.table[i][j];
                    }else if(graph.table[i][j] < smallestWeight){
                        smallestColumn = j;
                        smallestWeight = graph.table[i][j];
                    }
                }
            }
            if (smallestColumn == 0){ //Throws an error is it's not all connected.
                System.out.println("Error: Graph is not connected.");
                return;
            } else {
                shortestConnections.table[i][smallestColumn] = smallestWeight;
            }
            shortestConnections.table[smallestColumn][i] = smallestWeight;
        }
        MinimumSpanningTreePrint(shortestConnections, nodesConnected);
    }

    /**
     * Kyle Hinkle
     * Prints out the MinimumSpanningTree
     * @param shortestConnections
     * @param nodesConnected
     */
    private static void MinimumSpanningTreePrint(Graph shortestConnections, int[] nodesConnected) {
        System.out.println(graph.nodeCount);
        for(int i = 0; i < graph.nodeCount; i++){
            String connections = "";
            for(int j = 0; j < graph.nodeCount; j++){
                if(shortestConnections.table[i][j] != 0){
                    nodesConnected[i]++;
                    connections += j + " " + shortestConnections.table[i][j] + " ";
                }
            }
            System.out.print(nodesConnected[i] + " " + connections);
            System.out.println();
        }
        System.out.println();
        System.out.println();
    }

    public static void shortestPath(){

    }
    public static void isMetric(){
        if(depthFirstSearch(graph)){
            //Do math
            //print connection
        } else{
            System.out.println("Graph is not metric: Graph is not completely connected.");
            System.out.println();
        }
    }
    public static void makeMetric(){

    }

    /**
     * Kyle Hinkle
     * Does the Traveling Salesman Problem
     */
    public static void travelingSalesmanProblem(){
        if(depthFirstSearch(graph)){
            List<Graph> paths = new ArrayList<Graph>(); //initialize path
            Graph path = new Graph();
            path.table = new int[graph.nodeCount][graph.nodeCount];
            path.nodeCount++;
            path.order.add(0);
            paths.add(path);

            pathCalculator(path, paths, 0);// Find smallest Path
            //Get rid of unnecessary fluff
            Iterator<Graph> iterator = paths.iterator();
            while (iterator.hasNext()) {
                Graph temp = iterator.next();
                if(temp.nodeCount != graph.nodeCount
                        || temp.order.get(temp.order.size()-1) != 0){
                    iterator.remove();
                }
            }

            Graph smallestPath = calculateSmallestPath(paths);
            // Prints out smallest Graph
            if(smallestPath == null){
                    System.out.println("Error: Graph has no tour.");
                    System.out.println();
            } else {
                int tempPathWeight = 0;
                String pathOrder = "";
                for(int i = 0; i < graph.nodeCount; i++) {
                    for (int j = 0; j < graph.nodeCount; j++) {
                        tempPathWeight += smallestPath.table[i][j];
                    }
                }
                for(int i = 0; i < smallestPath.order.size(); i++){
                    if(i == smallestPath.order.size()-1){
                        pathOrder += smallestPath.order.get(i);
                    } else{
                        pathOrder += smallestPath.order.get(i) + " -> ";
                    }

                }
                System.out.print(tempPathWeight + ": ");
                System.out.println(pathOrder);
                System.out.println();
            }
        }else{
        System.out.println("Error: Graph is not connected.");
        System.out.println();
        }
    }

    /**
     * Kyle Hinkle
     * Finds the smallest path from a list of Graphs
     * @param paths
     * @return
     */
    private static Graph calculateSmallestPath(List<Graph> paths){
        Graph smallestGraph = null;
        int smallestPathWeight = -1;
        for (Graph path:paths) {
            if(path.nodeCount == graph.nodeCount
                    && path.order.get(path.order.size()-1) == 0){
                int tempPathWeight = 0;
                for(int i = 0; i < graph.nodeCount; i++){
                    for(int j = 0; j < graph.nodeCount; j++){
                        tempPathWeight += path.table[i][j];
                    }
                }

                if(tempPathWeight > 0 ){
                    if(smallestGraph == null || tempPathWeight < smallestPathWeight){
                        smallestGraph = path;
                        smallestPathWeight = tempPathWeight;
                    }
                }
            }
        }
        return smallestGraph;
    }

    /**
     * Kyle Hinkle
     * Calculates every path that exists.
     * @param currentGraph
     * @param paths
     * @param i
     */
    private static void pathCalculator(Graph currentGraph, List<Graph> paths, int i) {
        boolean newPath = false;
        Graph regretfulTable = null;
        if(currentGraph.nodeCount > 1 //This set is more accurate but runs out of memory with heap space error when there are too many.
                && currentGraph.order.get(currentGraph.order.size()-1) == 0){
            return;
        }
        for(int j = 0; j < graph.nodeCount; j++){
            if(graph.table[i][j] != 0
                    && graph.table[i][j] != currentGraph.table[i][j]
                    && graph.table[i][j] != currentGraph.table[j][i]) { //Checks for duplicate edge and whether the edge exists already
                if(!newPath){
                    //Create Regretful Table used in recursion
                    regretfulTable = new Graph();
                    regretfulTable.table = new int[graph.nodeCount][graph.nodeCount];
                    copyTable(currentGraph, regretfulTable);
                    copyOrder(currentGraph, regretfulTable);
                    regretfulTable.nodeCount = currentGraph.nodeCount;
                    //Continue onward to next node connected
                    currentGraph.table[i][j] = graph.table[i][j];
                    if(!currentGraph.order.contains(j)){
                        currentGraph.nodeCount++;
                    }
                    currentGraph.order.add(j);
                    pathCalculator(currentGraph, paths, j);
                    newPath = true;
                } else {
                    Graph newGraph = new Graph();
                    newGraph.table = new int[graph.nodeCount][graph.nodeCount];
                    copyTable(regretfulTable, newGraph);
                    //clearRow(newGraph, i);
                    newGraph.table[i][j] = graph.table[i][j];
                    newGraph.nodeCount = regretfulTable.nodeCount;
                    copyOrder(regretfulTable, newGraph);
                    if(!newGraph.order.contains(j)){
                        newGraph.nodeCount++;
                    }
                    newGraph.order.add(j);
                    paths.add(newGraph);
                    pathCalculator(newGraph, paths, j);
                }
            }
        }
    }
    /**
     * Kyle Hinkle
     * Copies the existing table into a new one
     * @param copyFrom
     * @param copyTo
     */
    private static void copyTable(Graph copyFrom, Graph copyTo) {
        for(int k = 0; k < graph.nodeCount; k++){
            for(int l = 0; l < graph.nodeCount; l++){
                copyTo.table[k][l] = copyFrom.table[k][l];
            }
        }
    }

    /**
     * Kyle Hinkle
     * Copies the existing order into a new order
     * @param copyFrom
     * @param copyTo
     */
    private static void copyOrder(Graph copyFrom, Graph copyTo) {
        for(int l = 0; l < copyFrom.order.size(); l++){
            copyTo.order.add(copyFrom.order.get(l));
        }
    }

    /**
     * Kyle Hinkle
     * Checks to see if path has landed on every node
     * @param connected
     * @return
     */
    private static boolean connectivityChecker(boolean[] connected) {
        for(int j = 0; j < connected.length; j++){ //checks to see if it landed on every node.
            if(connected[j] == false){
                return true;
            }
        }
        return false;
    }

    public static void approximateTSP(){

    }
}