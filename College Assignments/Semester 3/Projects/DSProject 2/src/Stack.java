import java.util.NoSuchElementException;

//toString is your friend.

public class Stack<T> {
    private T[] data;
    private int size;

    public Stack() {
        size = 0;
        data = (T[]) new Object[10];
    }


    public void push(T value) {
        if (size == data.length) {
             T[] biggerArray = (T[]) new Object[size*2];
             for (int i = 0; i < data.length; ++i) {
                 biggerArray[i] = data [i];
             }
             data = biggerArray;
        }
        data[size] = value;
        ++size;
    }


    public T pop() {
        if (size == 0) {
            throw new NoSuchElementException();
        }
        T removedValue = data [size-1];
        data [size-1] = null;
        --size;
        return removedValue;
    }


    public T top() {
        if (size == 0) {
            throw new NoSuchElementException();
        }
        return data[size-1];
    }


    public int size() {
        return size;
    }
}
