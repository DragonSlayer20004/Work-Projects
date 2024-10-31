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
            Assert.IsTrue(SortingAlgorithms.CheckSort(insertionsortArray), "Bubble Sort took: " + stopWatch.Elapsed.TotalMilliseconds + " Millisecond. with " + amount + " numbers.\n");
        }
    }
}
