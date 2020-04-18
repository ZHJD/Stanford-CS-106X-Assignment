#pragma once
#include "pqueue.h"
#include <string>
#include "vector.h"

struct Node {
    std::string elem;
    Vector<Node *> children;

    Node(std::string _elem = "") : elem(_elem){}

    static Node* mergeTree(Node *&tree1, Node *&tree2);
};

class BinomialHeapPQueue : public PQueue {
public:
	BinomialHeapPQueue();
	~BinomialHeapPQueue();
	
	static BinomialHeapPQueue *merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
    Vector<Node*> m_trees;

    static void merge(Vector<Node*>& result, Vector<Node*>& one, Vector<Node*>& two);

    int findMin() const;

    void deleteTree(Node *tree);
};

