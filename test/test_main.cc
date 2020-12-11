#include <iostream>
#include "gtest/gtest.h"
#include "stack.h"
using namespace std;
/*---------------------GTest Class Base-----------------------------
class Template_Test : public ::testing::Test {
protected:
    virtual void SetUp();
    virtual void TearDown();
};
void Template_Test::SetUp() {
}
void Template_Test::TearDown() {
}
---------------------------------------------------------------------- */


//----------------------Test Case------------------------------------
TEST(Template_Test, test_stack) {
  Stack<int> stack;
  int ele = 3;
  stack.push(3);
  int tmp;
  tmp = stack.top();
  stack.pop();
  EXPECT_EQ(ele, tmp);
}

// strut mem memory align test
struct s_1 {
  char a;
  int b;
  double c;
};
struct s_2 {
  int a;
  double b;
  char c;
};
/*  default align mode
  1. 当前元素偏移量是当前元素大小的整数倍
  2. 整个struct的大小是最大元素的整数倍
  3. 当struct中有其它struct时候，内部struct的偏移量必须为其中最大元素大小的整数倍
*/
TEST(Align_Test, struct_test) {
  EXPECT_NE(sizeof(s_1), 13); 
  EXPECT_EQ(sizeof(s_1), 16);

  EXPECT_NE(sizeof(s_2), 13);
  EXPECT_EQ(sizeof(s_2), 24);
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}