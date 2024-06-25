#include <iostream>
#include <utility>
#include <string>
#include "myIteratorList.hpp"
#include "myInteger.hpp"

// default constructor
template <typename T>
MyList<T>::MyList() {
  // initialises the variables declared in the header file
  sentinelNode = new Node(); // Sentinel node links the tail and the head
  head = nullptr;
  tail = nullptr;
  numNodes = 0;
}

// copy constructor
template <typename T>
MyList<T>::MyList(const MyList& other) : MyList() {
  // Begins at the head of 'other' and copies each element to the new list
  Node *current = other.head;
  while(current != nullptr){
    push_back(current->data);
    current = current->next;
  }
}

// assignment operator 
template <typename T>
MyList<T>& MyList<T>::operator=(MyList other) {
  // swap all nodes of the first List with the 'other' List 
  MyList temp(other); // create a copy of 'other' list
  std::swap(*this, temp); // swap contents with temporary object 
  return *this; 
}


// destructor
template <typename T>
MyList<T>::~MyList() {
  //remove each node using pop_back function until the List is empty
  while(!empty()){
    pop_back();
  }
  delete sentinelNode; //delete the sentinel node to avoid memory leaks
}

// constructor from an initializer list
template <typename T>
MyList<T>::MyList(std::initializer_list<T> vals) : MyList() {
  //pushes each value of the initializer list to the back of the new List
  for(const auto &val : vals){
    push_back(val);
  }
}

// push back
template <typename T>
void MyList<T>::push_back(T val) {
  // Create the new node to be added to the end of the list,
  // data of the node is set to the val parameter. 
  Node *newNode = new Node(val, tail, sentinelNode);
  // update the pointers of the tail and sentinel node to add the new node
  if(empty()){
    head = tail = newNode;
    head->prev = sentinelNode;
    sentinelNode->next = head;
    sentinelNode->prev = tail;
  } else {
    tail->next = newNode;
    tail = newNode;
    sentinelNode->prev = newNode;
  }
  numNodes++;
}

// pop back
template <typename T>
void MyList<T>::pop_back() {
  if(empty()) return; 

  Node *oldTail = tail; // Store a pointer to the current tail
  tail = tail->prev;
  delete oldTail;
  numNodes--;
}

template <typename T>
void MyList<T>::push_front(T val) {
  // Create the new node to be added to the end of the list,
  // data of the node is set to the val parameter. 
  Node *newNode = new Node(val, sentinelNode, head);
  // update the pointers of the head and sentinel node to add the new node
  if(empty()){
    head = tail = newNode;
    head->next = sentinelNode;
    sentinelNode->next = head;
    sentinelNode->prev = tail;
  } else {
    head->prev = newNode;
    head = newNode;
    sentinelNode->next = head;
  }
  numNodes++;
}

template <typename T>
void MyList<T>::pop_front() {
  if(empty()) return;

  Node *oldHead = head;// Store a pointer to the current tail
  head = head->next;
  delete oldHead;
  numNodes--;
}

// return the first element by reference
template <typename T>
T& MyList<T>::front() {
  return head->data;
}

// return the first element by const reference
template <typename T>
const T& MyList<T>::front() const {
  return head->data;
}

// return the last element by reference
template <typename T>
T& MyList<T>::back() {
  return tail->data;
}

// return the last element by const reference
template <typename T>
const T& MyList<T>::back() const {
  return tail->data;
}

// return a boolean depending on if the list is empty
template <typename T>
bool MyList<T>::empty() const {
  return numNodes == 0;
}

// return the number of elements in the list
template <typename T>
int MyList<T>::size() const {
  return numNodes;
}

// return an iterator pointing to the first element
template <typename T>
typename MyList<T>::Iterator MyList<T>::begin() const {
  if(head != nullptr){
    return Iterator(head);
  }
  return Iterator(sentinelNode);
}

// return an iterator pointing to the last element
template <typename T>
typename MyList<T>::Iterator MyList<T>::end() const {
  return Iterator(sentinelNode);
}

// insert an element into the linked list before *it with the value val
template <typename T>
void MyList<T>::insert(const Iterator& it, const T& val) {
  // Create new node to be inserted with data val and correct pointers 
  Node* newNode = new Node(val, it.current_->prev, it.current_);
  // update pointers depending on its position given the node will be
  // added before the iterator
  if(empty()){
    head = tail = newNode;
    sentinelNode->next = newNode;
    sentinelNode->prev = newNode;
  }
  if(it.current_ == head){
    newNode->prev = sentinelNode;
    it.current_->prev = newNode;
    head = newNode;
  }
  else if(it.current_ == sentinelNode){
    tail->next = newNode;
    sentinelNode->prev = newNode;
    tail = newNode;
  } else {
    it.current_->prev->next = newNode;
    it.current_->prev = newNode;
  }
  numNodes++;
}

// remove *it from the list
template <typename T>
void MyList<T>::erase(const Iterator& it) {
  // update pointers depending on the position of the iterator
  if(it.current_ == head){
    sentinelNode->next = it.current_->next;
    it.current_->next->prev = sentinelNode;
    head = it.current_->next;
  }
  if(it.current_ == tail){
    sentinelNode->prev = it.current_->prev;
    it.current_->prev->next = sentinelNode;
    tail = it.current_->prev;
  } else {
    it.current_->prev->next = it.current_->next;
    it.current_->next->prev = it.current_->prev;
  }

  delete it.current_;
  numNodes--;
}

// Iterator constructor
template <typename T>
MyList<T>::Iterator::Iterator(Node* pointer) {
  current_ = pointer;
}

// (pre-)increment an iterator
template <typename T>
typename MyList<T>::Iterator& MyList<T>::Iterator::operator++() {
  current_ = current_->next;
  return *this;
}

// (pre-)decrement an iterator
template <typename T>
typename MyList<T>::Iterator& MyList<T>::Iterator::operator--() {
  current_ = current_->prev;
  return *this;
}

// return the value pointed at by an iterator
template <typename T>
T& MyList<T>::Iterator::operator*() const {
  return current_->data;
}

template class MyList<int>;
template class MyList<std::string>;
template class MyList<MyInteger>;