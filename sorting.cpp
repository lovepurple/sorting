#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/time.h>
using namespace std;

// Sort function typedef
typedef void (*SortFunction)(int*, int);

// Function prototypes - documentation in declarations
void SingleTest(SortFunction, int, int);
void RunTestSuite();
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

int main() {
    RunTestSuite();
    //SingleTest(RadixSort, 15, 50000);

    return 0;
}

void SingleTest(SortFunction func, int size, int maxKey) {
    int *test = RandomArray(size, maxKey);

    PrintArray(test, size);
    (*func)(test, size);
    PrintArray(test, size);

    delete[] test;
}

void RunTestSuite() {
    //const int NUM_SORT_FUNCS = 7;
    const int NUM_SORT_FUNCS = 5;
    const int MAX_KEY_VALUE = 50000;
    const int NUM_TESTS = 11;
    const int NUM_SAMPLES = 10;
    const int TEST_SIZES[] = {10, 50, 100,
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

    for (int i = 0; i < NUM_TESTS; i++) {
        int N = TEST_SIZES[i];
        cout << "N=" << N << endl;

        // Generate random samples
        int **randArrays = new int*[NUM_SAMPLES];
        for (int i = 0; i < NUM_SAMPLES; i++)
            randArrays[i] = RandomArray(N, MAX_KEY_VALUE);

        for (int j = 0; j < NUM_SORT_FUNCS; j++) {
            cout << sortFuncNames[j];
            for (int k = 0; k < NUM_SAMPLES; k++) {
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

        for (int j = 0; j < NUM_SAMPLES; j++)
            delete[] randArrays[j];
        delete[] randArrays;
    }
}

void PrintArray(int *ary, int size) {
    for (int i = 0; i < size; i++) {
        cout << ary[i] << " ";
    }
    cout << endl;
}

int *RandomArray(int size, int maxRandNum) {
    int *randArray = new int[size];
    timeval timeseed;
    gettimeofday(&timeseed, NULL);

    srand(timeseed.tv_usec);
    for (int i = 0; i < size; i++) {
        randArray[i] = (rand() % maxRandNum) + 1;
    }

    return randArray;
}

int *CopyArray(int *ary, int size) {
    int *tmp = new int[size];

    for (int i = 0; i < size; i++)
        tmp[i] = ary[i];

    return tmp;
}

void PrintTimeDiff(timeval start, timeval end) {
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;

    //double mtime = ((seconds) + useconds/1000000.0);
    long utime = ((seconds * 1000000) + useconds);

    // utime is in microseconds
    cout << "\t" << utime;
}

void InsertionSort(int *ary, int size) {
    for (int i = 1; i < size; i++) {
        int key = ary[i];
        for (int j = i - 1; j >= 0 && ary[j] > key; j--) {
            ary[j + 1] = ary[j];
            ary[j] = key;
        }
    }
}

void Merge(int *ary, int start, int mid, int end) {

    // Necessary variables
    int tmpIndex = 0,
            rightStart = mid + 1,
            rightEnd = end,
            leftEnd = mid;

    // Temporary array
    int *tmp = new int[end - start + 1];

    // Append max until one half is out of elements
    while (start <= leftEnd && rightStart <= rightEnd) {
        if (ary[leftEnd] > ary[rightEnd])
            tmp[tmpIndex++] = ary[leftEnd--];
        else
            tmp[tmpIndex++] = ary[rightEnd--];
    }

    // Append the remaining elements
    while (start <= leftEnd)
        tmp[tmpIndex++] = ary[leftEnd--];
    while (rightStart <= rightEnd)
        tmp[tmpIndex++] = ary[rightEnd--];

    // Reverse temp array and put it back into ary
    for (int i = start; i <= end; i++) {
        ary[i] = tmp[end - i];
    }

    // Delete temporary array
    delete[] tmp;
}

void RecursiveMergeSort(int *ary, int start, int end) {
    int mid = (end + start) / 2;

    if (start < end) {
        RecursiveMergeSort(ary, start, mid);
        RecursiveMergeSort(ary, mid + 1, end);
        Merge(ary, start, mid, end);
    }
}

void MergeSort(int *ary, int size) {
    RecursiveMergeSort(ary, 0, size - 1);
}

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

void Heapify(int *ary, int size) {
    // Bottom-up
    for (int i = size - 1; i >= 0; i--) {
        HeapifyElement(ary, size, i);
    }
}

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

void RecursiveQuickSort(int *ary, int start, int end) {
    int i = start,
            j = end,
            pivot = ary[(start + end) / 2],
            tmp;

    while (i <= j) {
        while (ary[i] < pivot)
            i++;
        while (ary[j] > pivot)
            j--;
        if (i <= j) {
            tmp = ary[i];
            ary[i++] = ary[j];
            ary[j--] = tmp;
        }
    };

    if (start < j)
        RecursiveQuickSort(ary, start, j);
    if (i < end)
        RecursiveQuickSort(ary, i, end);
}

void QuickSort(int *ary, int size) {
    RecursiveQuickSort(ary, 0, size - 1);
}

int MaxValue(int *ary, int size) {
    int max = 0;

    for (int i = 0; i < size; i++)
        if (ary[i] > max)
            max = ary[i];

    return max;
}

void CountingSort(int *ary, int size) {
    int k = MaxValue(ary, size) + 1;

    // Initialize temporary & counting arrays
    int *temp = new int[size];
    int *counts = new int[k];
    for (int i = 0; i < k; i++)
        counts[i] = 0;

    // Count occurences
    for (int i = 0; i < size; i++)
        counts[ary[i]]++;

    // Add previous indexes to get spot in final array
    for (int i = 1; i < k; i++)
        counts[i] += counts[i - 1];

    // Copy from ary to correct position in temp
    for (int i = size - 1; i >= 0; i--) {
        temp[counts[ary[i]] - 1] = ary[i];
        counts[ary[i]]--;
    }

    // Copy sorted array back into the original array
    for (int i = 0; i < size; i++)
        ary[i] = temp[i];

    delete []temp;
    delete []counts;
}

int NumDigits(int num) {
    int digits = 0;

    while (num > 0) {
        digits++;
        num /= 10;
    }

    return digits;
}

int DigitValue(int num, int digit) {
    for (int i = 0; num > 0 && i < digit; i++)
        num /= 10;

    return num % 10;
}

void CountingSort(int *ary, int size, int digit) {
    const int NUM_DIGITS = 10;

    // Initialize temporary & counting arrays
    int *temp = new int[size];
    int counts[NUM_DIGITS];
    for (int i = 0; i < NUM_DIGITS; i++)
        counts[i] = 0;

    // Count occurences
    for (int i = 0; i < size; i++)
        counts[DigitValue(ary[i], digit)]++;

    // Add previous indexes to get spot in final array
    for (int i = 1; i < NUM_DIGITS; i++)
        counts[i] += counts[i - 1];

    // Copy from ary to correct position in temp
    for (int i = size - 1; i >= 0; i--) {
        temp[counts[ DigitValue(ary[i], digit)] - 1] = ary[i];
        counts[DigitValue(ary[i], digit)]--;
    }

    // Copy sorted array back into the original array
    for (int i = 0; i < size; i++)
        ary[i] = temp[i];

    delete []temp;
}

void RadixSort(int *ary, int size) {
    int digits = NumDigits(MaxValue(ary, size));

    for (int i = 0; i < digits; i++)
        CountingSort(ary, size, i);
}

void BucketSort(int *ary, int size) {
    //TODO: Bucket Sort
}
