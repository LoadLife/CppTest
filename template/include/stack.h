#include <vector>

template <typename T>
class Stack{
 public:
  Stack() {}

  void push(T element) {
    elements.push_back(element);  
  }

  void pop(T& ele){
    if(elements.empty()) {
      std::cerr << "this stack is empty" << std::endl;
      return;
    }
    else {
      ele = elements.back();
      elements.pop_back();
    }
  }  

  T top(){
    if(elements.empty()) {
      std::cerr << "this stack is empty" << std::endl;
      return;
    }
    else {
      return elements.pop_back();
    }
  }

  size_t size(){
    return elements.size();
  }
 private:
  std::vector<T> elements;
};
