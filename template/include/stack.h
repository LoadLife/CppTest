#ifndef _STACK_H_
#define _STACK_H_
#include <vector>

template <typename T>
class Stack{
 public:
  Stack() {}

  void push(T element) {
    elements.push_back(element);  
  }

  void pop(){
    if(elements.empty()) {
      throw std::out_of_range("stack<>::top(): empty stack!");
      return;
    }
    else {
      elements.pop_back();
    }
  }  

  T top() {
    if(elements.empty()) {
      throw std::out_of_range("stack<>::top(): empty stack!");
    }
    else {
      return elements.back();
    }
  }

  size_t size() {
    return elements.size();
  }
 private:
  std::vector<T> elements;
};
#endif