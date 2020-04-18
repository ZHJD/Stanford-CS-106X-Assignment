#include "pqueue-binomial-heap.h"
using namespace std;

// move not copy
Node* Node::mergeTree(Node *&tree1, Node *&tree2) {
    Node *res = nullptr;
    if(tree1->elem < tree2->elem) {
        tree1->children.push_back(tree2);
        res = tree1;
    }
    else {
        tree2->children.push_back(tree1);
        res = tree2;
    }
    tree1 = nullptr;
    tree2 = nullptr;
    return res;
}

BinomialHeapPQueue::BinomialHeapPQueue() {}

BinomialHeapPQueue::~BinomialHeapPQueue() {
    for(int i = 0; i < m_trees.size(); i++) {
        deleteTree(m_trees[i]);
        m_trees[i] = nullptr;
    }
}

void BinomialHeapPQueue::deleteTree(Node *tree) {
    if(!tree) {
        return;
    }
    for(auto child : tree->children) {
        deleteTree(child);
    }
    delete tree;
}

int BinomialHeapPQueue::findMin() const {
    int min_elem = 0;
    for(int i = 0; i < m_trees.size(); i++) {
        if(m_trees[i]) {
            min_elem = i;
            break;
        }
    }
    for(int i = min_elem + 1; i < m_trees.size(); i++) {
        if(!m_trees[i]) {
            continue;
        }
        if(m_trees[i]->elem < m_trees[min_elem]->elem) {
            min_elem = i;
        }
    }
    return min_elem;
}

const string& BinomialHeapPQueue::peek() const {
    return m_trees[findMin()]->elem;
}

void BinomialHeapPQueue::merge(Vector<Node*>& result, Vector<Node*>& one, Vector<Node*>& two) {
    int i = 0;
    Node *carry = nullptr;
    while(i < one.size() && i < two.size()) {
        if(one[i] && two[i]) {
            if(carry) {
                result.push_back(carry);
            }
            else {
                result.push_back(nullptr);
            }
            carry = Node::mergeTree(one[i], two[i]);
        }
        else {
            Node *tmp = one[i] ? one[i] : two[i];
            if(tmp) {
                if(carry) {
                    carry = Node::mergeTree(tmp, carry);
                    result.push_back(nullptr);
                }
                else {
                    carry = nullptr;
                    result.push_back(tmp);
                }
            }
        }
        i++;
    }
    auto tmp = (i < one.size()) ? one : two;
    while(i < tmp.size()) {
        if(tmp[i]) {
            if(carry) {
                carry = Node::mergeTree(tmp[i], carry);
                result.push_back(nullptr);
            }
            else {
                carry = nullptr;
                result.push_back(tmp[i]);
            }
        }
        i++;
    }
    if(carry) {
        result.push_back(carry);
    }
}

string BinomialHeapPQueue::extractMin() {
    int minIndex = findMin();
    Node *smallRoot = m_trees[minIndex];
    string elem = smallRoot->elem;
    Vector<Node*> newTrees;
    for(int i = 0; i < smallRoot->children.size(); i++) {
        newTrees.push_back(smallRoot->children[i]);
    }
    delete smallRoot;
    m_trees[minIndex] = nullptr;
    Vector<Node*> result;
    merge(result, m_trees, newTrees);
    m_trees = result;
    logSize--;
    return elem;
}

void BinomialHeapPQueue::enqueue(const string& elem) {
    Node *tree = new Node(elem);
    for(int i = 0; i < m_trees.size(); i++) {
        if(m_trees[i]) {
            tree = Node::mergeTree(m_trees[i], tree);
            m_trees[i] = nullptr;
        }
        else {
            m_trees[i] = tree;
            tree = nullptr;
            break;
        }
    }
    if(tree) {
        m_trees.push_back(tree);
    }
    logSize++;
}

BinomialHeapPQueue *BinomialHeapPQueue::merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two) {
    BinomialHeapPQueue *newPQ = new BinomialHeapPQueue();
    newPQ->logSize = one->logSize + two->logSize;
    merge(newPQ->m_trees, one->m_trees, two->m_trees);
    one = nullptr;
    two = nullptr;
    return newPQ;
}
