/**
 * Kyle Hinkle
 * 10/11/2023
 * Data Structures 2100
 * Assignment 4
 */

public class HashTable {
    private static class Node {
        public String key;
        public String value;
    }

    private Node[] table = new Node[101];
    private int size = 0;

    /**
     * Finds the hash of a string in this case the variable key
     * @param key
     * @return
     */
    private int hash(String key) {
        int code = (key.hashCode() & 0x7fffffff);//Make non negative
        code = code % table.length;
        return code;
    }

    /**
     * Finds the step size used when searching for an unused bucket
     * @param key
     * @return
     */
    private int step(String key) {
        int code = (key.hashCode() & 0x7fffffff);//Make non negative
        code = code%97;
        return code+1;
    }

    /**
     * Searches for the next prime closest to the new length
     * @param length
     * @return
     */
    private int nextPrime(int length) {
        int newLength = length*2;
        while(true){
            if(isPrime(newLength)){
                return length;
            }else {
                newLength++;
            }
        }
    }

    /**
     * Checks to see if the number is a prime number or not.
     * @param number
     * @return
     */
    private boolean isPrime(int number) {
        for(int i = 2; i*i <= number; i++){
            if(number % i == 0){
                return false;
            }
        }
        return true;
    }

    /**
     *Checks to see if the key exists in the table. Returns true if it does exist.
     * @param key
     * @return
     */
    public boolean containsKey(String key) {
        return get(key) != null;
    }

    /**
     * Puts in a new value based on the key. Increases in size if needed and returns whether or not there was already a key there.
     * @param key
     * @param value
     * @return
     */
    public boolean put( String key, String value ) {
        //Checks Load Factor
        if (.75 < (size / (double)table.length)){
            //if more than .75 increase table size by creating a new table and move the nodes over to it whilst adjusting for the new size
            Node[] newTable = new Node[nextPrime(table.length)];
            for(int i = 0; i <= table.length; i++){
                //If table contains a node it will transfer the node to the new table
                if(table[i] != null){
                    //Checks for an Existing Key
                    int location = hash(key);
                    while(true){
                        //If there is no node there it creates a new one
                        if(newTable[location] == null){
                            newTable[location] = new Node();
                            newTable[location].value = table[i].value;
                            newTable[location].key = table[i].key;
                            size++;
                            return true;
                            // If the key is equal to this node then it will replace its value
                        } else if(newTable[location].key.equals(table[i].key)){
                            newTable[location].value = table[i].value;
                            return false;
                            //If it is the wrong key it will search for a new location.
                        } else {
                            location += step(table[i].key);
                            location %= newTable.length;
                        }
                    }
                }
            }
            table = newTable;
        }

        //Checks for an Existing Key
        int location = hash(key);
        while(true){
            //If there is no node there it creates a new one
            if(table[location] == null){
                table[location] = new Node();
                table[location].value = value;
                table[location].key = key;
                size++;
                return true;
            // If the key is equal to this node then it will replace its value
            } else if(table[location].key.equals(key)){
                table[location].value = value;
                return false;
            //If it is the wrong key it will search for a new location.
            } else {
                location += step(key);
                location %= table.length;
            }
        }
    }

    /**
     * Returns the value using the key that is within the table
     * @param key
     * @return
     */
    public String get( String key ) {
        //Checks for an Existing Key
        int location = hash(key);
        while(true){
            if(table[location] == null){
                return null;
            } else if(table[location].key.equals(key)){
                return table[location].value;
            } else {
                location += step(key);
                location %= table.length;
            }
        }
    }
}