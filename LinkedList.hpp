#include <cstddef>
#include <iterator>
#include <exception>


#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP


template<typename T> class list {
private:
  struct node;
  template<typename Compare = std::less<>> class mergeSort;

  node* first;
  node* last;
  std::size_t length;

  void _clear() {
    node* p = first;
    node* nextp;
    while(p != nullptr) {
      nextp = p->next;
      delete p;
      p = nextp;
    }
    first = nullptr;
    last = nullptr;
    length = 0;
  }

  static node* _incrementBy(node* start, const std::size_t& elements) {
    for(std::size_t i = 0; i < elements; ++i) {
      start = start->next;
    }
    return start;
  }
  static const node* _incrementBy(const node* start, const std::size_t& elements) {
    for(std::size_t i = 0; i < elements; ++i) {
      start = start->next;
    }
    return start;
  }

  node* _findNode(const std::size_t& index) {
    node* p = first;
    for(std::size_t i = 0; i < index; ++i) {
      p = p->next;
    }
    return p;
  }
  const node* _findNode(const std::size_t& index) const {
    const node* p = first;
    for(std::size_t i = 0; i < index; ++i) {
      p = p->next;
    }
    return p;
  }

  node* _findLast() {
    return _findNode(length - 1);
  }
  const node* _findLast() const {
    return _findNode(length - 1);
  }

  void _resize(const std::size_t& size, const T* const value) {
    if(length > size) {
      if(size <= 0) {
        _clear();
      } else {
        node* p = first;
        for(std::size_t i = 0; i < size - 1; ++i) {
          p = p->next;
        }
        last = p;

        node* nextp;
        nextp = p->next;
        p->next = nullptr;
        for(std::size_t i = size; i < length; ++i) {
          p = nextp;
          nextp = p->next;
          delete p;
        }

        length = size;
      }
    } else if(length < size) {
      node* p = last;

      if(value == nullptr) {
        for(std::size_t i = length - 1; i < size - 1; ++i) {
          p->next = new node;
          p = p->next;
        }
      } else {
        for(std::size_t i = length - 1; i < size - 1; ++i) {
          p->next = new node(new T(*value));
          p = p->next;
        }
      }

      last = p;
      length = size;
    }
  }

  void _add(T* const value) {
    if(length == 0) {
      first = new node(value);
      last = first;
      length++;
    } else {
      last->next = new node(value);
      last = last->next;
      length++;
    }
  }

  void _insert(const std::size_t& index, T* const value) {
    if(index == length) {
      _add(value);
    } else if(index == 0) {
      first = new node(value, first);
      length++;
    } else {
      node* p = _findNode(index - 1);
      p->next = new node(value, p->next);
      length++;
    }
  }

  void _pop() {
    if(length == 1) {
      // Delete the only element
      delete first;
      first = nullptr;
      last = nullptr;
      length = 0;
    } else if(length > 1) {
      // Delete the last element
      delete last;
      length--;

      // Find the new last element
      last = _findLast();

      // Clear its now-deleted next node
      last->next = nullptr;
    }
  }

  void _removeExceptBack(node* const ptr) {
    node* p = ptr->next;
    if(p == last) {
      last = ptr;
    }

    delete ptr->value;
    ptr->value = p->value;
    ptr->next = p->next;

    p->value = nullptr;
    delete p;
    length--;
  }
  void _remove(node*& ptr) {
    if(ptr->next == nullptr) {
      _pop();
      ptr = nullptr;
    } else {
      _removeExceptBack(ptr);
    }
  }


public:
  class iterator;
  class const_iterator;
  class OutOfRange;

  list<T>() : first(nullptr), last(nullptr), length(0) {}

  /* Deep Copy */
  list<T>(const list<T>& other) : first(nullptr), last(nullptr), length(other.length) {
    if(length != 0) {
      node* otherp = other.first; // Always one ahead of thisp
      node* thisp = new node(new T(*otherp->value));
      first = thisp;

      otherp = otherp->next;
      while(otherp != nullptr) {
        thisp->next = new node(new T(*otherp->value));
        thisp = thisp->next;
        otherp = otherp->next;
      }
      last = thisp;
    }
  }

  /* Deep Copy */
  list<T>& operator=(const list<T>& other) {
    if(this == &other) {
      return *this;
    }

    // Add a first node if it doesn't have one
    if(length == 0) {
      first = new node;
    }
    // Shorten it if necessary
    if(length > other.length) {
      _resize(other.length, nullptr);
    }

    // Assign values
    node* thisp = first;
    node* otherp = other.first;
    while(true) {
      delete thisp->value;
      thisp->value = new T(*otherp->value);

      if(thisp->next == nullptr) break;
      thisp = thisp->next;
      otherp = otherp->next;
    }

    // thisp->next should now be nullptr
    otherp = otherp->next; // Set otherp to one ahead of thisp
    while(otherp != nullptr) {
      thisp->next = new node(new T(*otherp->value));
      thisp = thisp->next;
      otherp = otherp->next;
    }
    last = thisp;
    length = other.length;
    
    return *this;
  }

  ~list() {
    node* p = first;
    node* nextp;
    while(p != nullptr) {
      nextp = p->next;
      delete p;
      p = nextp;
    }
  }

  iterator begin() { return iterator(first); }
  const_iterator begin() const { return const_iterator(first); }

  iterator end() { return nullptr; }
  const_iterator end() const { return nullptr; }

  std::size_t size() const { return length; }

  T& operator[](const std::size_t& index) { return *_findNode(index)->value; }
  const T& operator[](const std::size_t& index) const { return *_findNode(index)->value; }

  T& at(const std::size_t& index) {
    if(index < 0 || index >= length) {
      throw OutOfRange(length, index);
    }
    return operator[](index);
  }
  const T& at(const std::size_t& index) const {
    if(index < 0 || index >= length) {
      throw OutOfRange(length, index);
    }
    return operator[](index);
  }

  T& back() { return *last->value; }
  const T& back() const { return *last->value; }

  void clear() { _clear(); }

  void resize(const std::size_t& size) { _resize(size, nullptr); }
  void resize(const std::size_t& size, const T& value) { _resize(size, &value); }

  void add(const T& value) {
    _add(new T(value));
  }

  void insert(const std::size_t& index, const T& value) {
    if(index < 0 || index > length) {
      throw OutOfRange(length, index);
    }
    _insert(index, new T(value));
  }

  void pop() { _pop(); }

  void remove(const std::size_t& index) {
    if(index < 0 || index >= length) {
      throw OutOfRange(length, index);
    }
    node* p = _findNode(index);
    _remove(p);
  }
  void remove(iterator& it) {
    _remove(it.ptr);
  }
  void remove(const iterator& it) {
    if(!it.nextValid()) {
      _pop();
    } else {
      _removeExceptBack(it.ptr);
    }
  }

  // Overwrites list other with the data in this list, removing the data in this list
  void transferTo(list& other) {
    if(this == &other) {
      return;
    }

    other.clear();
    other.first = first;
    other.last = last;
    other.length = length;
    
    first = nullptr;
    last = nullptr;
    length = 0;
  }

  // Reverses the order of the list
  void reverse() {
    if(length == 0) return;

    node* head = first->next;
    first->next = nullptr;
    last = first;

    node* p;
    while(head != nullptr) {
      p = first;
      first = head;
      head = head->next;
      first->next = p;
    }
  }

  template<typename Compare = std::less<>>
  void sort(Compare comp = Compare()) {
    mergeSort<Compare> s(first, comp);
    s.sort();

    last = _findLast();
  }

  bool checkLast() const {
    const node* finalNode = _findLast();
    return (finalNode->next == nullptr);
  }
};


