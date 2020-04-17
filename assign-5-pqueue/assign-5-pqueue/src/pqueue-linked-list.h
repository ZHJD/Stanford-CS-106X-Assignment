#pragma once
#include "pqueue.h"
#include <string>

struct DLNode {
    std::string val;
    DLNode *prev;
    DLNode *next;

    DLNode(std::string _val = "") : val(_val){}
};

class LinkedListPQueue : public PQueue {
public:
	LinkedListPQueue();
	~LinkedListPQueue();
	
	static LinkedListPQueue *merge(LinkedListPQueue *one, LinkedListPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
    DLNode *head;
    DLNode *tail;

    friend void insertBefore(DLNode *before, DLNode *node);

};



