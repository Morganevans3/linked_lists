//
// Created by Morgan Evans on 2/16/23.
//

#ifndef PROJECT1_LINKEDLISTS_LINKEDLIST_H
#define PROJECT1_LINKEDLISTS_LINKEDLIST_H

#include <iostream>
#include <vector>
using namespace std;

// reuse code from other functions
template <typename T>
class LinkedList {
public:
    // Nested class
    struct Node {
        // the data you are storing
        T data;
        // a pointer to the next node
        Node* next;
        // a pointer to a previous node (if doubly-linked)
        Node* prev;
    };
    // a node pointer to the head
    // a node pointer to the tail
    // how many nodes are there?

    // Construction / Destruction
    LinkedList();
    LinkedList(const LinkedList<T>& list);
    LinkedList<T>& operator=(const LinkedList<T>& rhs);
    ~LinkedList();

    // Behaviors
    void PrintForward() const;
    void PrintReverse() const;
    void PrintForwardRecursive(const Node* node) const;
    void PrintReverseRecursive(const Node* node) const;

    // Accessors
    unsigned int NodeCount() const;
    void FindAll(vector<Node*>& outData, const T& value) const;
    const Node* Find(const T& data) const {
        Node* finder = head_ptr;
        for (unsigned int i = 0; i < count_nodes; i++) {
            if (finder->data == data) {
                return finder;
            }
            else {
                finder = finder->next;
            }
        }
        finder = nullptr;
        return finder;
    }
    Node* Find(const T& data);
    const Node* GetNode(unsigned int index) const {
        //Given an index, return a pointer to the node at that index. Throws an exception
        //of type out_of_range if the index is out of range. Const and non-const versions.

        if (index >= count_nodes) {
            throw out_of_range("Index out of range");
        }
        Node* ptr_index = head_ptr;
        for (unsigned int i = 0; i < index-1; i++) {
            ptr_index = ptr_index->next;
        }
        return ptr_index;

    }
    Node* GetNode(unsigned int index);
    Node* Head();
    const Node* Head() const {
        return head_ptr;
    }
    Node* Tail();
    const Node* Tail() const {
        return tail_ptr;
    }

    // Insertion
    void AddHead(const T& data);
    void AddTail(const T& data);
    void AddNodesHead(const T* data, unsigned int count);
    void AddNodesTail(const T* data, unsigned int count);
    void InsertAfter(Node* node, const T& data);
    void InsertBefore(Node* node, const T& data);
    void InsertAt(const T& data, unsigned int index);

    // Removal
    bool RemoveHead();
    bool RemoveTail();
    unsigned int Remove(const T&data);
    bool RemoveAt(unsigned int index);
    void Clear();

    // Operators
    const T& operator[](unsigned int index) const {
        // Overloaded subscript operator. Takes an index, and returns data from the
        // index-th node. Throws an out_of_range exception for an invalid index. Const
        // and non-const versions.

        if (index >= count_nodes) {
            throw out_of_range("Index out of range");
        }
        Node* data_index = head_ptr;
        for (unsigned int i = 0; i < index; i++) {
            data_index = data_index->next;
        }
        return data_index->data;
    }
    T& operator[](unsigned int index);
    bool operator==(const LinkedList<T>& rhs) const;


private:
    unsigned int count_nodes;
    Node* head_ptr;
    Node* tail_ptr;
};


class Node;

template <typename T>
LinkedList<T>::LinkedList() {
    this->count_nodes = 0;
    this->head_ptr = nullptr;
    this->tail_ptr = nullptr;
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) {
    // copy constructor
    head_ptr = nullptr;
    tail_ptr = nullptr;
    count_nodes = 0;
    //*this = list;
    Node* copy = list.head_ptr;
    while (copy != nullptr) {
        AddTail(copy->data);
        copy = copy->next;
        //Node* copy_node = new Node(copy->next);
        //tail_ptr->next = copy_node;
        //tail_ptr = copy_node;
        //copy = copy->next;
        //head_ptr = copy;
        //count_nodes += 1;
    }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {
    this->~LinkedList();
    head_ptr = nullptr;
    tail_ptr = nullptr;
    count_nodes = 0;
    Node* copy = rhs.head_ptr;
    while (copy != nullptr) {
        AddTail(copy->data);
        copy = copy->next;
    }
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    // delete
    Node* current = head_ptr;
    while (current != nullptr) {
        Node *after = current->next;
        delete current;
        current = after;
        count_nodes -= 1;
    }
}

template <typename T>
void LinkedList<T>::PrintForward() const {
    Node* forward = head_ptr;
    while (forward != nullptr) {
        cout << forward->data << endl;
        forward = forward->next;
    }
}

template <typename T>
void LinkedList<T>::PrintReverse() const {
    Node* backwards = tail_ptr;
    while (backwards != nullptr) {
        cout << backwards->data << endl;
        backwards = backwards->prev;
    }
}

template <typename T>
unsigned int LinkedList<T>::NodeCount() const {
    return count_nodes;
}

template <typename T>
void LinkedList<T>::AddHead(const T &data) {
    Node* new_head = new Node();
    new_head->data = data;
    new_head->next = head_ptr;
    // head_ptr->prev = new_head;
    head_ptr = new_head;
    count_nodes += 1;
    if (tail_ptr) {
        head_ptr->next->prev = head_ptr;
    }
    else {
        tail_ptr = new_head;
    }
}

template <typename T>
void LinkedList<T>::AddTail(const T &data) {
    Node* new_tail = new Node();
    new_tail->data = data;
    // tail_ptr->next = new_tail;
    new_tail->prev = tail_ptr;
    tail_ptr = new_tail;
    count_nodes += 1;
    if (head_ptr) {
        tail_ptr->prev->next = tail_ptr;
    }
    else {
        head_ptr = new_tail;
    }
}

template <typename T>
void LinkedList<T>::AddNodesHead(const T *data, unsigned int count) {
    for (unsigned int i = count; i > 0; i--) {
        AddHead(data[i-1]);
    }
}

template <typename T>
void LinkedList<T>::AddNodesTail(const T *data, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        AddTail(data[i]);
    }
}