template<typename T> struct list<T>::node {
  T* value;
  node* next;

  node() : value(nullptr), next(nullptr) {}
  node(T* const value) : value(value), next(nullptr) {}
  node(T* const value, node* const next) : value(value), next(next) {}

  node(const node& other) : next(nullptr) { // Deep Copy
    value = new T(*other.value);
  }
  node& operator=(const node& other) { // Deep Copy
    if(value == other.value) {
      return *this;
    }

    delete value;
    value = new T(other.value);
    next = other.next;
    return *this;
  }

  ~node() {
    delete value;
  }
};


template<typename T> class list<T>::iterator {
  using iterator_category = std::forward_iterator_tag;
  friend class list<T>;

private:
  node* ptr;

  iterator(node& n) : ptr(&n) {}
  iterator(node* const n) : ptr(n) {}

public:
  iterator() : ptr(nullptr) {}
  iterator(std::nullptr_t) : ptr(nullptr) {}

  operator const_iterator() const { return const_iterator(ptr); }

  T& operator*() const { return *ptr->value; }
  T* operator->() const { return ptr->value; }
  T& value() const {
    if(ptr == nullptr) throw std::runtime_error("Trying to access null iterator.");
    if(ptr->value == nullptr) throw std::runtime_error("Trying to access null node value.");
    return *ptr->value;
  }
  T& nextValue() const { return *ptr->next->value; }

  iterator& operator++() {
    if(ptr) {
      ptr = ptr->next;
    }
    return *this;
  }
  iterator operator++(int) {
    iterator prev = *this;
    if(ptr) {
      ptr = ptr->next;
    }
    return prev;
  }
  iterator next() const { return iterator(ptr->next); }

  bool operator==(const iterator& other) const { return ptr == other.ptr; }
  bool operator<(const iterator& other) const { return ptr < other.ptr; }
  bool operator>(const iterator& other) const { return ptr > other.ptr; }

  iterator operator+(int other) const { return _incrementBy(ptr, other); }
  iterator operator+(const std::size_t& other) const { return _incrementBy(ptr, other); }

  operator bool() const { return ptr != nullptr; }
  bool isnull() const { return ptr == nullptr; }
  bool valid() const { return ptr != nullptr; }
  bool nextValid() const { return ptr->next != nullptr; }

  void clear() const { ptr = nullptr; }
};

