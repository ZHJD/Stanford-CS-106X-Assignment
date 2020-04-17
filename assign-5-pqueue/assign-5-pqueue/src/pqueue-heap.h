#pragma once
#include "pqueue.h"
#include <string>

class HeapPQueue : public PQueue {
public:
    explicit HeapPQueue(int capacity = 5);
	~HeapPQueue();
	
	static HeapPQueue *merge(HeapPQueue *one, HeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
    
private:
    std::string *m_array;
    int m_capacity;         // array capacity
    int m_pos;              // insert position

    void expand();

    void adjustUp();

    // friend function to use in both static and non-static member function
    friend void adjustDown(HeapPQueue *pq, int index);
};
