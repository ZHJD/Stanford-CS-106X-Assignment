#include "pqueue-heap.h"
using namespace std;

HeapPQueue::HeapPQueue(int capacity) : m_capacity(capacity) {
    m_pos      = 1;
    m_array = new string[m_capacity];
    m_array[0] = ""; // if logSize = 0, return this value
}
HeapPQueue::~HeapPQueue() {
    delete [] m_array;
}

const string& HeapPQueue::peek() const {
    if(logSize == 0) {
        return m_array[0];
    }
    return m_array[1];
}

// friend function
void adjustDown(HeapPQueue *pq, int index) {
    int parent = index;
    int child  = 2 * index;
    int m_pos = pq->m_pos;
    auto m_array = pq->m_array;
    while (child < m_pos) {
        if(child + 1 < m_pos && m_array[child + 1] < m_array[child]) {
            child++;
        }
        if(m_array[child] < m_array[parent]) {
            swap(m_array[child], m_array[parent]);
        }
        else {
            break;
        }

        parent = child;
        child  = child * 2;
    }
}

string HeapPQueue::extractMin() {
    if(logSize == 0) {
        return m_array[0];
    }
    string res = peek();
    logSize--;
    m_pos--;
    swap(m_array[1], m_array[m_pos]);
    adjustDown(this, 1);
    return res;
}

void HeapPQueue::expand() {
    string *new_array = new string[m_capacity * 2];
    for(int i = 1; i < m_capacity; i++) {
        new_array[i] = m_array[i];
    }
    m_capacity *= 2;
    delete [] m_array;
    m_array = new_array;
}

void HeapPQueue::adjustUp() {
    int child = m_pos;
    int parent = m_pos / 2;
    while(parent >= 1) {
        if(m_array[child] < m_array[parent]) {
            swap(m_array[child], m_array[parent]);
        }
        else {
            break;
        }
        child = parent;
        parent /= 2;
    }
}

void HeapPQueue::enqueue(const string& elem) {
    if(m_pos == m_capacity) {
        expand();
    }
    m_array[m_pos] = elem;
    adjustUp();
    logSize++;
    m_pos++;
}

HeapPQueue *HeapPQueue::merge(HeapPQueue *one, HeapPQueue *two) {
    int oneLogSize = one->logSize;
    int twoLogSize = two->logSize;
    HeapPQueue *newPQ = new HeapPQueue(oneLogSize + twoLogSize + 1);
    newPQ->logSize = oneLogSize + twoLogSize;

    for(int i = 1; i <= oneLogSize; i++) {
        newPQ->m_array[newPQ->m_pos++] = one->m_array[i];
    }
    for(int i = 1; i <= twoLogSize; i++) {
        newPQ->m_array[newPQ->m_pos++] = two->m_array[i];
    }

    // heapify
    for(int i = (newPQ->m_pos - 1) / 2; i >= 1; i--) {
        adjustDown(newPQ, i);
    }
    delete one;
    one = nullptr;
    delete two;
    two = nullptr;
    return newPQ;
}
