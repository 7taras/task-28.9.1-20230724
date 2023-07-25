// task-28.9.1-20230724.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Доработайте алгоритм merge sort, описанный в 11 модуле, сделав его многопоточным.
// Используйте std::futurez в качестве инструмента для синхронизации результата работы потоков.
// Нужно определить независимые части алгоритма, которые могут быть распараллелены и запустить их вычисление в отдельных потоках, как это было сделано в юните 28.7.
// Потоки могут быть запущены через std::thread или std::async, но синхронизация строго через std::future.Также предусмотрите ограничение количества запускаемых потоков по примеру из юнита 28.7.

#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <random>

using namespace std;

unsigned int numberOfConcurrentThreads;
unsigned int test{ 0 };

// Merges two subarrays of array[].
// First subarray is arr[begin..mid]
// Second subarray is arr[mid+1..end]
void merge(int array[], int const left, int const mid, int const right)
{
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;

    // Create temp arrays
    auto* leftArray = new int[subArrayOne],
        * rightArray = new int[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) 
    {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) 
        {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else 
        {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    // Copy the remaining elements of left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) 
    {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    // Copy the remaining elements of right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) 
    {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

// begin is for left index and end is right index of the sub-array of arr to be sorted
void mergeSort(int array[], int const begin, int const end)
{
    if (begin >= end)
        return;
    
    int mid = begin + (end - begin) / 2;
    
    if (numberOfConcurrentThreads)
    {
        numberOfConcurrentThreads--;
        future<void> fut = async(launch::async, [&]() { mergeSort(array, begin, mid); });
        mergeSort(array, mid + 1, end);
        fut.get();
    }
    else
    {
        mergeSort(array, begin, mid);
        mergeSort(array, mid + 1, end);
    }
    merge(array, begin, mid, end);
    return;
}

void mergeSortWithoutThreads(int array[], int const begin, int const end)
{
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
    return;
}

// Function to print an array
void printArray(int A[], int size)
{
    for (int i = 0; i < size; i++)
        cout << A[i] << " ";
    cout << endl;
}


int main()
{
    numberOfConcurrentThreads = std::thread::hardware_concurrency();
    std::cout << numberOfConcurrentThreads << " concurrent threads are supported.\n";
    numberOfConcurrentThreads--;

    srand(0);
    long arr_size = 1000000;
    int* arr = new int[arr_size];
    for (long i = 0; i < arr_size; i++) 
    {
        arr[i] = rand() % 500000;
    }

    //cout << "Given array is \n";
    //printArray(arr, arr_size);

    time_t start, end;

    time(&start);
    mergeSort(arr, 0, arr_size - 1);
    //mergeSortWithoutThreads(arr, 0, arr_size - 1);
    time(&end);

    cout << test << "\n";
    int seconds = difftime(end, start);
    printf("Threads. The time: %d seconds\n", seconds);

    time(&start);
    //mergeSort(arr, 0, arr_size - 1);
    mergeSortWithoutThreads(arr, 0, arr_size - 1);
    time(&end);

    seconds = difftime(end, start);
    printf("No threads. The time: %d seconds\n", seconds);

    //cout << "\nSorted array is \n";
    //printArray(arr, arr_size);
}