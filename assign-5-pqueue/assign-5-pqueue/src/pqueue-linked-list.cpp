#include "pqueue-linked-list.h"
#include "error.h"
using namespace std;

LinkedListPQueue::LinkedListPQueue() {
    head = new DLNode();
    tail = new DLNode();

    head->prev = nullptr;
    head->next = tail;
    tail->prev = head;
    tail->next = nullptr;
}

LinkedListPQueue::~LinkedListPQueue() {
    auto cur = tail;
    while(cur) {
        auto tmp = cur->prev;
        delete cur;
        cur = tmp;
    }
}

const string& LinkedListPQueue::peek() const {
    if(logSize == 0) {
        error("Priority queue is empty!");
        return nullptr;
    }
    return head->next->val;
}

string LinkedListPQueue::extractMin() {
    if(logSize == 0) {
        error("Priority queue is empty!");
        return nullptr;
    }
    auto del_node = head->next;
    string res = del_node->val;
    head->next = del_node->next;
    del_node->next->prev = head;
    delete del_node;
    logSize--;
    return res;
}

void insertBefore(DLNode *before, DLNode *node) {
    node->next = before;
    node->prev = before->prev;
    before->prev->next = node;
    before->prev = node;
}

void LinkedListPQueue::enqueue(const string& elem) {
    logSize++;
    tail->val = elem;
    auto node = head->next;
    while(node->val < elem) {
        node = node->next;
    }
    auto newNode = new DLNode(elem);
    insertBefore(node, newNode);
}

LinkedListPQueue *LinkedListPQueue::merge(LinkedListPQueue *one, LinkedListPQueue *two) {
    LinkedListPQueue *listPQ = new LinkedListPQueue();
    auto p = one->head->next;
    auto q = two->head->next;
    auto k = listPQ->head;
    while(p != one->tail && q != two->tail) {
        if(p->val < q->val) {
            auto newNode = new DLNode(p->val);
            insertBefore(k->next, newNode);
            p = p->next;
            k = k->next;
        }
        else if(p->val == q->val) {
            auto newNode = new DLNode(p->val);
            insertBefore(k->next, newNode);
            k = k->next;
            newNode = new DLNode(q->val);
            insertBefore(k->next, newNode);
            p = p->next;
            q = q->next;
            k = k->next;
        }
        else {
            auto newNode = new DLNode(q->val);
            insertBefore(k->next, newNode);
            q = q->next;
            k = k->next;
        }
    }
    while(p != one->tail) {
        auto newNode = new DLNode(p->val);
        insertBefore(k->next, newNode);
        k = k->next;
        p = p->next;
    }
    while(q != two->tail) {
        auto newNode = new DLNode(q->val);
        insertBefore(k->next, newNode);
        k = k->next;
        q = q->next;
    }
    listPQ->logSize = one->logSize + two->logSize;
    delete one;
    delete two;
    return listPQ;
}

