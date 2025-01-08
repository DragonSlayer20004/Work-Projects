using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Diagnostics;

namespace SortingAlgorithm
{
    [TestClass]
    public class SortingTests
    {
        private int[] randomizedValues;

        [TestInitialize]
        public void InitializeTests()
        {
            //Generate Random and random values
            int amount = 100000;
            Random random = new Random();
            randomizedValues = new int[amount];

            //Populate randomizedValues;
            for (int i = 0; i < amount; i++)
            {
                randomizedValues[i] = random.Next();
            }
        }

        [DataTestMethod]
        [DataRow(100)]
        [DataRow(1000)]
        [DataRow(10000)]
        [DataRow(50000)]
        public void BubbleSortTest(int amount)
        {
            //Create Bubble Sort Array
            int[] bubblesortArray = new int[amount];
            Array.Copy(randomizedValues, 0, bubblesortArray, 0, amount);

            //Start Timer
            Stopwatch stopWatch = Stopwatch.StartNew();

            //Perform Bubble Sort
            bubblesortArray = SortingAlgorithms.BubbleSort(bubblesortArray);

            //End Timer
            stopWatch.Stop();

            //Check Bubble Sort
            Assert.IsTrue(SortingAlgorithms.CheckSort(bubblesortArray), "Bubble Sort took: " + stopWatch.Elapsed.TotalMilliseconds + " Millisecond. with " + amount + " numbers.\n");
        }

        [DataTestMethod]
        [DataRow(100)]
        [DataRow(1000)]
        [DataRow(10000)]
        [DataRow(50000)]
        public void InsertionSortTest(int amount)
        {
            //Create Insertion Sort Array
            int[] insertionsortArray = new int[amount];
            Array.Copy(randomizedValues, 0, insertionsortArray, 0, amount);

            //Start Timer
            Stopwatch stopWatch = Stopwatch.StartNew();

            //Perform Insertion Sort
            insertionsortArray = SortingAlgorithms.InsertionSort(insertionsortArray);

            //End Timer
            stopWatch.Stop();

            //Check Insertion Sort
            Assert.IsTrue(SortingAlgorithms.CheckSort(insertionsortArray), "Insertion Sort took: " + stopWatch.Elapsed.TotalMilliseconds + " Millisecond. with " + amount + " numbers.\n");
        }

        [DataTestMethod]
        [DataRow(100)]
        [DataRow(1000)]
        [DataRow(10000)]
        [DataRow(50000)]
        public void QuickSortTest(int amount)
        {
            //Create Insertion Sort Array
            int[] quickSortArray = new int[amount];
            Array.Copy(randomizedValues, 0, quickSortArray, 0, amount);

            //Start Timer
            Stopwatch stopWatch = Stopwatch.StartNew();

            //Perform Insertion Sort
            quickSortArray = SortingAlgorithms.QuickSort(quickSortArray, 0, quickSortArray.Length - 1);

            //End Timer
            stopWatch.Stop();

            //Check Insertion Sort
            Assert.IsTrue(SortingAlgorithms.CheckSort(quickSortArray), "Quick Sort took: " + stopWatch.Elapsed.TotalMilliseconds + " Millisecond. with " + amount + " numbers.\n");
        }

        [DataTestMethod]
        [DataRow(100)]
        [DataRow(1000)]
        [DataRow(10000)]
        [DataRow(50000)]
        public void HashSortTest(int amount)
        {
            //Create Hash Sort Array
            int[] hashSortArray = new int[amount];
            Array.Copy(randomizedValues, 0, hashSortArray, 0, amount);

            //Round all values of hashSortArray to be under 100000 for the sort to work.
            for (int i = 0; i < amount; i++)
            {
                hashSortArray[i] = hashSortArray[i] % 100000;
            }

            //Start Timer
            Stopwatch stopWatch = Stopwatch.StartNew();

            //Perform Hash Sort
            hashSortArray = SortingAlgorithms.HashSort(hashSortArray);

            //End Timer
            stopWatch.Stop();

            //Check Hash Sort
            Assert.IsTrue(SortingAlgorithms.CheckSort(hashSortArray), "Hash Sort took: " + stopWatch.Elapsed.TotalMilliseconds + " Millisecond. with " + amount + " numbers.\n");
        }
    }
}
