#include "pqueue-vector.h"
using namespace std;

VectorPQueue::VectorPQueue() {}
VectorPQueue::~VectorPQueue() {}

int VectorPQueue::findMin() const{
    int res = 0;
    for(int i = 1; i < logSize; i++) {
        if(m_vector[i] < m_vector[res]) {
            res = i;
        }
    }
    return res;
}

const string& VectorPQueue::peek() const {
    if(logSize == 0) {
        throw "PQueue is empty!\n";
    }
    return m_vector[findMin()];
}

string VectorPQueue::extractMin() {
    if(logSize == 0) {
        throw "PQueue is empty!\n";
    }
    int index = findMin();
    string res = m_vector[index];
    m_vector.remove(index);
    logSize--;
    return res;
}

void VectorPQueue::enqueue(const string& elem) {
    m_vector.push_back(elem);
    logSize++;
}

void VectorPQueue::insertToNewPQ(VectorPQueue *old_pq, VectorPQueue *new_pq) {
    while(!old_pq->isEmpty()) {
        new_pq->enqueue(old_pq->m_vector.pop_front());
        old_pq->logSize--;
    }
}

VectorPQueue *VectorPQueue::merge(VectorPQueue *one, VectorPQueue *two) {
    VectorPQueue* new_pq = new VectorPQueue();
    insertToNewPQ(one, new_pq);
    insertToNewPQ(two, new_pq);
    return new_pq;
}
