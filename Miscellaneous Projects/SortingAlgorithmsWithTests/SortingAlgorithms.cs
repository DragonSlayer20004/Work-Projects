using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Linq;

namespace SortingAlgorithm
{
    public class SortingAlgorithms
    {
        public static void Main()
        {
            SortByAmount(100);
            SortByAmount(1000);
            SortByAmount(10000);
            //SortByAmount(100000);

            Console.ReadLine();
        }

        public static void SortByAmount(int amount)
        {
            //Generate Random and random values
            Random random = new Random();
            int[] randomizedValues = new int[amount];

            //Populate randomizedValues;
            for (int i = 0; i < amount; i++)
            {
                randomizedValues[i] = random.Next();
            }

            Console.WriteLine("Performing sorting tests with " + amount + " integers.\n");


            Console.WriteLine("Executing Bubble Sort");

            //Create Bubble Sort Array
            int[] bubblesortArray = new int[amount];
            Array.Copy(randomizedValues, 0, bubblesortArray, 0, amount);

            //Start Timer
            Stopwatch stopWatch = Stopwatch.StartNew();

            //Perform Bubble Sort
            bubblesortArray = BubbleSort(bubblesortArray);

            //End Timer
            stopWatch.Stop();


            //Check Bubble Sort
            if (CheckSort(bubblesortArray))
            {
                Console.WriteLine("Bubble Sort took: " + stopWatch.Elapsed.TotalMilliseconds + " Millisecond.\n");
            }
            else
            {
                Console.WriteLine("Bubble Sort failed to sort.\n");
            }

            Console.WriteLine("Executing Insertion Sort");

            //Create Insertion Sort Array
            int[] insertionsortArray = new int[amount];
            Array.Copy(randomizedValues, 0, insertionsortArray, 0, amount);

            //Start Timer
            stopWatch.Restart();

            //Perform Insertion Sort
            insertionsortArray = InsertionSort(insertionsortArray);

            //End Timer
            stopWatch.Stop();


            //Check Insertion Sort
            if (CheckSort(insertionsortArray))
            {
                Console.WriteLine("Insertion Sort took: " + stopWatch.Elapsed.TotalMilliseconds + " Millisecond.\n");
            }
            else
            {
                Console.WriteLine("Insertion Sort failed to sort.\n");
            }

            Console.WriteLine("Executing QuickSort Sort");

            //Create Insertion Sort Array
            int[] quickSortArray = new int[amount];
            Array.Copy(randomizedValues, 0, quickSortArray, 0, amount);

            //Start Timer
            stopWatch.Restart();

            //Perform Insertion Sort
            quickSortArray = QuickSort(quickSortArray, 0, quickSortArray.Length - 1);

            //End Timer
            stopWatch.Stop();


            //Check Insertion Sort
            if (CheckSort(quickSortArray))
            {
                Console.WriteLine("QuickSort Sort took: " + stopWatch.Elapsed.TotalMilliseconds + " Millisecond.\n");
            }
            else
            {
                Console.WriteLine("QuickSort Sort failed to sort.\n");
            }
        }

        /*
         * Simple loop to ensure that all numbers are numbered numerically from highest to lowest.
         */
        public static bool CheckSort(int[] sortedArray)
        {
            for (int i = 0; i < sortedArray.Count() - 1; i++)
            {
                if (sortedArray[i] < sortedArray[i + 1])
                {
                    return false;
                }
            }
            return true;
        }

        /*
         * Swaps two values inside an array
         */
        public static int[] Swap(int[] array1, int location1, int location2)
        {
            int temp = array1[location1];
            array1[location1] = array1[location2];
            array1[location2] = temp;
            return array1;
        }

        /*
         *  BubbleSort Method that moves numbers one position at a time.
         */
        public static int[] BubbleSort(int[] bubblesortArray)
        {
            for (int i = 0; i < bubblesortArray.Length - 1; i++)
            {
                bool swapped = false;
                for (int j = 0; j < bubblesortArray.Length - 1 - i; j++)
                {
                    if (bubblesortArray[j] < bubblesortArray[j + 1])
                    {
                        bubblesortArray = Swap(bubblesortArray, j, j + 1);
                        swapped = true; //No need to continue searching once a swap was completed
                    }
                }
                if (!swapped)
                {
                    break;
                }
            }
            return bubblesortArray;
        }

        /*
         * InsertionSort Method that locates a key then moves all the numbers that are less than it to the right until it finds a number 
         * bigger than it in which case it will stop and fill in the hole with the key.
         */
        public static int[] InsertionSort(int[] insertionsortArray)
        {
            for (int i = 0; i < insertionsortArray.Length; i++)
            {
                int key = insertionsortArray[i];
                int j = i - 1;

                while (j >= 0 && insertionsortArray[j] < key)
                {
                    insertionsortArray[j + 1] = insertionsortArray[j];
                    j -= 1;
                }
                insertionsortArray[j + 1] = key;
            }

            return insertionsortArray;
        }

        /*
         *  QuickSort chooses a pivot point and splits the away apart into smaller segments that are quicker to organize.
         */
        public static int[] QuickSort(int[] quickSortArray, int start, int end)
        {
            if (start < end)
            {
                int pivot = Partition(quickSortArray, start, end); //Find initial center pivot
                QuickSort(quickSortArray, start, pivot - 1); //Sort left side
                QuickSort(quickSortArray, pivot + 1, end); //Sort right side
            }
            return quickSortArray;
        }

        /*
         * Helper method of QuickSort. Aids in breaking the array into segments and then sorting them
         */
        public static int Partition(int[] quickSortArray, int left, int right)
        {
            int pivotValue = quickSortArray[right]; // Use the rightmost element as the pivot
            int pivotIndex = left; // Pointer for greater element

            // Loop through moving elements to the left so larger numbers are on the right
            for (int i = left; i < right; i++)
            {
                if (quickSortArray[i] >= pivotValue)
                {
                    int tempValue = quickSortArray[i];
                    quickSortArray[i] = quickSortArray[pivotIndex];
                    quickSortArray[pivotIndex] = tempValue;
                    pivotIndex++;
                }
            }

            //Swap the pivot back to its original position
            int temp = quickSortArray[pivotIndex];
            quickSortArray[pivotIndex] = quickSortArray[right];
            quickSortArray[right] = temp;
            return pivotIndex;
        }
    }
}