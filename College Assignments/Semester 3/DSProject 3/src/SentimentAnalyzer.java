/*
 *Authors: Kyle Hinkle, Ibrahim Hashi
 * Course: COMP 2100
 * Assignment: Project 3
 * Date: 10/17/2023
 */

import java.io.*;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.LinkedList;
import java.util.Scanner;

public class SentimentAnalyzer {
    private static String reviewFileName;
    /**
     * Kyle Hinkle, Abraham Hashi
     * main methods that connects everything together.
     * @param args
     */
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        //Reads in the files and attempts to put all the words in the files into trees.
        WordTable stopWordTable = null;
        WordTable reviewTable = null;
        boolean success = false;
        //User input file name and if that file doesn't exist requests them to try again.
        while(!success) {
            try {
                System.out.print("Enter a stop word file: ");
                reviewFileName = scanner.nextLine();
                Scanner stopScanner = new Scanner(new File(reviewFileName));
                //Reads in all the words in the stopWordFile
                stopWordTable = putStopWordFileIntoTree(stopScanner);
                success = true;
            } catch (IOException e) {
                System.out.println("Invalid File: " + reviewFileName + " Please try again");
            }
        }
        success = false;
        //User input file name and if that file doesn't exist requests them to try again.
        while(!success) {
            try {
                System.out.print("Enter a review file: ");
                reviewFileName = scanner.nextLine();
                Scanner reviewScanner = new Scanner(new File(reviewFileName));
                //Reads in all the words in review file and compares them with the words in the stopwordfile tree
                reviewTable = putReviewFileIntoTree(reviewScanner, stopWordTable);
                success = true;
            } catch (IOException e) {
                System.out.println("Invalid File: " + reviewFileName + " Please try again");
            }
        }

        //listing the statistics (total rating and count) for each (non-stop) word in the reviews.
        try {
            PrintWriter reviewOut = new PrintWriter(reviewFileName.substring(0, reviewFileName.length()-3)+"word.txt");
            reviewTable.print(reviewOut);
            reviewOut.close();
            System.out.println("Word statistics written to file reviews.txt.words");
            System.out.println();
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
        //Prompt the user for phrases and rate them.
        String userPhrase = "";
        LinkedList userPhraseParsed;
        while (!userPhrase.equalsIgnoreCase("no")) {
            System.out.print("Would you like to analyze a phrase? (yes/no) ");
            userPhrase = scanner.nextLine().toLowerCase();
            if (!userPhrase.equalsIgnoreCase("yes") && !userPhrase.equalsIgnoreCase("no")) {
                System.out.println("Please Try again.");
            }
            if (userPhrase.equalsIgnoreCase("yes")) {
                NumberFormat formatter = new DecimalFormat("#0.000");
                System.out.print("Enter Phrase: ");
                userPhrase = scanner.nextLine();
                userPhraseParsed = userChoiceArray(userPhrase, stopWordTable);
                double score = scoreAverage(reviewTable, userPhraseParsed);
                if (score > 2) {
                    System.out.println("Score: " + formatter.format(score));
                    System.out.println("Your Phrase was positive");
                } else if (score == 0) {
                    System.out.println("Your phrase contained no words that affect sentiment.");
                }else if (score == 2) {
                    System.out.println("Score: " + formatter.format(score));
                    System.out.println("Your Phrase was perfectly neutral");
                } else {
                    System.out.println("Score: " + formatter.format(score));
                    System.out.println("Your Phrase was negative");
                }
                userPhrase = "";
            }
        }
    }
    /**
     * Kyle Hinkle, Abrahim Hashi
     * Puts all the words in the review file into the tree.
     * Compares the words with the words in the stopwordfile tree and if they match do not add it in.
     * Requires parsing of the text in order for the tree to work
     * (Currently not working. Need to change from reading lines into tree to reading just the words into the tree)
     * @param reviewFileReader
     * @return reviewTree
     * @throws IOException
     */
    private static WordTable putReviewFileIntoTree(Scanner reviewFileReader, WordTable stopWordFileTree) throws IOException {
        WordTable reviewTree = new WordTable();
        //Goes through all the lines in the stopWordFile
        while (reviewFileReader.hasNext()) {
            int score = reviewFileReader.nextInt();
            String line = reviewFileReader.nextLine().toLowerCase();
            String word = "";

            for (int i = 0; i < line.length(); i++) {
                if (Character.isLetter(line.charAt(i)) || line.charAt(i) == '-' || line.charAt(i) == '\'') {
                  word+= line.charAt(i);
                }
                else {
                    word = clean(word);
                    if(!word.isEmpty() && !stopWordFileTree.contains(word)) {
                        reviewTree.add(word,score);
                    }
                    word = "";
                }
            }
            word = clean(word);
            if(!word.isEmpty() && !stopWordFileTree.contains(word)) {
                reviewTree.add(word,score);
            }
        }
        return reviewTree;
    }

    /**
     * Abraham Hashi
     * Cleans up the word by getting rid of unnecessary special characters.
     * @param remove
     * @return
     */
    private static String clean(String remove) {
     while (!remove.isEmpty() && (remove.charAt(0) == '-' || remove.charAt(0) == '\'')) {
         remove = remove.substring(1);
     }
        while (!remove.isEmpty() && (remove.charAt(remove.length() - 1) == '-' || remove.charAt(remove.length() - 1) == '\'')) {
            remove = remove.substring(0, remove.length() - 1);
        }
        return remove;
    }

    /**
     * Kyle Hinkle
     * Puts all the lines from the wordStop file into a tree
     * No Parsing neccessary for the stopWordTree
     * @param stopWordFileReader
     * @return stopWordTree
     * @throws IOException
     */
    private static WordTable putStopWordFileIntoTree(Scanner stopWordFileReader) throws IOException {
        WordTable stopWordTable = new WordTable();
        //Goes through all the lines in the stopWordFile
        while(stopWordFileReader.hasNext()){
            //Put currentLine in tree based on lines
            stopWordTable.add(stopWordFileReader.next(), 2);
        }
        return stopWordTable;
    }

    /**
     * Kyle Hinkle
     * Behaves very similarly to PutReviewFileIntoTree
     * @Params userPhrase
     * @return
     */
    private static LinkedList userChoiceArray(String userPhrase, WordTable stopWordTable){
        LinkedList userChoiceArray = new LinkedList();
        String line = userPhrase.toLowerCase();
        String word = "";
        for (int i = 0; i < line.length(); i++) {
            if (Character.isLetter(line.charAt(i)) || line.charAt(i) == '-' || line.charAt(i) == '\'') {
                word+= line.charAt(i);
            }
            else {
                word = clean(word);
                if(!word.isEmpty() && !stopWordTable.contains(word)) {
                    userChoiceArray.add(word);
                }
                word = "";
            }
        }
        word = clean(word);
        if(!word.isEmpty() && !stopWordTable.contains(word)) {
            userChoiceArray.add(word);
        }
        return userChoiceArray;
    }

    /**
     * Kyle Hinkle
     * Gets the average of word scores within a tree and returns it.
     * @param phrase
     * @return
     */
    private static double scoreAverage(WordTable reviewTable, LinkedList phrase){
        double average = 0;
        int count = 0;

        for(int i = 0; i < phrase.size(); i++){
            if (reviewTable.getScore((String) phrase.get(i)) == -1) {
                //Do Nothing
            } else {
                average += reviewTable.getScore((String)phrase.get(i));
                count++;
            }
        }
        if(count == 0){
            return 0;
        }else{
            return average/count;
        }
    }
}
