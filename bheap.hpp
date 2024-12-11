#pragma once
#include "randomized_alg/globals.hpp"
//From geeksforgeeks: https://www.geeksforgeeks.org/binary-heap/
// A C++ program to demonstrate common Binary Heap Operations
#include<iostream>
#include<climits>
#include <memory> 
using namespace std;

template <class V> class MinHeap;

template <class V> struct Node {
private:
    V value;
    int heap_idx;

public:
	friend class MinHeap<V>;
    V getValue() {return value;}
    int getHeapIdx() {return heap_idx;}
    Node(V val, int idx) {
        value = val;
        heap_idx = idx;
    }
};


// A class for Min Heap
template <class V> class MinHeap
{
    Node<V> **harr; // pointer to array of elements in heap
    int capacity; // maximum possible size of min heap
    int heap_size; // Current number of elements in min heap
public:
    // Constructor
    MinHeap(int capacity);

    // to heapify a subtree with the root at given index
    void MinHeapify(int i);

    int parent(int i) {
		dijkstras_arithmetic_op_counter++;
		randomized_arithmetic_op_counter++;
         return (i-1)/2; 
    }

    // to get index of left child of Node at index i
    int left(int i) {
		dijkstras_arithmetic_op_counter++;
		randomized_arithmetic_op_counter++;
        return (2*i + 1);
    }

    // to get index of right child of Node at index i
    int right(int i) {
		dijkstras_arithmetic_op_counter++;
		randomized_arithmetic_op_counter++;
        return (2*i + 2);
    }

    // to extract the root which is the minimum element
    Node<V> extractMin();

    bool isEmpty() {return heap_size <= 0;}

    // Decreases key value of key at index i to new_val
    void decreaseKey(int i, V new_val);

    // Inserts a new key 'k'
    Node<V>* insertKey(V k);

    void swap(int i, int parent_i);
};



// Constructor: Builds a heap from a given array a[] of given size
template <class V> MinHeap<V>::MinHeap(int cap)
{
    heap_size = 0;
    capacity = cap;
    harr = new Node<V>*[cap];
}

// Inserts a new key 'k'
template <class V> Node<V>* MinHeap<V>::insertKey(V k)
{
    dijkstras_comparison_counter++;
    randomized_comparison_counter++;
    if (heap_size == capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return nullptr;
    }

    // First insert the new key at the end
    dijkstras_arithmetic_op_counter+=2;
    randomized_arithmetic_op_counter+=2;
    heap_size++;
    int i = heap_size - 1;
    Node<V>* n = new Node<V>(k, i);
    harr[i] = n;

    // Fix the min heap property if it is violated
    dijkstras_comparison_counter+=2;
    randomized_comparison_counter+=2;
    while (i != 0 && harr[parent(i)]->value > harr[i]->value)
    {
        swap(i, parent(i));
        i = parent(i);
        dijkstras_comparison_counter+=2;
        randomized_comparison_counter+=2;
    }
    return n;
}

// Decreases value of key at index 'i' to new_val.  It is assumed that
// new_val is smaller than harr[i].
template <class V> void MinHeap<V>::decreaseKey(int i, V new_val)
{
    harr[i]->value = new_val;
    dijkstras_comparison_counter+=2;
    randomized_comparison_counter+=2;
    while (i != 0 && harr[parent(i)]->value > harr[i]->value)
    {
        swap(i, parent(i));
        i = parent(i);
        dijkstras_comparison_counter+=2;
        randomized_comparison_counter+=2;
    }
}

// Method to remove minimum element (or root) from min heap
template <class V> Node<V> MinHeap<V>::extractMin()
{
    // dijkstras_comparison_counter++;
    // randomized_comparison_counter++;
    // if (heap_size <= 0)
    //     return NULL;
    dijkstras_comparison_counter++;
    randomized_comparison_counter++;
    if (heap_size == 1)
    {
        dijkstras_arithmetic_op_counter++;
        randomized_arithmetic_op_counter++;
        heap_size--;
        return *harr[0];
    }

    // Store the minimum value, and remove it from heap
    dijkstras_arithmetic_op_counter+=2;
    randomized_arithmetic_op_counter+=2;
    Node<V>* root = harr[0];
    harr[0] = harr[heap_size-1];
    heap_size--;
    MinHeapify(0);
    // root->heap_idx = NULL;
    return *root;
}

// A recursive method to heapify a subtree with the root at given index
// This method assumes that the subtrees are already heapified
template <class V> void MinHeap<V>::MinHeapify(int i)
{
    dijkstras_comparison_counter+=5;
    randomized_comparison_counter+=5;
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l]->value < harr[i]->value)
        smallest = l;
    if (r < heap_size && harr[r]->value < harr[smallest]->value)
        smallest = r;
    if (smallest != i)
    {
        swap(i, smallest);
        MinHeapify(smallest);
    }
}

// A utility function to swap two elements
template <class V> void MinHeap<V>::swap(int i, int parent_i)
{
    Node<V>* temp = harr[i];
    harr[i] = harr[parent_i];
    harr[parent_i] = temp;
    harr[i]->heap_idx = i;
    harr[parent_i]->heap_idx = parent_i;
}