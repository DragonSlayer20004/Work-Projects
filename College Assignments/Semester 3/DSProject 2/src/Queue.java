import java.util.NoSuchElementException;

public class Queue<T> {
    private T[] data;
    private int size;
    private int start;

    public Queue() {
        size = 0;
        start = 0;
        data = (T[]) new Object[10];
    }

    /**
     * Kyle Hinkle
     * Takes the current queue and converts the whole thing into a string.
     * @return
     */
    @Override
    public String toString(){
        String result = "";
        for(int i = 0; i< this.size; i++){
            result+= data[(i+start)%data.length];
            result +=" ";
        }
        return result;
    }
    public void enqueue(T value) {
        if (size == data.length) {
            T[] biggerArray = (T[]) new Object[size*2];
            for (int i = 0; i < data.length - start; ++i) {
                biggerArray[i] = data [i+start];
            }
            data = biggerArray;
            start = 0;
        }
        data[(start + size) % data.length] = value;
        ++size;
    }


    public T dequeue() {
        if (size == 0) {
            throw new NoSuchElementException();
        }
        T removedValue = data[start];
        data[start] = null;
        if (start % (data.length - 1) == 0 && start != 0) {
            start = 0;
        }
        else {
            ++start;
        }
        --size;
        return removedValue;
    }


    public T front() {
        if (size == 0) {
            throw new NoSuchElementException();
        }
        return data[start];
    }


    public int size() {
        return size;
    }
}
