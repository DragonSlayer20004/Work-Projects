/*
 *Authors: Kyle Hinkle
 * Course: COMP 2100
 * Assignment: Assignment 7
 * Date: 11/16/2023
 */
import java.util.Arrays;
import java.util.Random;

public class Sorting {
    public static void main(String[] args){
        sortAll(10000);
        sortAll(100000);
        sortAll(1000000);
    }

    /**
     * Creates an array of the specified size and then runs 4 different types of sorts on it.
     * @param size
     */
    public static void sortAll(int size){
        int[] array = new int[size];
        Random random = new Random();
        for (int i = 0; i <array.length; ++i){
            array[i] = random.nextInt(0, 100000);
        }
        System.out.println("Array length: \t\t\t" + array.length);

        //Perform QuickSort
        int[] arrayCopy = Arrays.copyOf(array, array.length);
        long start = System.nanoTime();
        quickSort(arrayCopy, 0, array.length);
        long end = System.nanoTime();
        Boolean sorted = isSorted(arrayCopy);
        if(sorted){
            System.out.format("Quick Sort\t\t\t%.4f seconds\n", ((end-start)/1000000000.0));
        } else{
            System.out.println("Your QuickSort list is not sorted.");
        }

        //Perform Merge Sort
        arrayCopy = Arrays.copyOf(array, array.length);
        start = System.nanoTime();
        mergeSort(arrayCopy);
        end = System.nanoTime();
        sorted = isSorted(arrayCopy);
        if(sorted){
            System.out.format("Merge Sort\t\t\t%.4f seconds\n", ((end-start)/1000000000.0));
        } else{
            System.out.println("Your Merge list is not sorted.");
        }

        //Perform Heap Sort
        arrayCopy = Arrays.copyOf(array, array.length);
        start = System.nanoTime();
        heapSort(arrayCopy);
        end = System.nanoTime();
        sorted = isSorted(arrayCopy);
        if(sorted){
            System.out.format("Heap Sort\t\t\t%.4f seconds\n", ((end-start)/1000000000.0));
        } else{
            System.out.println("Your Heap list is not sorted.");
        }
        //Perform Radix Sort
        arrayCopy = Arrays.copyOf(array, array.length);
        start = System.nanoTime();
        radixSort(arrayCopy, 6);
        end = System.nanoTime();
        sorted = isSorted(arrayCopy);
        if(sorted){
            System.out.format("Radix Sort\t\t\t%.4f seconds\n", ((end-start)/1000000000.0));
        } else{
            System.out.println("Your Radix list is not sorted.\n");
        }
        System.out.println();
    }

    /**
     * Checks to see if the array is sorted.
     * @param array
     * @return
     */
    public static boolean isSorted(int[] array){
        for(int i = 1; i < array.length-1; i++){
            if(array[i]>array[i+1])
                return false;
        }
        return true;
    }

    /**
     * Insertion sort for when it comes in handy.
     * @param array
     */
    public static void insertionSort(int[] array){
        int n = array.length;
        for (int i = 1; i < n; ++i) {
            int key = array[i];
            int j = i - 1;

            while (j >= 0 && array[j] > key) {
                array[j + 1] = array[j];
                j = j - 1;
            }
            array[j + 1] = key;
        }
    }

    /**
     * Finds a pivot and adjusts the array around it.
     * @param array
     * @param index
     * @param left
     * @param right
     * @return
     */
    public static int partition(int[] array, int index, int left, int right){
        int pivot = array[index];
        int temp = array[right];
        array[right] = array[index];
        array[index] = temp;
        index = left;
        for(int i = left; i < right; i++){
            if(array[i] <= pivot){
                temp = array[i];
                array[i] = array[index];
                array[index] = temp;
                index++;
            }
        }
        temp = array[right];
        array[right] = array[index];
        array[index] = temp;
        return index;
    }

