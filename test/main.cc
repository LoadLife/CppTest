#include <iostream>
#include "stack.h"
using namespace std;
void test_stack();
int main() {

}

void test_stack() {
  Stack<int> s;
  s.push(3);
  int b;
  s.pop(b);
  cout << "b: " << b <<endl;
  cout << "stack's size : " << s.size() << endl;
  cout << __FILE__<< endl;
}