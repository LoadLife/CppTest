#include "gtest/gtest.h"
#include "stack.h"
using namespace std;
// stack<int> test 
TEST(T_Class, test_stack) {
  Stack<int> stack;
  int ele = 3;
  stack.push(3);
  int tmp;
  tmp = stack.top();
  stack.pop();
  ASSERT_EQ(ele, tmp);
}

// stack<string> specialization test
TEST(T_Class, test_stack_string) {
  try {
    Stack<string> stack;
    stack.pop();
  } catch(const exception& e) {
    cout << e.what() <<endl;
  }
}