import java.io.PrintWriter;

public class WordTable {
    private WordTree[] wordTrees = new WordTree[26];

    public WordTable(){
        for(int i = 0; i< 26; i++) {
            wordTrees[i] = new WordTree();
        }
    }
    /**
     * Kyle Hinkle
     * Adds word to the appropriate WordTree, with the given score.
     * @param word
     * @param score
     */
    public void add(String word, int score) {
        wordTrees[word.charAt(0)-'a'].add(word, score);
     }

    /**
     * Kyle Hinkle
     * Gets the average score of a word from the appropriate WordTree.
     * @param word
     * @return
     */
    public double getScore(String word) {
        return wordTrees[word.charAt(0)-'a'].getScore(word);
    }

    /**
     * Kyle Hinkle
     * Asks the appropriate WordTree if it contains word.
     * @param word
     * @return
     */
    public boolean contains(String word) {
        return wordTrees[word.charAt(0)-'a'].contains(word);
    }

    /**
     * Kyle Hinkle
     * Loops through all 26 WordTree objects and print out their data.
     * @param reviewOut
     */
    public void print(PrintWriter reviewOut) {
        for(int i =0; i< 26; i++) {
            wordTrees[i].print(reviewOut);
        }
    }
}
