#include <iostream>
#include <sys/time.h>
using namespace std;

void RunTestSuite();
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
    long seconds  = end.tv_sec  - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;

    double mtime = ((seconds) + useconds/1000000.0);

    cout << "\t\t" << mtime << endl;
}

void swap(int *ary, int loc1, int loc2) {
    int tmp = ary[loc1];
    ary[loc1] = ary[loc2];
    ary[loc2] = tmp;
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
    while(start <= leftEnd)
        tmp[tmpIndex++] = ary[leftEnd--];
    while(rightStart <= rightEnd)
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
            swap(ary, loc, toSwap);
            loc = toSwap;
            child = loc * 2 + 1;
        } else 
            break;
    }
}

void Heapify(int *ary, int size) {
    // Bottom-up
    for(int i = size-1; i >= 0; i--) {
        HeapifyElement(ary, size, i);   
    }
}

void HeapSort(int *ary, int size) {
    Heapify(ary, size);
    
    while (size > 0) {
        swap(ary, 0, --size);
        HeapifyElement(ary, size, 0);
    }
}

void QuickSort(int *ary, int size) {

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

    // Initialize counting array
    int *counts = new int[k];
    for (int i = 0; i < k; i++)
        counts[i] = 0;

    // Count occurances
    for (int i = 0; i < size; i++)
        counts[ary[i]]++;

    // Place counts into original array
    int index = 0;
    for (int i = 0; i < k; i++)
        for (int j = 0; j < counts[i]; j++)
            ary[index++] = i;

    delete []counts;
}

void RadixSort(int *ary, int size) {

}

void BucketSort(int *ary, int size) {

}


int main() {
    /*
    int size = 20;
    int *test = RandomArray(size, 99);
    
    PrintArray(test, size);
    CountingSort(test, size);
    PrintArray(test, size);
    */

    RunTestSuite();

    return 0;
}

void RunTestSuite()
{
    timeval start, end;
    int maxNum = 50;
    int tests = 11;
    int samples = 5;
    int testSizes[] = {10, 50, 100, 
                       500, 1000, 5000,  
                       10000, 50000, 100000, 
                       500000, 1000000};
    

    for (int i = 0; i < tests; i++) {
        int N = testSizes[i];
        cout << "Sorting with N=" << N << endl;

        int **randArrays = new int*[samples];
        for (int i = 0; i < samples; i++)
            randArrays[i] = RandomArray(N, maxNum);

        int *tmpArray;
        
        /*
        // Insetion Sort
        cout << "\tInsertion Sort" << endl;
        for (int j = 0; j < samples; j++) {
            tmpArray = CopyArray(randArrays[j], N);
            gettimeofday(&start, NULL);
            InsertionSort(tmpArray, N);
            gettimeofday(&end, NULL);
            PrintTimeDiff(start, end);
            delete[] tmpArray;
        }
        */

        // Merge Sort
        cout << "\tMerge Sort" << endl;
        for (int j = 0; j < samples; j++) {
            tmpArray = CopyArray(randArrays[j], N);
            gettimeofday(&start, NULL);
            MergeSort(tmpArray, N);
            gettimeofday(&end, NULL);
            PrintTimeDiff(start, end);
            
            delete[] tmpArray;
        }

        // Heap Sort
        cout << "\tHeap Sort" << endl;;
        for (int j = 0; j < samples; j++) {
            tmpArray = CopyArray(randArrays[j], N);
            gettimeofday(&start, NULL);
            HeapSort(tmpArray, N);
            gettimeofday(&end, NULL);
            PrintTimeDiff(start, end);
            delete[] tmpArray;
        }

        /*
        // Quick Sort
        cout << "\tQuick Sort" << endl;
        for (int j = 0; j < samples; j++) {
            tmpArray = CopyArray(randArrays[j], N);
            gettimeofday(&start, NULL);
            QuickSort(tmpArray, N);
            gettimeofday(&end, NULL);
            PrintTimeDiff(start, end);
            delete[] tmpArray;
        }
        */

        // Counting Sort
        cout << "\tCounting Sort" << endl;
        for (int j = 0; j < samples; j++) {
            tmpArray = CopyArray(randArrays[j], N);
            gettimeofday(&start, NULL);
            CountingSort(tmpArray, N);
            gettimeofday(&end, NULL);
            PrintTimeDiff(start, end);
            delete[] tmpArray;
        }
    
        /*
        // Radix Sort
        cout << "\tRadix Sort" << endl;;
        for (int j = 0; j < samples; j++) {
            tmpArray = CopyArray(randArrays[j], N);
            gettimeofday(&start, NULL);
            RadixSort(tmpArray, N);
            gettimeofday(&end, NULL);
            PrintTimeDiff(start, end);
            delete[] tmpArray;
        }
        
        // Bucket Sort
        cout << "\tBucket Sort" << endl;
        for (int j = 0; j < samples; j++) {
            tmpArray = CopyArray(randArrays[j], N);
            gettimeofday(&start, NULL);
            BucketSort(tmpArray, N);
            gettimeofday(&end, NULL);
            PrintTimeDiff(start, end);
            delete[] tmpArray;
        }
        */

        cout << endl;

        for (int j = 0; j < samples; j++)
            delete[] randArrays[j];
        delete[] randArrays;
    }
}