template<typename T> class list<T>::const_iterator {
  using iterator_category = std::forward_iterator_tag;
  friend class list<T>;

private:
  const node* ptr;
  
  const_iterator(const node& n) : ptr(&n) {}
  const_iterator(const node* const n) : ptr(n) {}

public:
  const_iterator() : ptr(nullptr) {}
  const_iterator(std::nullptr_t) : ptr(nullptr) {}

  const_iterator(const iterator& other) : ptr(other.ptr) {}
  const_iterator& operator=(const iterator& other) { ptr = other.ptr; return *this; }

  const T& operator*() const { return *ptr->value; }
  const T* operator->() const { return ptr->value; }

  const_iterator& operator++() {
    if(ptr) {
      ptr = ptr->next;
    }
    return *this;
  }
  const_iterator operator++(int) {
    const_iterator prev = *this;
    if(ptr) {
      ptr = ptr->next;
    }
    return prev;
  }
  const_iterator next() const { return const_iterator(ptr->next); }

  bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
  bool operator<(const const_iterator& other) const { return ptr < other.ptr; }
  bool operator>(const const_iterator& other) const { return ptr > other.ptr; }

  const_iterator operator+(int other) const { return _incrementBy(ptr, other); }
  const_iterator operator+(const std::size_t& other) const { return _incrementBy(ptr, other); }

  operator bool() const { return ptr != nullptr; }
  bool isnull() const { return ptr == nullptr; }
  bool valid() const { return ptr != nullptr; }
  bool nextValid() const { return ptr->next != nullptr; }

  const T& node() const {
    if(ptr == nullptr) throw std::runtime_error("Trying to access null iterator.");
    if(ptr->value == nullptr) throw std::runtime_error("Trying to access null node value.");
    return *ptr->value;
  }
  
  void clear() const { ptr = nullptr; }
};


template<typename T>
class list<T>::OutOfRange : public std::exception
{
  std::string msg;

public:
  OutOfRange() : msg("Linked List index out of range.") {}
  OutOfRange(std::size_t containerSize, std::size_t requestedSize) : msg("Linked List index out of range.") {
    msg += " Container size: " + std::to_string(containerSize);
    msg += "; requested index: " + std::to_string(requestedSize);
  }

  virtual const char* what() const throw()
  {
    return msg.c_str();
  }
};


template<typename T>
template<typename Compare>
class list<T>::mergeSort {
private:
  node** start;
  Compare compare;

  // Split the linked list into two halves
  void _splitList(node* source, node** front, node** back) {
    node* slow = source;
    node* fast = source->next;

    // Fast moves 2 nodes and slow moves 1 node
    while (fast != nullptr) {
      fast = fast->next;
      if (fast != nullptr) {
        slow = slow->next;
        fast = fast->next;
      }
    }

    *front = source;
    *back = slow->next;
    slow->next = nullptr;
  }

  // Merge two sorted linked lists
  node* _mergeLists(node* a, node* b) {
    node* result = nullptr;
    node** end = &result;

    while(a && b) {
      if (compare(*a->value, *b->value)) {
        *end = a;
        a = a->next;
        end = &(*end)->next;
      } else {
        *end = b;
        b = b->next;
        end = &(*end)->next;
      }
    }

    if(a == nullptr) {
      *end = b;
    } else {
      *end = a;
    }

    return result;
  }

  // Main Merge Sort function
  void _sort(node** headRef) {
    node* head = *headRef;
    if (head == nullptr || head->next == nullptr) {
      return;
    }

    node* a;
    node* b;

    // Split the list into two halves
    _splitList(head, &a, &b);

    // Recursively sort the two halves
    _sort(&a);
    _sort(&b);

    // Merge the sorted halves
    *headRef = _mergeLists(a, b);
  }

public:
  mergeSort(node*& start, Compare comp = Compare()) : start(&start), compare(comp) {}
  mergeSort(list<T>& linkedlist, Compare comp = Compare()) : start(&linkedlist.first), compare(comp) {}

  // Sorts list. Doesn't update last element.
  void sort() {
    _sort(start);
  }
};


#endif /* LINKED_LIST_HPP */
