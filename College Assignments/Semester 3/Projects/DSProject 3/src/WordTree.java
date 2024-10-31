import java.io.PrintWriter;

/**
 * Kyle Hinkle
 * Red Black word tree
 */
public class WordTree {
    private Node root;
    private static class Node {
        public String word;
        public double score;
        public int count;
        public boolean black;
        public Node left;
        public Node right;
        public Node parent;
    }

    /**
     * Kyle Hinkle
     * Adds a String to a WordTree with a given score.
     * If word is already contained in the WordTree, this method should add the supplied score to the score of the given Node and increment its count.
     * @param word
     * @param score
     */
    public void add(String word, int score){
        //Base Case
        if(root == null){
            Node newNode = new Node();
            newNode.word = word;
            newNode.score = score;
            newNode.black = true;
            newNode.count++;
            root = newNode;
        } else{ //Search to find the node's correct place
            Node currentNode = root;
            boolean left = false;
            Node potentialParent = null;
            while(currentNode!=null){
                if (word.compareTo(currentNode.word) == 0) {
                    currentNode.score += score;
                    currentNode.count++;
                    return;
                }
                if (word.compareTo(currentNode.word) < 0) {
                    potentialParent = currentNode;
                    currentNode = currentNode.left;
                    left = true;
                } else {
                    potentialParent = currentNode;
                    currentNode = currentNode.right;
                    left = false;
                }
            }
            //Assign Parents and Siblings
            Node newNode = new Node();
            newNode.word = word;
            newNode.score = score;
            newNode.black = true;
            newNode.count++;
            newNode.parent = potentialParent;
            if(left){
                potentialParent.left = newNode;
            } else {
                potentialParent.right = newNode;
            }
            fixTree(newNode);
        }
    }

    /**
     * Kyle Hinkle
     * Balances the tree after adding in a new node.
     * @param node
     */
    private void fixTree(Node node){
        while (!node.parent.black && node != root){
            if(node.parent == node.parent.parent.left){
                Node uncle = node.parent.parent.right;
                if(!uncle.black){
                    node.parent.black = true;
                    uncle.black = true;
                    node.parent.parent.black = false;
                    node = node.parent.parent;
                } else{
                    if(node == node.parent.right){
                        node = node.parent;
                        leftRotate(node);
                    }
                    node.parent.black = true;
                    node.parent.parent.black = false;
                    rightRotate(node.parent.parent);
                }
            }else{
                Node uncle = node.parent.parent.left;
                if(!uncle.black){
                    node.parent.black = true;
                    uncle.black = true;
                    node.parent.parent.black = false;
                    node = node.parent.parent;
                }else{
                    if (node == node.parent.left){
                        node = node.parent;
                        rightRotate(node);
                    }
                    node.parent.black = true;
                    node.parent.parent.black = false;
                    leftRotate(node.parent.parent);
                }
            }
        }
        root.black = true;
    }

    /**
     * Kyle Hinkle
     * Rotates the tree left to make it balanced
     * @param node
     */
    private void leftRotate(Node node){
        Node sibling = node.right;
        node.right = sibling.left;
        //Turn sibling's left subtree into node's right subtree
        if (sibling.left != null){
            sibling.left.parent = node;
        }
        sibling.parent = node.parent;
        if (node.parent == null){
            root = sibling;
        }else{
            if(node == node.parent.left){
                node.parent.left = sibling;
            } else{
                node.parent.right = sibling;
            }
            sibling.left = node;
            node.parent = sibling;
        }
    }

    /**
     * Kyle Hinkle
     * Rotates the tree to the right to make it balanced
     * @param node
     */
    private void rightRotate(Node node){
        Node sibling = node.left;
        node.left = sibling.right;
        //Turn sibling's left subtree into node's right subtree
        if (sibling.right != null){
            sibling.right.parent = node;
        }
        sibling.parent = node.parent;
        if (node.parent == null){
            root = sibling;
        }else{
            if(node == node.parent.right){
                node.parent.right = sibling;
            } else{
                node.parent.left = sibling;
            }
            sibling.right = node;
            node.parent = sibling;
        }
    }

    /**
     * Kyle Hinkle
     * Returns true if word has already been added to the WordTree and false otherwise.
     * @param word
     * @return
     */
    public boolean contains(String word){
        Node currentNode = root;

        while (currentNode != null){
            if (word.compareTo(currentNode.word) == 0) {
                return true;
            }
            if (word.compareTo(currentNode.word) < 0) {
                currentNode = currentNode.left;
            } else {
                currentNode = currentNode.right;
            }
        }
        return false;
    }

    /**
     * Kyle Hinkle
     * Searches for word in the tree. If found, it will return the floating-point quotient of the corresponding Node's score and count.
     * If the word is not found, it will return 2.0, a neutral score.
     * @param word
     * @return
     */
    public double getScore(String word){
        Node currentNode = root;
        while (currentNode != null){
            if (word.compareTo(currentNode.word) == 0) {
                return currentNode.score/ currentNode.count;
            }
            if (word.compareTo(currentNode.word) < 0) {
                currentNode = currentNode.left;
            } else {
                currentNode = currentNode.right;
            }
        }
        return 2.0;
    }

    /**
     * Kyle Hinkle
     * Passes in the root
     * @param reviewOut
     */
    public void print(PrintWriter reviewOut){
        print(reviewOut, root);
    }
    /**
     * Kyle Hinkle
     * This method prints out each word in the WordTree in alphabetical order, followed by a tab, followed by its total score, followed by a tab, followed by its count, followed by a newline.
     * It makes sense to adapt an inorder traversal to this task.
     * @param reviewOut
     */
    public void print(PrintWriter reviewOut, Node node){
        if(node != null) {
            print(reviewOut, node.left);
            reviewOut.println(node.word +"\t" + (int)node.score + "\t" + node.count);
            print(reviewOut, node.right);
        }
    }
}
