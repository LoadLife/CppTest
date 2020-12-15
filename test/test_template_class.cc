#include "gtest/gtest.h"
#include "stack.h"
// 1.stack test 
TEST(Template_Test, test_stack) {
  Stack<int> stack;
  int ele = 3;
  stack.push(3);
  int tmp;
  tmp = stack.top();
  stack.pop();
  EXPECT_EQ(ele, tmp);
}