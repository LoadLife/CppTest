#include <iostream>
#include "gtest/gtest.h"
#include "stack.h"
using namespace std;

class Template_Test : public ::testing::Test {
private:
protected:
    virtual void SetUp();
    virtual void TearDown();
};

void Template_Test::SetUp() {
}
void Template_Test::TearDown() {
}
// ---------------------------------------Test Case-----------------------------------
TEST_F(Template_Test, test_stack) {
  Stack<int> stack;
  int ele = 3;
  stack.push(3);
  int tmp;
  stack.pop(tmp);
  EXPECT_EQ(ele, tmp);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}