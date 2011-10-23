/* ========================================================================== 
 *	PROGRAM Sorting Algorithms
 *
 *		AUTHOR: Steven Burgart
 *		FSU MAIL ID: skb08c@my.fsu.edu
 *		COP4531 - Fall 2011
 *		PROJECT NUMBER: 1
 *		DUE DATE: 10/28/11
 *
 *	Description:	This program implements and benchmarks the following
 *              sorting algorithms:
 * 
 *              Insertion Sort, Merge Sort, Heap Sort,
 *              Quick Sort, Counting Sort, Bucket Sort,
 *              Radix Sort
 *
 * ========================================================================== */

#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/time.h>
using namespace std;

// Sort function typedef
typedef void (*SortFunction)(int*, int);

// Function prototypes - documentation in declarations
void SingleTest(SortFunction, int, int);
void RunBenchmarks();
void PrintArray(int*, int);
int *RandomArray(int, int);
int *CopyArray(int*, int);
void PrintTimeDiff(timeval, timeval);
void InsertionSort(int*, int);
void Merge(int*, int, int, int);
void RecursiveMergeSort(int*, int, int);
void MergeSort(int*, int);
void HeapifyElement(int*, int, int);
void Heapify(int*, int);
void HeapSort(int*, int);
void RecursiveQuickSort(int*, int, int);
void QuickSort(int*, int);
int MaxValue(int*, int);
void CountingSort(int*, int);
int NumDigits(int);
int DigitValue(int, int);
void CountingSort(int*, int, int);
void RadixSort(int*, int);
void BucketSort(int*, int);

// ========================================================================== //
// Method Name - Main
//      Simply run the benchmarks defined below
// ========================================================================== //
int main() {
    RunBenchmarks();

    return 0;
}

// ========================================================================== //
// Method Name - SingleTest
//      For debugging purposes, quickly tests a single sort function
// ========================================================================== //
void SingleTest(SortFunction func, int size, int maxKey) {
    int *test = RandomArray(size, maxKey);

    PrintArray(test, size);
    (*func)(test, size);
    PrintArray(test, size);

    delete[] test;
}

// ========================================================================== //
// Method Name - RunBenchmarks
//      Run a series of benchmarks based on the constant parameters declared
//      at the beginning of the function. 
// ========================================================================== //
void RunBenchmarks() {
    //const int NUM_SORT_FUNCS = 7;
    const int NUM_SORT_FUNCS = 5;               // Number of sort functions
    const int MAX_KEY_VALUE = 50000;            // Max value of any element
    const int NUM_TESTS = 11;                   // Number of separate tests
    const int NUM_SAMPLES = 10;                 // Number of samples per test
    const int TEST_SIZES[] = {10, 50, 100,      // The size of each array
        500, 1000, 5000,
        10000, 50000, 100000,
        500000, 1000000};

    /*
    const string sortFuncNames[] = {"Insertion Sort", "Merge Sort", "Heap Sort",
                              "Quick Sort", "Counting Sort", "Radix Sort", 
                              "Bucket Sort"};
    
    const SortFunction sortFuncs[] = {InsertionSort, MergeSort, HeapSort,
                            QuickSort, CountingSort, RadixSort, 
                            BucketSort};
     */

    const string sortFuncNames[] = {"Merge Sort", "Heap Sort",
        "Quick Sort", "Counting Sort", "Radix Sort"};

    const SortFunction sortFuncs[] = {MergeSort, HeapSort,
        QuickSort, CountingSort, RadixSort};

    timeval start, end;
    int *tmpArray;

    for (int i = 0; i < NUM_TESTS; ++i) {
        int N = TEST_SIZES[i];
        cout << "N=" << N << endl;

        // Generate random samples
        int **randArrays = new int*[NUM_SAMPLES];
        for (int i = 0; i < NUM_SAMPLES; ++i)
            randArrays[i] = RandomArray(N, MAX_KEY_VALUE);

        // Benchmark the function
        for (int j = 0; j < NUM_SORT_FUNCS; ++j) {
            cout << sortFuncNames[j];
            for (int k = 0; k < NUM_SAMPLES; ++k) {
                tmpArray = CopyArray(randArrays[k], N);
                gettimeofday(&start, NULL);
                (*sortFuncs[j])(tmpArray, N);
                gettimeofday(&end, NULL);
                PrintTimeDiff(start, end);
                delete[] tmpArray;
            }
            cout << endl;
        }
        cout << endl;

        for (int j = 0; j < NUM_SAMPLES; ++j)
            delete[] randArrays[j];
        delete[] randArrays;
    }
}

// ========================================================================== //
// Method Name - PrintArray
//      Simple method to print an array.
// ========================================================================== //
void PrintArray(int *ary, int size) {
    for (int i = 0; i < size; ++i)
        cout << ary[i] << " ";
    cout << endl;
}

