#ifndef _STACK_H_
#define _STACK_H_
#include <vector>
#include <deque>

template <typename T>
class Stack {
 public:
  Stack() {}
  void push(T const& element);
  void pop(); 
  T top();
  size_t size();
  bool empty() {
    return elements.empty();
  }
 private:
  std::vector<T> elements;
};

// stack<string> specialization 
template<>
class Stack<std::string> {
 public:
  Stack() {}
  void push(const std::string& element);
  void pop(); 
  std::string top();
  size_t size();
  bool empty() {
    return elements.empty();
  }
 private:
  std::deque<std::string> elements;
};

// --------------------------template instantiation---------------------------------
// base stack
template<typename T>
void Stack<T>::push(T const& element) {
  elements.push_back(element);  
}

template<typename T>
void Stack<T>::pop() {
  if(elements.empty()) {
    throw std::out_of_range("stack<>::top(): empty stack!");
    return;
  } else {
    elements.pop_back();
  }
}

template<typename T>
T Stack<T>::top() {
  if(elements.empty()) {
    throw std::out_of_range("stack<>::top(): empty stack!");
  } else {
    return elements.back();
  }
}

template<typename T>
size_t size() {
  return elements.size();
}

#endif