    /**
     * Initial quicksort method that goes through the array splitting it
     * @param array
     * @param start
     * @param end
     */
    public static void quickSort(int[] array, int start, int end){
        if(start < end-1){
            int middle = partition(array, start, start, end-1);
            quickSort(array, start, middle);
            quickSort(array, middle+1, end);
        }
    }

    /**
     * Initial method called that then sets the parameters for the sorting.
     * @param array
     */
    private static void mergeSort(int[] array){
        int[] Scratch = new int[array.length];
        mergeSort(array, Scratch, 0, array.length);
    }

    /**
     * Main merge sort method that splits the array in half and then merges it all together.
     * @param values
     * @param scratch
     * @param start
     * @param end
     */
    private static void mergeSort(int[] values, int[] scratch, int start, int end){
        if(start < end-1){
            int midPoint = (start+end)/2;
            mergeSort(values, scratch, start, midPoint);
            mergeSort(values, scratch, midPoint, end);
            merge(values, scratch, start, midPoint, end);
        }
    }

    /**
     * Combines together scratch and values
     * @param values
     * @param scratch
     * @param start
     * @param midPoint
     * @param end
     */
    public static void merge(int[] values, int[] scratch, int start, int midPoint, int end){
        for(int i = start; i < end; ++i){
            scratch[i] = values[i];
        }
        int index1 = start;
        int index2 = midPoint;
        for(int i = start; i < end; ++i){
            if(index1 >= midPoint){
                values[i] = scratch[index2];
                index2++;
            } else if(index2 >= end){
                values[i] = scratch[index1];
                index1++;
            } else if(scratch[index1] <= scratch[index2]){
                values[i] = scratch[index1];
                index1++;
            } else{
                values[i] = scratch[index2];
                index2++;
            }
        }
    }

    /**
     * Main Heap Sort method
     * Takes in an array and sorts it
     * @param array
     */
    public static void heapSort(int[] array){
        int size = array.length;

        //Rearrange Array
        for(int i = size/2 - 1; i >= 0; i--){
            bubbleDown(array, size, i);
        }

        //Extract elements from heap
        for(int i = size - 1; i >= 0; i--) {
            int temp = array[0];
            array[0] = array[i];
            array[i] = temp;

            bubbleDown(array, i, 0);
        }
    }

    /**
     * Moves stuff around so its in order
     * @param keys
     * @param size
     * @param index
     */
    public static void bubbleDown(int[] keys,  int size, int index){
        int largestIndex = index;
        //check if left is larger
        if(2*index+1 < size && keys[2*index+1] > keys[largestIndex]) {
            largestIndex = 2*index+1;
        }
        //check if right is larger
        if(2*index+2 < size && keys[2*index+2] > keys[largestIndex]){
            largestIndex = 2*index+2;
        }

        //If largest is not the root.
        if(largestIndex != index){
            int temp = keys[index];
            keys[index] = keys[largestIndex];
            keys[largestIndex] = temp;

            bubbleDown(keys, size, largestIndex);
        }
    }

    /**
     * Takes in an array and digits and sorts it all is a digit place repeating for all digits.
     * @param array
     * @param digits
     */
    public static void radixSort(int[] array, int digits) {
        int length = array.length;
        //Max length is 10 raised to digits.
        for (int place = 1; Math.pow(10,digits)/place >0; place*=10) {
            int[] output = new int[length];
            int[] count = new int[10];
            int i;
            //Fill count with 0
            Arrays.fill(count, 0);

            // Store numbers in specific digit place
            for (i = 0; i < length; i++){
                int numberSpot = ((array[i] / place) % 10);
                count[numberSpot]++;
            }

            // Change count[i] so that count[i] now contains
            // actual position of this digit in output[]
            for (i = 1; i < 10; i++) {
                count[i] += count[i - 1];
            }

            // Place the elements in sorted order
            for (i = length - 1; i >= 0; i--) {
                output[count[(array[i] / place) % 10] - 1] = array[i];
                count[((array[i] / place) % 10)]--;
            }

            //Copy output to array
            for (i = 0; i < length; i++) {
                array[i] = output[i];
            }
        }
    }
}