// ========================================================================== //
// Method Name - RandomArray
//      Returns and integer array based on the size and max key value.
// ========================================================================== //
int *RandomArray(int size, int maxKeyValue) {
    int *randArray = new int[size];
    timeval timeseed;
    gettimeofday(&timeseed, NULL);

    srand(timeseed.tv_usec);
    for (int i = 0; i < size; ++i)
        randArray[i] = (rand() % maxKeyValue) + 1;

    return randArray;
}

// ========================================================================== //
// Method Name - CopyArray
//      Returns a deep copy of an array.
// ========================================================================== //
int *CopyArray(int *ary, int size) {
    int *tmp = new int[size];

    for (int i = 0; i < size; ++i)
        tmp[i] = ary[i];

    return tmp;
}

// ========================================================================== //
// Method Name - PrintTimeDiff
//      Prints the time difference, in microseconds, of two time parameters.
// ========================================================================== //
void PrintTimeDiff(timeval start, timeval end) {
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    long utime = ((seconds * 1000000) + useconds);

    cout << "\t" << utime;
}

// ========================================================================== //
// Method Name - InsertionSort
//      Implementation of the insertion sort algorithm for integers.
// ========================================================================== //
void InsertionSort(int *ary, int size) {
    for (int i = 1; i < size; ++i) {
        int key = ary[i];
        for (int j = i - 1; j >= 0 && ary[j] > key; --j) {
            ary[j + 1] = ary[j];
            ary[j] = key;
        }
    }
}

// ========================================================================== //
// Method Name - Merge
//      Merge a range from an array - used in the merge sort implementation.
// ========================================================================== //
void Merge(int *ary, int start, int mid, int end) {
    // Necessary variables
    int tmpIndex = 0,
            rightStart = mid + 1,
            rightEnd = end,
            leftEnd = mid;

    // Temporary array
    int *tmp = new int[end - start + 1];

    // Append max until one half is out of elements
    while (start <= leftEnd && rightStart <= rightEnd)
        if (ary[leftEnd] > ary[rightEnd])
            tmp[tmpIndex++] = ary[leftEnd--];
        else
            tmp[tmpIndex++] = ary[rightEnd--];

    // Append the remaining elements
    while (start <= leftEnd)
        tmp[tmpIndex++] = ary[leftEnd--];
    while (rightStart <= rightEnd)
        tmp[tmpIndex++] = ary[rightEnd--];

    // Reverse temp array and put it back into ary
    for (int i = start; i <= end; ++i)
        ary[i] = tmp[end - i];

    // Delete temporary array
    delete[] tmp;
}

// ========================================================================== //
// Method Name - RecursiveMergeSort
//      Recursive portion of the merge sort algorithm.
// ========================================================================== //
void RecursiveMergeSort(int *ary, int start, int end) {
    int mid = (end + start) / 2;

    if (start < end) {
        RecursiveMergeSort(ary, start, mid);
        RecursiveMergeSort(ary, mid + 1, end);
        Merge(ary, start, mid, end);
    }
}

// ========================================================================== //
// Method Name - MergeSort
//      Implementation of the merge sort algorithm for integers.
// ========================================================================== //
void MergeSort(int *ary, int size) {
    RecursiveMergeSort(ary, 0, size - 1);
}

// ========================================================================== //
// Method Name - Heapify Element
//      Heapify a single element from an array. If the heap property is
//      violated, it will be corrected by heapifying the offending element.
// ========================================================================== //
void HeapifyElement(int *ary, int size, int loc) {
    int tmp;
    int toSwap = loc;
    int child = loc * 2 + 1;

    // While the current element has a child
    while (child < size) {

        // Check if left child is larger
        if (ary[child] > ary[toSwap])
            toSwap = child;

        // Check if right child exists and is larger
        if (child + 1 < size && ary[child + 1] > ary[toSwap])
            toSwap = child + 1;

        // Swap if necessary
        if (loc != toSwap) {
            tmp = ary[loc];
            ary[loc] = ary[toSwap];
            ary[toSwap] = tmp;

            loc = toSwap;
            child = loc * 2 + 1;
        } else
            break;
    }
}

// ========================================================================== //
// Method Name - Heapify
//      Heapify an entire array from the bottom-up. The array is guaranteed 
//      to satisfy the heap property after this method is called.
// ========================================================================== //
void Heapify(int *ary, int size) {
    // Bottom-up
    for (int i = size - 1; i >= 0; --i)
        HeapifyElement(ary, size, i);
}