template <typename T>
void LinkedList<T>::FindAll(vector<Node*>& outData, const T& value) const {
    // Find all nodes which match the passed in parameter value, and store a pointer to
    //that node in the passed in vector. Use of a parameter like this (passing a
    //something in by reference, and storing data for later use) is called an output
    //parameter.
    Node* t_finds = head_ptr;
    for (unsigned int i = 0; i < count_nodes; i++) {
        if (t_finds->data != value) {
            t_finds = t_finds->next;
        }
        else {
            outData.push_back(t_finds);
            t_finds = t_finds->next;
        }
    }

}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) {
    Node* finder = head_ptr;
    for (unsigned int i = 0; i < count_nodes; i++) {
        if (finder->data == data) {
            return finder;
        }
        else {
            finder = finder->next;
        }
    }
    finder = nullptr;
    return finder;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) {

    if (index >= count_nodes) {
        throw out_of_range("Index out of range");
    }

    Node* ptr_index = head_ptr;
    for (unsigned int i = 0; i < index; i++) {
        ptr_index = ptr_index->next;
    }
    return ptr_index;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Head() {
    // Returns the head pointer. Const and non-const versions.
    return head_ptr;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Tail() {
    // Returns the tail pointer. Const and non-const versions.
    return tail_ptr;
}


template <typename T>
T& LinkedList<T>::operator[](unsigned int index) {

    if (index >= count_nodes) {
        throw out_of_range("Index out of range");
    }

    Node* data_index = head_ptr;
    for (unsigned int i = 0; i < index; i++) {
        data_index = data_index->next;
    }
    return data_index->data;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& rhs) const {
    //Overloaded equality operator. Given listA and listB, is listA equal to listB? What
    //would make one Linked List equal to another? If each of its nodes were equal to
    //the corresponding node of the other. (Similar to comparing two arrays, just with
    //non-contiguous data).

    Node* original = head_ptr;
    for (unsigned int i = 0; i < count_nodes; i++) {
        if (rhs[i] == original->data) {
            original = original->next;
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}



template <typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& data) {
    Node* after = new Node();
    after->data = data;
    after->next = node->next;
    after->prev = node;
    node->next = after;
    count_nodes += 1;
}

template <typename T>
void LinkedList<T>::InsertBefore(Node* node, const T& data) {
    InsertAfter(node->prev, data);

}

template <typename T>
void LinkedList<T>::InsertAt(const T& data, unsigned int index) {
    if (index == count_nodes) {
        AddTail(data);
    }
    else if (index == 0){
        AddHead(data);
    }
    else {
        InsertAfter(GetNode(index-1), data);
    }
}

// -------------------------------------
// good
template <typename T>
void LinkedList<T>::PrintForwardRecursive(const Node* node) const {
    while (node != nullptr) {
        cout << node->data << endl;
        node = node->next;
    }
}
// good
template <typename T>
void LinkedList<T>::PrintReverseRecursive(const Node* node) const {
    while (node != nullptr) {
        cout << node->data << endl;
        node = node->prev;
    }
}
// good
template <typename T>
bool LinkedList<T>::RemoveHead() {
    if (count_nodes == 0) {
        return false;
    }
    if (count_nodes == 1) {
        this->~LinkedList();
        return true;
    }

    if (head_ptr->next != nullptr) {
        head_ptr->next->prev = nullptr;
    }
    else {
        tail_ptr = head_ptr;
    }
    count_nodes -= 1;
    Node* r_head = head_ptr;
    head_ptr = head_ptr->next;
    delete r_head;
    return true;
}
// good
template <typename T>
bool LinkedList<T>::RemoveTail() {
    if (count_nodes == 0) {
        return false;
    }
    if (count_nodes == 1) {
        this->Clear();
        return true;
    }

    count_nodes -= 1;
    if (tail_ptr->prev != nullptr) {
        tail_ptr->prev->next = nullptr;
    }
    else {
        head_ptr = tail_ptr;
    }
    Node* r_tail = tail_ptr;
    tail_ptr = tail_ptr->prev;
    delete r_tail;
    return true;
}
// error
template <typename T>
unsigned int LinkedList<T>::Remove(const T& data) {

    vector<LinkedList<T>::Node *> nodes;
    FindAll(nodes, data);
    for (unsigned int i = 0; i < nodes.size(); i++) {
        Node* before = nodes[i]->prev;
        before->next = nodes[i]->next;
        delete nodes[i];
        count_nodes -= 1;
    }
    return nodes.size();
}
//error
template <typename T>
bool LinkedList<T>::RemoveAt(unsigned int index) {
    int x = 0;
    try {
        if (index >= count_nodes) {
            throw x;
        }
    }
    catch (int x) {
        return false;
    }
    Node* removed_node = GetNode(index);
    Node* before = removed_node->prev;
    before->next = removed_node->next;
    delete removed_node;
    count_nodes -= 1;
    return true;
}

template <typename T>
void LinkedList<T>::Clear() {
    Node* current = head_ptr;
    while (current != nullptr) {
        Node *after = current->next;
        delete current;
        current = after;
        count_nodes -= 1;
    }
    this->count_nodes = 0;
    this->head_ptr = nullptr;
    this->tail_ptr = nullptr;
}


#endif //PROJECT1_LINKEDLISTS_LINKEDLIST_H
