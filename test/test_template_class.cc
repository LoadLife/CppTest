#include "gtest/gtest.h"
#include "stack.h"
using namespace std;
// 1.stack<int> test 
TEST(T_Class, test_stack) {
  Stack<int> stack;
  int ele = 3;
  stack.push(3);
  int tmp;
  tmp = stack.top();
  stack.pop();
  EXPECT_EQ(ele, tmp);
}

// 2.stack<string> specialization test
TEST(T_Class, test_stack_string) {
  try {
    Stack<string> stack;
    stack.pop();
  } catch(const exception& e) {
    cout << e.what() <<endl;
  }
}