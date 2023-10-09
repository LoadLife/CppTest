#include <string>
#include <deque>
#include "stack.h"

using namespace std;
// stack<string> specialization

void Stack<string>::push(const string& element) {
  elements.push_back(element);
}

void Stack<string>::pop() {
  if(elements.empty()) {
    throw std::out_of_range("stack<string>::top(): empty stack!");
    return;
  } else {
    elements.pop_back();
  }
}

string Stack<string>::top() {
  if(elements.empty()) {
    throw std::out_of_range("stack<string>::top(): empty stack!");
  } else {
    return elements.back();
  }
}

size_t Stack<string>::size() {
  return elements.size();
}