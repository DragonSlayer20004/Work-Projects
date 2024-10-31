public class NewList {
    private String[] array = new String[10];
    private int size = 0;

    public NewList(){
         array = new String[10];
         size = 0;
         clear();
    }
     void add(String element){
         for(int i = 0; array.length > i; i++){
             if(array[i] == (null)){
                 array[i] = element;
                 size++;
                 return;
             }
         }
         size++;
         String[] tempArray = new String[size];

         for(int i = 0; array.length > i; i++){
            tempArray[i] = array[i];
         }
         tempArray[size] = element;
         array = tempArray;
    }
     void add(int index, String element){
         String[] tempArray;
         tempArray = array.clone();

         clear();
         for(int i = 0; tempArray.length > i; i++) {
             if(i==index){
                 add(element);
                 size++;
             }
             add(tempArray[i]);
         }
    }
     void addAll(NewList list){
         for(int i = 0; list.size > i; i++){
             add(list.get(i));
         }
    }
     void clear(){
         for(int i = 0; array.length > i; i++){
             array[i] = null;
             size = 0;
         }
    }
     boolean contains(String element){
         for(int i = 0; size > i; i++){
             if(array[i] ==(element)){
                 return true;
             }
         }
         return false;
    }
     String get(int index) {
         try{
             if(array[index] == (null)){
                 throw new IndexOutOfBoundsException("Out of bounds input: "+ index);
             }
             return array[index];
         }catch (IndexOutOfBoundsException e){
            throw new IndexOutOfBoundsException("Out of bounds input: "+ index);
         }
     }
    int indexOf(String element){
        for(int i = 0; size > i; i++){
            if(array[i].equals(element)){
                return i;
            }
        }
        return -1;
    }
    boolean isEmpty(){
        for(int i = 0; array.length > i; i++){
            if(!(array[i] == (null))){
                return false;
            }
        }
        return true;
    }
     int lastIndexOf(String element){
         int lastinstance = -1;
         for(int i = 0; size > i; i++){
             if(array[i].equals(element)){
                 lastinstance = i;
             }
         }
         return lastinstance;
    }
     String remove(int index){
         String[] tempArray;
         String removedElement ="null";
         tempArray = array.clone();

         /*
         clear is clearing tempArray as well. Fix that and program should work.
          */

         clear();
         int j = 0;
         for(int i = 0; tempArray.length > j; i++) {
             if(i==index){
                 removedElement = tempArray[index];
                 j++;
                 size--;
             }
             add(tempArray[j]);
             j++;
         }
         return removedElement;
    }
    void set(int index, String element){
        try{
            if(array[index] == (null)){
                throw new IndexOutOfBoundsException("Out of bounds input: "+ index);
            }
            array[index] = element;
        }catch (IndexOutOfBoundsException e){
            throw new IndexOutOfBoundsException("Out of bounds input: "+ index);
        }
         array[index] = element;
    }
    int size(){
         return size;
    }
}