// ========================================================================== //
// Method Name - HeapSort
//      Implementation of the heap sort algorithm for integers.
// ========================================================================== //
void HeapSort(int *ary, int size) {
    int tmp;
    Heapify(ary, size);

    while (size > 0) {
        tmp = ary[0];
        ary[0] = ary[--size];
        ary[size] = tmp;

        HeapifyElement(ary, size, 0);
    }
}

// ========================================================================== //
// Method Name - RecursiveQuickSort
//      Recursive portion of the quick sort algorithm.
// ========================================================================== //
void RecursiveQuickSort(int *ary, int start, int end) {
    int i = start,
            j = end,
            pivot = ary[(start + end) / 2],
            tmp;

    while (i <= j) {
        while (ary[i] < pivot)
            ++i;
        while (ary[j] > pivot)
            --j;
        if (i <= j) {
            tmp = ary[i];
            ary[i++] = ary[j];
            ary[j--] = tmp;
        }
    }

    if (start < j)
        RecursiveQuickSort(ary, start, j);
    if (i < end)
        RecursiveQuickSort(ary, i, end);
}

// ========================================================================== //
// Method Name - QuickSort
//      Implementation of the quick sort algorithm for integers.
// ========================================================================== //
void QuickSort(int *ary, int size) {
    RecursiveQuickSort(ary, 0, size - 1);
}

// ========================================================================== //
// Method Name - MaxValue
//      Returns the element with the maximum value from an array.
// ========================================================================== //
int MaxValue(int *ary, int size) {
    int max = 0;

    for (int i = 0; i < size; ++i)
        if (ary[i] > max)
            max = ary[i];

    return max;
}

// ========================================================================== //
// Method Name - CountingSort
//      Implementation of the counting sort algorithm for integers.
// ========================================================================== //
void CountingSort(int *ary, int size) {
    int k = MaxValue(ary, size) + 1;

    // Initialize temporary & counting arrays
    int *temp = new int[size];
    int *counts = new int[k];
    for (int i = 0; i < k; ++i)
        counts[i] = 0;

    // Count occurrences
    for (int i = 0; i < size; ++i)
        counts[ary[i]]++;

    // Add previous indexes to get spot in final array
    for (int i = 1; i < k; ++i)
        counts[i] += counts[i - 1];

    // Copy from ary to correct position in temp
    for (int i = size - 1; i >= 0; --i) {
        temp[counts[ary[i]] - 1] = ary[i];
        --counts[ary[i]];
    }

    // Copy sorted array back into the original array
    for (int i = 0; i < size; ++i)
        ary[i] = temp[i];

    delete []temp;
    delete []counts;
}

// ========================================================================== //
// Method Name - NumDigits
//      Returns the number of digits in the specified integer.
// ========================================================================== //
int NumDigits(int num) {
    int digits = 0;

    while (num > 0) {
        ++digits;
        num /= 10;
    }

    return digits;
}

// ========================================================================== //
// Method Name - DigitValue
//      Returns the value of the digit specified from the number specified.
// ========================================================================== //
int DigitValue(int num, int digit) {
    for (int i = 0; num > 0 && i < digit; ++i)
        num /= 10;

    return num % 10;
}

// ========================================================================== //
// Method Name - CountingSort
//      Overloaded implementation of the counting sort algorithm that uses
//      the specified digit of each element as the key.
// ========================================================================== //
void CountingSort(int *ary, int size, int digit) {
    const int NUM_DIGITS = 10;

    // Initialize temporary & counting arrays
    int *temp = new int[size];
    int counts[NUM_DIGITS];
    for (int i = 0; i < NUM_DIGITS; ++i)
        counts[i] = 0;

    // Count occurrences
    for (int i = 0; i < size; ++i)
        ++counts[DigitValue(ary[i], digit)];

    // Add previous indexes to get spot in final array
    for (int i = 1; i < NUM_DIGITS; ++i)
        counts[i] += counts[i - 1];

    // Copy from ary to correct position in temp
    for (int i = size - 1; i >= 0; --i) {
        temp[counts[ DigitValue(ary[i], digit)] - 1] = ary[i];
        --counts[DigitValue(ary[i], digit)];
    }

    // Copy sorted array back into the original array
    for (int i = 0; i < size; ++i)
        ary[i] = temp[i];

    delete []temp;
}

// ========================================================================== //
// Method Name - RadixSort
//      Implementation of the radix sort algorithm for integers.
// ========================================================================== //
void RadixSort(int *ary, int size) {
    int digits = NumDigits(MaxValue(ary, size));

    for (int i = 0; i < digits; ++i)
        CountingSort(ary, size, i);
}

// ========================================================================== //
// Method Name - BucketSort
//      Implementation of the bucket sort algorithm for integers.
// ========================================================================== //
void BucketSort(int *ary, int size) {
    //TODO: Bucket Sort
}
